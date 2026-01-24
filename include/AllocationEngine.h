#ifndef ALLOCATIONENGINE_H
#define ALLOCATIONENGINE_H

#include "Vehicle.h"
#include "ParkingSlot.h"
#include "ParkingRequest.h"
#include "Zone.h"
#include "LinkedList.h"

// ============================================================================
// ALLOCATION ENGINE CLASS
// ============================================================================
class AllocationEngine {
private:
    DoublyLinkedList<Zone*> allZones;

public:
    // Constructor
    AllocationEngine();
    
    // Destructor
    ~AllocationEngine();
    
    // ========================================================================
    // ZONE MANAGEMENT
    // ========================================================================
    void addZone(Zone* zone);
    Zone* findZoneByID(int zoneID);
    
    // ========================================================================
    // ALLOCATION ALGORITHM
    // ========================================================================
    
    /**
     * Allocate a parking slot for a vehicle
     * 
     * Algorithm:
     * 1. Search in requestedZone first
     * 2. If found, return the slot
     * 3. If not found, check adjacentZones
     * 4. If found in adjacentZone, mark penalty and return
     * 5. If no slot available anywhere, return nullptr
     * 
     * @param vehicle - The vehicle requesting parking
     * @param parkingRequest - The parking request object to update with penalties
     * @return ParkingSlot* - Pointer to allocated slot, or nullptr if no slot available
     */
    ParkingSlot* allocateSlot(Vehicle* vehicle, ParkingRequest* parkingRequest);
    
    /**
     * Free a parking slot (vehicle departs)
     * 
     * @param slotID - The slot ID to free
     * @return bool - Success or failure
     */
    bool freeSlot(int slotID);
    
    // ========================================================================
    // SEARCH HELPERS
    // ========================================================================
    ParkingSlot* searchInZone(Zone* zone);
    ParkingSlot* searchInAdjacentZones(Zone* primaryZone, ParkingRequest* parkingRequest);
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    DoublyLinkedList<Zone*>& getAllZones();
};

#endif // ALLOCATIONENGINE_H
