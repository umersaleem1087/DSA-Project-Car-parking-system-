#include "include/Zone.h"
#include <iostream>

// ============================================================================
// ZONE IMPLEMENTATION
// ============================================================================

Zone::Zone(int id)
    : zoneID(id), totalCapacity(0) {
}

Zone::~Zone() {
    // Clean up parking areas
    while (!parkingAreas.isEmpty()) {
        ParkingArea* area = parkingAreas.getFront();
        parkingAreas.removeFront();
        delete area;
    }
    
    // Note: We don't delete adjacent zones as they may be used elsewhere
    // Just clear the list
    while (!adjacentZones.isEmpty()) {
        adjacentZones.removeFront();
    }
}

// ========================================================================
// ZONE MANAGEMENT
// ========================================================================

void Zone::addParkingArea(ParkingArea* area) {
    if (area != nullptr) {
        parkingAreas.insertBack(area);
        totalCapacity += area->getTotalSlots();
    }
}

ParkingArea* Zone::findParkingAreaByID(int areaID) {
    Node<ParkingArea*>* current = parkingAreas.getHead();
    while (current != nullptr) {
        if (current->data->getAreaID() == areaID) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

// ========================================================================
// ADJACENCY/NEIGHBOR MANAGEMENT
// ========================================================================

void Zone::addAdjacentZone(Zone* zone) {
    if (zone != nullptr) {
        adjacentZones.insertBack(zone);
    }
}

void Zone::removeAdjacentZone(Zone* zone) {
    if (zone == nullptr) return;
    
    Node<Zone*>* current = adjacentZones.getHead();
    while (current != nullptr) {
        if (current->data == zone) {
            adjacentZones.removeNode(current);
            return;
        }
        current = current->next;
    }
}

DoublyLinkedList<Zone*>& Zone::getAdjacentZones() {
    return adjacentZones;
}

// ========================================================================
// SLOT SEARCH
// ========================================================================

ParkingSlot* Zone::findAvailableSlot() {
    Node<ParkingArea*>* current = parkingAreas.getHead();
    while (current != nullptr) {
        ParkingSlot* availableSlot = current->data->findAvailableSlot();
        if (availableSlot != nullptr) {
            return availableSlot;
        }
        current = current->next;
    }
    return nullptr;
}

// ========================================================================
// GETTERS
// ========================================================================

int Zone::getZoneID() const {
    return zoneID;
}

int Zone::getTotalCapacity() const {
    return totalCapacity;
}

int Zone::getAvailableSlots() const {
    int available = 0;
    Node<ParkingArea*>* current = parkingAreas.getHead();
    while (current != nullptr) {
        available += current->data->getAvailableSlots();
        current = current->next;
    }
    return available;
}

DoublyLinkedList<ParkingArea*>& Zone::getParkingAreas() {
    return parkingAreas;
}

// ========================================================================
// UTILITY METHODS
// ========================================================================

void Zone::displayInfo() const {
    std::cout << "Zone ID: " << zoneID 
              << ", Total Capacity: " << totalCapacity 
              << ", Available Slots: " << getAvailableSlots() << std::endl;
}

void Zone::refreshCapacity() {
    totalCapacity = 0;
    Node<ParkingArea*>* current = parkingAreas.getHead();
    while (current != nullptr) {
        current->data->refreshAvailableCount();
        totalCapacity += current->data->getTotalSlots();
        current = current->next;
    }
}
