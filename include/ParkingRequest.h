#ifndef PARKINGREQUEST_H
#define PARKINGREQUEST_H

#include <string>
#include "Common.h"

// ============================================================================
// PARKING REQUEST CLASS
// ============================================================================
class ParkingRequest {
private:
    std::string vehicleID;
    int requestedZoneID;
    int allocatedSlotID;
    DateTime requestTime;
    RequestState currentStatus;
    double penaltyCost;
    
    // Valid state transitions map
    bool isValidTransition(RequestState from, RequestState to);

public:
    // Constructor
    ParkingRequest(const std::string& vID, int zoneID);
    
    // Destructor
    ~ParkingRequest();
    
    // ========================================================================
    // STATE MACHINE
    // ========================================================================
    bool updateState(RequestState newState);
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    std::string getVehicleID() const;
    int getRequestedZoneID() const;
    int getAllocatedSlotID() const;
    DateTime getRequestTime() const;
    RequestState getCurrentStatus() const;
    double getPenaltyCost() const;
    
    // ========================================================================
    // SETTERS
    // ========================================================================
    void setPenaltyCost(double cost);
    void addPenaltyCost(double cost);
    void setAllocatedSlotID(int slotID);
    
    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    void displayInfo() const;
    std::string statusToString(RequestState state) const;
};

#endif // PARKINGREQUEST_H
