#include "Zone.h"
#include "ParkingArea.h"
#include <iostream>

Zone::Zone(int id) : zoneID(id), totalCapacity(0) {}

Zone::~Zone() {}

void Zone::addParkingArea(ParkingArea* area) {
    if (area != nullptr) {
        parkingAreas.insertBack(area);
        totalCapacity += area->getTotalSlots();
    }
}

ParkingArea* Zone::findParkingAreaByID(int areaID) {
    Node<ParkingArea*>* current = parkingAreas.getHead();
    while (current != nullptr) {
        if (current->data != nullptr && current->data->getAreaID() == areaID) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

void Zone::addAdjacentZone(Zone* zone) {
    if (zone != nullptr) {
        adjacentZones.insertBack(zone);
    }
}

void Zone::removeAdjacentZone(Zone* zone) {
    if (zone != nullptr) {
        Node<Zone*>* current = adjacentZones.getHead();
        while (current != nullptr) {
            if (current->data == zone) {
                adjacentZones.removeNode(current);
                return;
            }
            current = current->next;
        }
    }
}

DoublyLinkedList<Zone*>& Zone::getAdjacentZones() {
    return adjacentZones;
}

ParkingSlot* Zone::findAvailableSlot() {
    Node<ParkingArea*>* current = parkingAreas.getHead();
    while (current != nullptr) {
        if (current->data != nullptr) {
            ParkingSlot* slot = current->data->findAvailableSlot();
            if (slot != nullptr) {
                return slot;
            }
        }
        current = current->next;
    }
    return nullptr;
}

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
        if (current->data != nullptr) {
            available += current->data->getAvailableSlots();
        }
        current = current->next;
    }
    return available;
}

void Zone::displayInfo() const {
    std::cout << "Zone ID: " << zoneID << ", Total Capacity: " << totalCapacity 
              << ", Available: " << getAvailableSlots() << std::endl;
}