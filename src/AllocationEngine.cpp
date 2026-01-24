#include "AllocationEngine.h"
#include "ParkingArea.h"
#include <iostream>

AllocationEngine::AllocationEngine() {}

AllocationEngine::~AllocationEngine() {}

Zone* AllocationEngine::findZoneByID(int zoneID) {
    Node<Zone*>* current = allZones.getHead();
    while (current != nullptr) {
        if (current->data != nullptr && current->data->getZoneID() == zoneID) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

void AllocationEngine::addZone(Zone* zone) {
    if (zone != nullptr) {
        allZones.insertBack(zone);
    }
}

ParkingSlot* AllocationEngine::allocateSlot(Vehicle* vehicle, ParkingRequest* parkingRequest) {
    if (vehicle == nullptr || parkingRequest == nullptr) {
        return nullptr;
    }

    // Strategy 1: Try to allocate in requested zone
    Zone* requestedZone = findZoneByID(parkingRequest->getRequestedZoneID());
    if (requestedZone != nullptr) {
        ParkingSlot* slot = requestedZone->findAvailableSlot();
        if (slot != nullptr) {
            slot->allocate();
            requestedZone->refreshCapacity();  // Refresh available counts
            parkingRequest->setAllocatedSlotID(slot->getSlotID());  // Store slot ID
            parkingRequest->updateState(RequestState::ALLOCATED);
            return slot;
        }
    }

    // Strategy 2: Try other zones with penalty
    Node<Zone*>* current = allZones.getHead();
    while (current != nullptr) {
        Zone* zone = current->data;
        if (zone != nullptr && zone->getZoneID() != parkingRequest->getRequestedZoneID()) {
            ParkingSlot* slot = zone->findAvailableSlot();
            if (slot != nullptr) {
                slot->allocate();
                zone->refreshCapacity();  // Refresh available counts
                parkingRequest->setAllocatedSlotID(slot->getSlotID());  // Store slot ID
                parkingRequest->updateState(RequestState::ALLOCATED);
                parkingRequest->addPenaltyCost(10.0); // Cross-zone penalty
                std::cout << "[Info] Cross-zone penalty applied: $10.0" << std::endl;
                return slot;
            }
        }
        current = current->next;
    }

    return nullptr; // No slot available anywhere
}

bool AllocationEngine::freeSlot(int slotID) {
    // Search for the slot in all zones
    Node<Zone*>* zoneNode = allZones.getHead();
    while (zoneNode != nullptr) {
        Zone* zone = zoneNode->data;
        if (zone != nullptr) {
            // Search through all parking areas in this zone
            auto& areas = zone->getParkingAreas();
            Node<ParkingArea*>* areaNode = areas.getHead();
            while (areaNode != nullptr) {
                ParkingArea* area = areaNode->data;
                if (area != nullptr) {
                    ParkingSlot* slot = area->findSlotByID(slotID);
                    if (slot != nullptr) {
                        slot->free();  // Free the slot
                        zone->refreshCapacity();  // Refresh available counts
                        std::cout << "✅ Slot " << slotID << " has been freed\n";
                        return true;
                    }
                }
                areaNode = areaNode->next;
            }
        }
        zoneNode = zoneNode->next;
    }
    
    std::cerr << "❌ ERROR: Slot " << slotID << " not found!\n";
    return false;
}

DoublyLinkedList<Zone*>& AllocationEngine::getAllZones() {
    return allZones;
}