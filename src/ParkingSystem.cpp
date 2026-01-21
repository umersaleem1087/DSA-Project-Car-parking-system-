#include "ParkingSystem.h"
#include <iostream>

ParkingSystem::ParkingSystem() : verbose(true) {}

void ParkingSystem::addZone(Zone z) {
    zones.push_back(z);
}

void ParkingSystem::setVerbose(bool v) {
    verbose = v;
}

void ParkingSystem::processRequest(ParkingRequest& req) {
    if (verbose) {
        std::cout << "Processing Request " << req.getID() << "..." << std::endl;
    }
    if (engine.allocateSlot(req, zones, rollbackManager)) {
        if (verbose) {
            std::cout << "Request Approved. Slot: " << req.getAllocatedSlotID() << std::endl;
        }
    } else {
        if (verbose) {
            std::cout << "Request Denied. No slots available." << std::endl;
        }
    }
}