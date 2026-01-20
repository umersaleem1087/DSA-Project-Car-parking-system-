#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include "LinkedList.h"
#include "ParkingSlot.h"

// ============================================================================
// PARKING AREA CLASS
// ============================================================================
class ParkingArea {
private:
    int areaID;
    DoublyLinkedList<ParkingSlot*> slots;
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
    DoublyLinkedList<ParkingSlot*>& getSlotsList();
    
    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    void displayInfo() const;
    void refreshAvailableCount();
};

#endif // PARKINGAREA_H
