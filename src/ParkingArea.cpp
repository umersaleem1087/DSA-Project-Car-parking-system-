#include "ParkingArea.h"
#include "ParkingSlot.h"
#include <iostream>

ParkingArea::ParkingArea(int id) : areaID(id), totalSlots(0), availableSlots(0) {}

ParkingArea::~ParkingArea() {}

void ParkingArea::addSlot(ParkingSlot* slot) {
    if (slot != nullptr) {
        slots.push_back(slot);
        totalSlots++;
        if (slot->getIsAvailable()) {
            availableSlots++;
        }
    }
}

ParkingSlot* ParkingArea::findAvailableSlot() {
    for (auto slot : slots) {
        if (slot != nullptr && slot->getIsAvailable()) {
            return slot;
        }
    }
    return nullptr;
}

ParkingSlot* ParkingArea::findSlotByID(int slotID) {
    for (auto slot : slots) {
        if (slot != nullptr && slot->getSlotID() == slotID) {
            return slot;
        }
    }
    return nullptr;
}

int ParkingArea::getAreaID() const { 
    return areaID; 
}

int ParkingArea::getTotalSlots() const { 
    return totalSlots; 
}

int ParkingArea::getAvailableSlots() const { 
    return availableSlots; 
}

std::vector<ParkingSlot*>& ParkingArea::getSlotsList() {
    return slots;
}

void ParkingArea::refreshAvailableCount() {
    availableSlots = 0;
    for (auto slot : slots) {
        if (slot != nullptr && slot->getIsAvailable()) {
            availableSlots++;
        }
    }
}

void ParkingArea::displayInfo() const {
    std::cout << "Area ID: " << areaID << ", Total Slots: " << totalSlots 
              << ", Available: " << availableSlots << std::endl;
}