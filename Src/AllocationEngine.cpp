#include "include/AllocationEngine.h"
#include <iostream>

// ============================================================================
// ALLOCATION ENGINE IMPLEMENTATION
// ============================================================================

AllocationEngine::AllocationEngine() {
}

AllocationEngine::~AllocationEngine() {
    // Note: We don't delete zones here as they may be managed elsewhere
    // Just clear the list
    while (!allZones.isEmpty()) {
        allZones.removeFront();
    }
}

// ========================================================================
// ZONE MANAGEMENT
// ========================================================================

void AllocationEngine::addZone(Zone* zone) {
    if (zone != nullptr) {
        allZones.insertBack(zone);
    }
}

// ========================================================================
// SEARCH HELPERS
// ========================================================================

Zone* AllocationEngine::findZoneByID(int zoneID) {
    Node<Zone*>* current = allZones.getHead();
    while (current != nullptr) {
        if (current->data->getZoneID() == zoneID) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

ParkingSlot* AllocationEngine::searchInZone(Zone* zone) {
    if (zone == nullptr) return nullptr;
    
    // Search for available slot in the zone
    return zone->findAvailableSlot();
}

ParkingSlot* AllocationEngine::searchInAdjacentZones(Zone* primaryZone, ParkingRequest* parkingRequest) {
    if (primaryZone == nullptr || parkingRequest == nullptr) return nullptr;
    
    DoublyLinkedList<Zone*>& adjacentZones = primaryZone->getAdjacentZones();
    Node<Zone*>* current = adjacentZones.getHead();
    
    while (current != nullptr) {
        ParkingSlot* availableSlot = current->data->findAvailableSlot();
        
        if (availableSlot != nullptr) {
            // Found slot in adjacent zone - apply penalty
            std::cout << "Slot found in adjacent zone (Zone " << current->data->getZoneID() 
                      << "). Applying penalty..." << std::endl;
            
            parkingRequest->addPenaltyCost(PenaltyCosts::INVALID_ALLOCATION_PENALTY);
            
            return availableSlot;
        }
        
        current = current->next;
    }
    
    return nullptr;
}

// ========================================================================
// ALLOCATION ALGORITHM
// ========================================================================

ParkingSlot* AllocationEngine::allocateSlot(Vehicle* vehicle, ParkingRequest* parkingRequest) {
    if (vehicle == nullptr || parkingRequest == nullptr) {
        std::cout << "Invalid vehicle or parking request" << std::endl;
        return nullptr;
    }
    
    int requestedZoneID = vehicle->getPreferredZoneID();
    
    // Step 1: Find the requested zone
    Zone* requestedZone = findZoneByID(requestedZoneID);
    
    if (requestedZone == nullptr) {
        std::cout << "Requested zone (ID: " << requestedZoneID << ") not found" << std::endl;
        return nullptr;
    }
    
    // Step 2: Search in the requested zone first
    std::cout << "Searching for parking in Zone " << requestedZoneID << "..." << std::endl;
    ParkingSlot* allocatedSlot = searchInZone(requestedZone);
    
    if (allocatedSlot != nullptr) {
        std::cout << "Slot found in requested zone (Slot ID: " << allocatedSlot->getSlotID() << ")" << std::endl;
        allocatedSlot->allocate();
        return allocatedSlot;
    }
    
    // Step 3: Fallback to adjacent zones
    std::cout << "No slots in requested zone. Checking adjacent zones..." << std::endl;
    allocatedSlot = searchInAdjacentZones(requestedZone, parkingRequest);
    
    if (allocatedSlot != nullptr) {
        allocatedSlot->allocate();
        return allocatedSlot;
    }
    
    // Step 4: No slot available anywhere
    std::cout << "No parking slot available in requested zone or adjacent zones" << std::endl;
    parkingRequest->addPenaltyCost(PenaltyCosts::CANCELLATION_PENALTY);
    return nullptr;
}

// ========================================================================
// GETTERS
// ========================================================================

DoublyLinkedList<Zone*>& AllocationEngine::getAllZones() {
    return allZones;
}
