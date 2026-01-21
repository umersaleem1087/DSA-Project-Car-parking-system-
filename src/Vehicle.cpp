#include "Vehicle.h"
#include <iostream>

Vehicle::Vehicle(const std::string& id, int zoneID) : vehicleID(id), preferredZoneID(zoneID) {}

Vehicle::~Vehicle() {}

std::string Vehicle::getVehicleID() const { 
    return vehicleID; 
}

int Vehicle::getPreferredZoneID() const { 
    return preferredZoneID; 
}

void Vehicle::setPreferredZoneID(int zoneID) {
    preferredZoneID = zoneID;
}

void Vehicle::displayInfo() const {
    std::cout << "Vehicle ID: " << vehicleID << ", Preferred Zone: " << preferredZoneID << std::endl;
}