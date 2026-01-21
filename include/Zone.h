#ifndef ZONE_H
#define ZONE_H

#include "LinkedList.h"

// Forward declarations
class ParkingArea;
class ParkingSlot;

// ============================================================================
// ZONE CLASS
// ============================================================================
class Zone {
private:
    int zoneID;
    DoublyLinkedList<ParkingArea*> parkingAreas;
    DoublyLinkedList<Zone*> adjacentZones;
    int totalCapacity;

public:
    // Constructor
    Zone(int id);
    
    // Destructor
    ~Zone();
    
    // ========================================================================
    // ZONE MANAGEMENT
    // ========================================================================
    void addParkingArea(ParkingArea* area);
    ParkingArea* findParkingAreaByID(int areaID);
    
    // ========================================================================
    // ADJACENCY/NEIGHBOR MANAGEMENT
    // ========================================================================
    void addAdjacentZone(Zone* zone);
    void removeAdjacentZone(Zone* zone);
    DoublyLinkedList<Zone*>& getAdjacentZones();
    
    // ========================================================================
    // SLOT SEARCH
    // ========================================================================
    ParkingSlot* findAvailableSlot();
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    int getZoneID() const;
    int getTotalCapacity() const;
    int getAvailableSlots() const;
    DoublyLinkedList<ParkingArea*>& getParkingAreas();
    
    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    void displayInfo() const;
    void refreshCapacity();
};

#endif // ZONE_H
