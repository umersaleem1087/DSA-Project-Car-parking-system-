#include "include/ParkingSystem.h"
#include <iostream>
#include <cmath>

// ============================================================================
// PARKING SYSTEM IMPLEMENTATION
// ============================================================================

ParkingSystem::ParkingSystem() {
    engine = new AllocationEngine();
    rollbackManager = new RollbackManager();
}

ParkingSystem::~ParkingSystem() {
    // Clean up master history
    while (!masterHistoryList.isEmpty()) {
        ParkingRequest* req = masterHistoryList.getFront();
        masterHistoryList.removeFront();
        delete req;
    }
    
    delete engine;
    delete rollbackManager;
}

// ========================================================================
// SYSTEM INITIALIZATION
// ========================================================================

void ParkingSystem::addZone(Zone* zone) {
    if (zone != nullptr) {
        engine->addZone(zone);
    }
}

// ========================================================================
// REQUEST PROCESSING
// ========================================================================

ParkingSlot* ParkingSystem::requestParking(Vehicle* vehicle) {
    if (vehicle == nullptr) {
        std::cout << "Invalid vehicle" << std::endl;
        return nullptr;
    }
    
    // Create parking request
    ParkingRequest* request = new ParkingRequest(vehicle->getVehicleID(), 
                                                  vehicle->getPreferredZoneID());
    
    // Try to allocate slot
    ParkingSlot* allocatedSlot = engine->allocateSlot(vehicle, request);
    
    // Add to master history regardless of outcome
    masterHistoryList.insertBack(request);
    
    if (allocatedSlot != nullptr) {
        // Record successful allocation
        Command cmd(request, allocatedSlot, RequestState::REQUESTED, RequestState::ALLOCATED);
        rollbackManager->recordCommand(cmd);
        request->updateState(RequestState::ALLOCATED);
        
        std::cout << "Successfully allocated Slot " << allocatedSlot->getSlotID() 
                  << " for Vehicle " << vehicle->getVehicleID() << std::endl;
    } else {
        // Mark request as cancelled
        request->updateState(RequestState::CANCELLED);
        std::cout << "Failed to allocate parking for Vehicle " 
                  << vehicle->getVehicleID() << std::endl;
    }
    
    return allocatedSlot;
}

bool ParkingSystem::occupySlot(ParkingRequest* request, ParkingSlot* slot) {
    if (request == nullptr || slot == nullptr) {
        return false;
    }
    
    // Record state change for rollback
    RequestState oldState = request->getCurrentStatus();
    
    if (!request->updateState(RequestState::OCCUPIED)) {
        std::cout << "Cannot transition request to OCCUPIED state" << std::endl;
        return false;
    }
    
    // Record command
    Command cmd(request, slot, oldState, RequestState::OCCUPIED);
    rollbackManager->recordCommand(cmd);
    
    std::cout << "Vehicle " << request->getVehicleID() 
              << " now occupying Slot " << slot->getSlotID() << std::endl;
    
    return true;
}

bool ParkingSystem::releaseSlot(ParkingRequest* request) {
    if (request == nullptr) {
        return false;
    }
    
    // Record state change for rollback
    RequestState oldState = request->getCurrentStatus();
    
    if (!request->updateState(RequestState::RELEASED)) {
        std::cout << "Cannot transition request to RELEASED state" << std::endl;
        return false;
    }
    
    std::cout << "Vehicle " << request->getVehicleID() << " has released parking" << std::endl;
    
    return true;
}

bool ParkingSystem::cancelRequest(ParkingRequest* request) {
    if (request == nullptr) {
        return false;
    }
    
    RequestState oldState = request->getCurrentStatus();
    
    if (!request->updateState(RequestState::CANCELLED)) {
        std::cout << "Cannot cancel request in current state" << std::endl;
        return false;
    }
    
    std::cout << "Request for Vehicle " << request->getVehicleID() 
              << " has been cancelled" << std::endl;
    
    return true;
}

// ========================================================================
// ROLLBACK OPERATIONS
// ========================================================================

bool ParkingSystem::rollbackLastOperations(int k) {
    return rollbackManager->performRollback(k);
}

// ========================================================================
// ANALYTICS - MODULE F
// ========================================================================

double ParkingSystem::calculateAverageDuration() const {
    if (masterHistoryList.isEmpty()) {
        std::cout << "No requests in history" << std::endl;
        return 0.0;
    }
    
    int totalDuration = 0;
    int validRequests = 0;
    
    Node<ParkingRequest*>* current = masterHistoryList.getHead();
    
    while (current != nullptr) {
        ParkingRequest* req = current->data;
        
        if (req != nullptr) {
            RequestState status = req->getCurrentStatus();
            
            // Only count completed requests (RELEASED or OCCUPIED)
            if (status == RequestState::RELEASED || status == RequestState::OCCUPIED) {
                // Calculate duration from request time to now (simplified: assume 1 hour per request)
                // In a real system, would subtract release time from request time
                totalDuration += 60;  // Placeholder: 60 minutes per request
                validRequests++;
            }
        }
        
        current = current->next;
    }
    
    if (validRequests == 0) {
        return 0.0;
    }
    
    return (double)totalDuration / validRequests;
}

double ParkingSystem::getZoneUtilizationRate(int zoneID) const {
    DoublyLinkedList<Zone*>& allZones = engine->getAllZones();
    
    Node<Zone*>* current = allZones.getHead();
    
    while (current != nullptr) {
        if (current->data != nullptr && current->data->getZoneID() == zoneID) {
            Zone* zone = current->data;
            
            int totalCapacity = zone->getTotalCapacity();
            int availableSlots = zone->getAvailableSlots();
            
            if (totalCapacity == 0) {
                return 0.0;
            }
            
            int occupiedSlots = totalCapacity - availableSlots;
            double utilizationRate = ((double)occupiedSlots / totalCapacity) * 100.0;
            
            return utilizationRate;
        }
        
        current = current->next;
    }
    
    std::cout << "Zone " << zoneID << " not found" << std::endl;
    return -1.0;
}

void ParkingSystem::displayZoneAnalytics() const {
    DoublyLinkedList<Zone*>& allZones = engine->getAllZones();
    
    if (allZones.getHead() == nullptr) {
        std::cout << "No zones in system" << std::endl;
        return;
    }
    
    std::cout << "\n========== ZONE ANALYTICS ==========" << std::endl;
    
    Node<Zone*>* current = allZones.getHead();
    int zoneIndex = 0;
    
    while (current != nullptr) {
        if (current->data != nullptr) {
            Zone* zone = current->data;
            zoneIndex++;
            
            int totalCapacity = zone->getTotalCapacity();
            int availableSlots = zone->getAvailableSlots();
            int occupiedSlots = totalCapacity - availableSlots;
            
            double utilizationRate = 0.0;
            if (totalCapacity > 0) {
                utilizationRate = ((double)occupiedSlots / totalCapacity) * 100.0;
            }
            
            std::cout << "\nZone " << zone->getZoneID() << ":" << std::endl;
            std::cout << "  Total Capacity: " << totalCapacity << std::endl;
            std::cout << "  Occupied Slots: " << occupiedSlots << std::endl;
            std::cout << "  Available Slots: " << availableSlots << std::endl;
            std::cout << "  Utilization Rate: " << utilizationRate << "%" << std::endl;
        }
        
        current = current->next;
    }
    
    std::cout << "==================================\n" << std::endl;
}

int ParkingSystem::getTotalRequests() const {
    return masterHistoryList.getSize();
}

int ParkingSystem::getRequestCountByStatus(RequestState status) const {
    int count = 0;
    
    Node<ParkingRequest*>* current = masterHistoryList.getHead();
    
    while (current != nullptr) {
        if (current->data != nullptr && current->data->getCurrentStatus() == status) {
            count++;
        }
        current = current->next;
    }
    
    return count;
}

void ParkingSystem::displayMasterHistory() const {
    if (masterHistoryList.isEmpty()) {
        std::cout << "Master history is empty" << std::endl;
        return;
    }
    
    std::cout << "\n========== MASTER HISTORY ==========" << std::endl;
    std::cout << "Total Requests: " << getTotalRequests() << std::endl;
    std::cout << "  - REQUESTED: " << getRequestCountByStatus(RequestState::REQUESTED) << std::endl;
    std::cout << "  - ALLOCATED: " << getRequestCountByStatus(RequestState::ALLOCATED) << std::endl;
    std::cout << "  - OCCUPIED: " << getRequestCountByStatus(RequestState::OCCUPIED) << std::endl;
    std::cout << "  - RELEASED: " << getRequestCountByStatus(RequestState::RELEASED) << std::endl;
    std::cout << "  - CANCELLED: " << getRequestCountByStatus(RequestState::CANCELLED) << std::endl;
    
    std::cout << "\nAverage Duration: " << calculateAverageDuration() << " minutes" << std::endl;
    
    std::cout << "\nRequest Details:" << std::endl;
    
    Node<ParkingRequest*>* current = masterHistoryList.getHead();
    int index = 0;
    
    while (current != nullptr) {
        if (current->data != nullptr) {
            index++;
            std::cout << "\n[" << index << "] ";
            current->data->displayInfo();
        }
        current = current->next;
    }
    
    std::cout << "\n==================================\n" << std::endl;
}

// ========================================================================
// GETTERS
// ========================================================================

AllocationEngine* ParkingSystem::getEngine() const {
    return engine;
}

RollbackManager* ParkingSystem::getRollbackManager() const {
    return rollbackManager;
}

DoublyLinkedList<ParkingRequest*>& ParkingSystem::getMasterHistory() {
    return masterHistoryList;
}

int ParkingSystem::getMasterHistorySize() const {
    return masterHistoryList.getSize();
}
