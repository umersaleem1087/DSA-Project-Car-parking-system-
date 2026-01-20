#include "include/Vehicle.h"
#include <iostream>

// ============================================================================
// VEHICLE IMPLEMENTATION
// ============================================================================

Vehicle::Vehicle(const std::string& id, int zoneID)
    : vehicleID(id), preferredZoneID(zoneID) {
}

Vehicle::~Vehicle() {
}

// ========================================================================
// GETTERS
// ========================================================================

std::string Vehicle::getVehicleID() const {
    return vehicleID;
}

int Vehicle::getPreferredZoneID() const {
    return preferredZoneID;
}

// ========================================================================
// SETTERS
// ========================================================================

void Vehicle::setPreferredZoneID(int zoneID) {
    preferredZoneID = zoneID;
}

// ========================================================================
// UTILITY METHODS
// ========================================================================

void Vehicle::displayInfo() const {
    std::cout << "Vehicle ID: " << vehicleID 
              << ", Preferred Zone: " << preferredZoneID << std::endl;
}
