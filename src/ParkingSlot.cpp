#include "ParkingSlot.h"
#include <iostream>

ParkingSlot::ParkingSlot(int id, int zone) : slotID(id), zoneID(zone), isAvailable(true) {}

ParkingSlot::~ParkingSlot() {}

int ParkingSlot::getSlotID() const { 
    return slotID; 
}

int ParkingSlot::getZoneID() const { 
    return zoneID; 
}

bool ParkingSlot::getIsAvailable() const { 
    return isAvailable; 
}

bool ParkingSlot::allocate() { 
    if (isAvailable) {
        isAvailable = false;
        return true;
    }
    return false;
}

void ParkingSlot::free() { 
    isAvailable = true; 
}

void ParkingSlot::displayInfo() const {
    std::cout << "Slot ID: " << slotID << ", Zone: " << zoneID 
              << ", Available: " << (isAvailable ? "Yes" : "No") << std::endl;
}