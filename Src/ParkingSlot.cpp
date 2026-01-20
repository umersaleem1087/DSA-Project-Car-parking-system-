#include "include/ParkingSlot.h"
#include <iostream>

// ============================================================================
// PARKING SLOT IMPLEMENTATION
// ============================================================================

ParkingSlot::ParkingSlot(int id, int zone)
    : slotID(id), zoneID(zone), isAvailable(true) {
}

ParkingSlot::~ParkingSlot() {
}

// ========================================================================
// GETTERS
// ========================================================================

int ParkingSlot::getSlotID() const {
    return slotID;
}

int ParkingSlot::getZoneID() const {
    return zoneID;
}

bool ParkingSlot::getIsAvailable() const {
    return isAvailable;
}

// ========================================================================
// SLOT MANAGEMENT
// ========================================================================

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

// ========================================================================
// UTILITY METHODS
// ========================================================================

void ParkingSlot::displayInfo() const {
    std::cout << "Slot ID: " << slotID 
              << ", Zone ID: " << zoneID 
              << ", Available: " << (isAvailable ? "Yes" : "No") << std::endl;
}
