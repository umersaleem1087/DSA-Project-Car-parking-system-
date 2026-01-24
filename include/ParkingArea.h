#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include <cstdint>

// Forward declarations only - no includes to avoid MOC issues
class ParkingSlot;

// ============================================================================
// PARKING AREA CLASS
// ============================================================================
class ParkingArea {
private:
    int areaID;
    intptr_t slotsPtr;  // Opaque pointer to std::vector<ParkingSlot*>
    int totalSlots;
    int availableSlots;

public:
    // Constructor
    ParkingArea(int id);
    
    // Destructor
    ~ParkingArea();
    
    // ========================================================================
    // SLOT MANAGEMENT
    // ========================================================================
    void addSlot(ParkingSlot* slot);
    ParkingSlot* findAvailableSlot();
    ParkingSlot* findSlotByID(int slotID);
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    int getAreaID() const;
    int getTotalSlots() const;
    int getAvailableSlots() const;
    
    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    void displayInfo() const;
    void refreshAvailableCount();
};

#endif // PARKINGAREA_H
