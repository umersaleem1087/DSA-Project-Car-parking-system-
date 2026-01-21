#include "AllocationEngine.h"
#include <iostream>

bool AllocationEngine::allocateSlot(ParkingRequest& req, std::vector<Zone>& zones, RollbackManager& rollbackMsg) {
    int prefZoneID = req.getVehicle().getPreferredZone();

    // Strategy 1: Preferred Zone
    for (auto& zone : zones) {
        if (zone.getZoneID() == prefZoneID) {
            ParkingSlot* slot = zone.findFirstAvailableSlot();
            if (slot) {
                slot->setAvailability(false);
                req.setAllocatedSlot(slot->getSlotID());
                req.transitionTo(ALLOCATED);
                rollbackMsg.logAllocation(req.getID(), slot->getSlotID()); // Log for undo
                return true;
            }
        }
    }

    // Strategy 2: Cross-Zone (Any available)
    for (auto& zone : zones) {
        if (zone.getZoneID() != prefZoneID) {
            ParkingSlot* slot = zone.findFirstAvailableSlot();
            if (slot) {
                slot->setAvailability(false);
                req.setAllocatedSlot(slot->getSlotID());
                req.transitionTo(ALLOCATED);
                rollbackMsg.logAllocation(req.getID(), slot->getSlotID());
                std::cout << "[Info] Cross-zone penalty applied." << std::endl;
                return true;
            }
        }
    }
    return false;
}