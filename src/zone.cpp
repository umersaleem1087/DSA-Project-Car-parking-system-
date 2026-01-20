#include "Zone.h"

Zone::Zone(int id, int numAreas, int slotsPerArea) : zoneID(id) {
    for (int i = 0; i < numAreas; i++) {
        // ID Scheme: Zone 1 -> Areas 10, 11, 12
        areas.push_back(ParkingArea(id * 10 + i, id, slotsPerArea));
    }
}

int Zone::getZoneID() const { return zoneID; }

ParkingSlot* Zone::findFirstAvailableSlot() {
    for (auto& area : areas) {
        ParkingSlot* slot = area.findFreeSlot();
        if (slot) return slot;
    }
    return nullptr;
}

ParkingSlot* Zone::getSlot(int slotID) {
    for (auto& area : areas) {
        ParkingSlot* slot = area.getSlotById(slotID);
        if (slot) return slot;
    }
    return nullptr;
}