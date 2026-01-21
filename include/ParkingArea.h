#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include <vector>

// Forward declarations
class ParkingSlot;

// ============================================================================
// PARKING AREA CLASS
// ============================================================================
class ParkingArea {
private:
    int areaID;
    std::vector<ParkingSlot*> slots;  // Use std::vector instead
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
    std::vector<ParkingSlot*>& getSlotsList();
    
    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    void displayInfo() const;
    void refreshAvailableCount();
};

#endif // PARKINGAREA_H
