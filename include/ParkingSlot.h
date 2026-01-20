#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

// ============================================================================
// PARKING SLOT CLASS
// ============================================================================
class ParkingSlot {
private:
    int slotID;
    int zoneID;
    bool isAvailable;

public:
    // Constructor
    ParkingSlot(int id, int zone);
    
    // Destructor
    ~ParkingSlot();
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    int getSlotID() const;
    int getZoneID() const;
    bool getIsAvailable() const;
    
    // ========================================================================
    // SLOT MANAGEMENT
    // ========================================================================
    bool allocate();
    void free();
    
    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    void displayInfo() const;
};

#endif // PARKINGSLOT_H
