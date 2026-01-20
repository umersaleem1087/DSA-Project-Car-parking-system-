#ifndef PARKINGSYSTEM_H
#define PARKINGSYSTEM_H

#include "LinkedList.h"
#include "Zone.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "AllocationEngine.h"
#include "RollbackManager.h"

// ============================================================================
// PARKING SYSTEM CLASS
// ============================================================================
class ParkingSystem {
private:
    AllocationEngine* engine;
    RollbackManager* rollbackManager;
    DoublyLinkedList<ParkingRequest*> masterHistoryList;  // All requests ever made
    
public:
    // Constructor
    ParkingSystem();
    
    // Destructor
    ~ParkingSystem();
    
    // ========================================================================
    // SYSTEM INITIALIZATION
    // ========================================================================
    void addZone(Zone* zone);
    
    // ========================================================================
    // REQUEST PROCESSING
    // ========================================================================
    ParkingSlot* requestParking(Vehicle* vehicle);
    bool occupySlot(ParkingRequest* request, ParkingSlot* slot);
    bool releaseSlot(ParkingRequest* request);
    bool cancelRequest(ParkingRequest* request);
    
    // ========================================================================
    // ROLLBACK OPERATIONS
    // ========================================================================
    bool rollbackLastOperations(int k);
    
    // ========================================================================
    // ANALYTICS - MODULE F
    // ========================================================================
    
    /**
     * Calculate average parking duration across all completed requests
     * Only considers RELEASED and OCCUPIED requests
     * 
     * @return double - Average duration in minutes
     */
    double calculateAverageDuration() const;
    
    /**
     * Get utilization rate for a specific zone
     * Utilization = (Occupied Slots / Total Slots) * 100
     * 
     * @param zoneID - The zone to analyze
     * @return double - Utilization percentage (0-100)
     */
    double getZoneUtilizationRate(int zoneID) const;
    
    /**
     * Display analytics summary for all zones
     */
    void displayZoneAnalytics() const;
    
    /**
     * Get total number of requests processed
     */
    int getTotalRequests() const;
    
    /**
     * Get count of requests by status
     */
    int getRequestCountByStatus(RequestState status) const;
    
    /**
     * Display master history list
     */
    void displayMasterHistory() const;
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    AllocationEngine* getEngine() const;
    RollbackManager* getRollbackManager() const;
    DoublyLinkedList<ParkingRequest*>& getMasterHistory();
    int getMasterHistorySize() const;
};

#endif // PARKINGSYSTEM_H
