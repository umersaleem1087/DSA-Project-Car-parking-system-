#include "include/ParkingArea.h"
#include <iostream>

// ============================================================================
// PARKING AREA IMPLEMENTATION
// ============================================================================

ParkingArea::ParkingArea(int id)
    : areaID(id), totalSlots(0), availableSlots(0) {
}

ParkingArea::~ParkingArea() {
    // Clean up slots
    while (!slots.isEmpty()) {
        ParkingSlot* slot = slots.getFront();
        slots.removeFront();
        delete slot;
    }
}

// ========================================================================
// SLOT MANAGEMENT
// ========================================================================

void ParkingArea::addSlot(ParkingSlot* slot) {
    if (slot != nullptr) {
        slots.insertBack(slot);
        totalSlots++;
        availableSlots++;
    }
}

ParkingSlot* ParkingArea::findAvailableSlot() {
    Node<ParkingSlot*>* current = slots.getHead();
    while (current != nullptr) {
        if (current->data->getIsAvailable()) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

ParkingSlot* ParkingArea::findSlotByID(int slotID) {
    Node<ParkingSlot*>* current = slots.getHead();
    while (current != nullptr) {
        if (current->data->getSlotID() == slotID) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

// ========================================================================
// GETTERS
// ========================================================================

int ParkingArea::getAreaID() const {
    return areaID;
}

int ParkingArea::getTotalSlots() const {
    return totalSlots;
}

int ParkingArea::getAvailableSlots() const {
    return availableSlots;
}

DoublyLinkedList<ParkingSlot*>& ParkingArea::getSlotsList() {
    return slots;
}

// ========================================================================
// UTILITY METHODS
// ========================================================================

void ParkingArea::displayInfo() const {
    std::cout << "Area ID: " << areaID 
              << ", Total Slots: " << totalSlots 
              << ", Available: " << availableSlots << std::endl;
}

void ParkingArea::refreshAvailableCount() {
    availableSlots = 0;
    Node<ParkingSlot*>* current = slots.getHead();
    while (current != nullptr) {
        if (current->data->getIsAvailable()) {
            availableSlots++;
        }
        current = current->next;
    }
}
