#include "ParkingArea.h"
#include "ParkingSlot.h"
#include <iostream>
#include <vector>
#include <cstdint>

ParkingArea::ParkingArea(int id) : areaID(id), slotsPtr(0), totalSlots(0), availableSlots(0) {
    slotsPtr = (intptr_t)(new std::vector<ParkingSlot*>());
}

ParkingArea::~ParkingArea() {
    if (slotsPtr != 0) {
        delete (std::vector<ParkingSlot*>*)(slotsPtr);
        slotsPtr = 0;
    }
}

void ParkingArea::addSlot(ParkingSlot* slot) {
    if (slot != nullptr && slotsPtr != 0) {
        auto* slotVec = (std::vector<ParkingSlot*>*)(slotsPtr);
        slotVec->push_back(slot);
        totalSlots++;
        if (slot->getIsAvailable()) {
            availableSlots++;
        }
    }
}

ParkingSlot* ParkingArea::findAvailableSlot() {
    if (slotsPtr == 0) return nullptr;
    auto* slotVec = (std::vector<ParkingSlot*>*)(slotsPtr);
    for (auto slot : *slotVec) {
        if (slot != nullptr && slot->getIsAvailable()) {
            return slot;
        }
    }
    return nullptr;
}

ParkingSlot* ParkingArea::findSlotByID(int slotID) {
    if (slotsPtr == 0) return nullptr;
    auto* slotVec = (std::vector<ParkingSlot*>*)(slotsPtr);
    for (auto slot : *slotVec) {
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

void ParkingArea::refreshAvailableCount() {
    if (slotsPtr == 0) return;
    auto* slotVec = (std::vector<ParkingSlot*>*)(slotsPtr);
    availableSlots = 0;
    for (auto slot : *slotVec) {
        if (slot != nullptr && slot->getIsAvailable()) {
            availableSlots++;
        }
    }
}

void ParkingArea::displayInfo() const {
    std::cout << "Area ID: " << areaID << ", Total Slots: " << totalSlots 
              << ", Available: " << availableSlots << std::endl;
}