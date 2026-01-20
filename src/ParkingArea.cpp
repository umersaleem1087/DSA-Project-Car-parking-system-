#include "ParkingArea.h"

ParkingArea::ParkingArea(int aID, int zID, int capacity) : areaID(aID), zoneID(zID) {
    for (int i = 0; i < capacity; i++) {
        // ID Scheme: Area 10 -> Slots 1000, 1001, etc.
        slots.push_back(ParkingSlot(aID * 100 + i, zID));
    }
}

int ParkingArea::getAreaID() const { return areaID; }

ParkingSlot* ParkingArea::findFreeSlot() {
    for (auto& slot : slots) {
        if (slot.getAvailability()) return &slot;
    }
    return nullptr;
}

ParkingSlot* ParkingArea::getSlotById(int slotID) {
    for (auto& slot : slots) {
        if (slot.getSlotID() == slotID) return &slot;
    }
    return nullptr;
}

int ParkingArea::getAvailableCount() const {
    int count = 0;
    for (const auto& slot : slots) if (slot.getAvailability()) count++;
    return count;
}