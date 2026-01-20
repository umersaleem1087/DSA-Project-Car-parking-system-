#ifndef PARKINGSYSTEM_H
#define PARKINGSYSTEM_H

#include <iostream>
#include <iomanip>
#include "LinkedList.h"
#include "Zone.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "AllocationEngine.h"
#include "RollbackManager.h"

// ============================================================================
// DASHBOARD STATISTICS STRUCT (For Qt Integration)
// ============================================================================
struct DashboardStats {
    int totalRequests;
    int requestsAllocated;
    int requestsOccupied;
    int requestsReleased;
    int requestsCancelled;
    double averageParkingDuration;
    int totalZones;
    double systemUtilization;
    
    void display() const {
        std::cout << "\n========== DASHBOARD STATISTICS ==========" << std::endl;
        std::cout << "Total Requests: " << totalRequests << std::endl;
        std::cout << "  - Allocated: " << requestsAllocated << std::endl;
        std::cout << "  - Occupied: " << requestsOccupied << std::endl;
        std::cout << "  - Released: " << requestsReleased << std::endl;
        std::cout << "  - Cancelled: " << requestsCancelled << std::endl;
        std::cout << "Average Duration: " << averageParkingDuration << " minutes" << std::endl;
        std::cout << "Total Zones: " << totalZones << std::endl;
        std::cout << "System Utilization: " << systemUtilization << "%" << std::endl;
        std::cout << "========================================\n" << std::endl;
    }
};

// ============================================================================
// PARKING SYSTEM CLASS (Controller Pattern - Qt-Ready)
// ============================================================================
class ParkingSystem {
private:
    AllocationEngine* engine;
    RollbackManager* rollbackManager;
    DoublyLinkedList<ParkingRequest*> masterHistoryList;  // All requests ever made
    DoublyLinkedList<ParkingRequest*> activeRequests;      // Currently active requests
    
    // Helper methods
    ParkingRequest* findRequestByVehicleID(const std::string& vehicleID);
    double calculateAverageDuration() const;
    
public:
    // Constructor
    ParkingSystem();
    
    // Destructor
    ~ParkingSystem();
    
    // ========================================================================
    // PUBLIC API - SYSTEM INITIALIZATION
    // ========================================================================
    void addZone(Zone* zone);
    void displaySystemStatus() const;
    
    // ========================================================================
    // PUBLIC API - REQUEST MANAGEMENT (Qt-Ready)
    // ========================================================================
    
    /**
     * Create a parking request for a vehicle in a preferred zone
     * This is the primary entry point for the UI
     * 
     * @param vehicleID - Unique vehicle identifier
     * @param preferredZoneID - Zone ID where user wants to park
     * @return ParkingRequest* - The created request, or nullptr if failed
     */
    ParkingRequest* createRequest(const std::string& vehicleID, int preferredZoneID);
    
    /**
     * Get active request by vehicle ID
     * 
     * @param vehicleID - The vehicle ID to search
     * @return ParkingRequest* - The request, or nullptr if not found
     */
    ParkingRequest* getRequestByVehicleID(const std::string& vehicleID);
    
    /**
     * Occupy a parking slot (vehicle enters the slot)
     * 
     * @param vehicleID - Vehicle occupying the slot
     * @return bool - Success or failure
     */
    bool occupyRequest(const std::string& vehicleID);
    
    /**
     * Release a parking slot (vehicle departs)
     * 
     * @param vehicleID - Vehicle releasing the slot
     * @return bool - Success or failure
     */
    bool releaseRequest(const std::string& vehicleID);
    
    /**
     * Cancel a parking request
     * 
     * @param vehicleID - Vehicle requesting cancellation
     * @return bool - Success or failure
     */
    bool cancelRequest(const std::string& vehicleID);
    
    // ========================================================================
    // PUBLIC API - ROLLBACK OPERATIONS
    // ========================================================================
    
    /**
     * Rollback the last k operations
     * 
     * @param k - Number of operations to undo
     * @return bool - Success or failure
     */
    bool rollbackOperations(int k);
    
    // ========================================================================
    // PUBLIC API - ANALYTICS & REPORTING (Qt-Ready)
    // ========================================================================
    
    /**
     * Get comprehensive dashboard statistics
     * This is designed for the Qt UI dashboard
     * 
     * @return DashboardStats - Struct containing all analytics
     */
    DashboardStats getDashboardStats() const;
    
    /**
     * Get utilization rate for a specific zone
     * 
     * @param zoneID - The zone to analyze
     * @return double - Utilization percentage (0-100)
     */
    double getZoneUtilization(int zoneID) const;
    
    /**
     * Display comprehensive zone analytics
     */
    void displayZoneAnalytics() const;
    
    /**
     * Display complete master history
     */
    void displayFullHistory() const;
    
    /**
     * Get rollback history status
     */
    void displayRollbackStatus() const;
    
    // ========================================================================
    // INTERNAL MANAGEMENT (Not for Qt UI)
    // ========================================================================
    
    int getTotalRequests() const;
    int getRequestCountByStatus(RequestState status) const;
    int getActiveRequestCount() const;
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    AllocationEngine* getEngine() const;
    RollbackManager* getRollbackManager() const;
    DoublyLinkedList<ParkingRequest*>& getMasterHistory();
    DoublyLinkedList<ParkingRequest*>& getActiveRequests();
};

#endif // PARKINGSYSTEM_H
