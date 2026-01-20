#include "include/ParkingRequest.h"
#include <iostream>
#include <cstring>

// ============================================================================
// PARKING REQUEST IMPLEMENTATION
// ============================================================================

ParkingRequest::ParkingRequest(const std::string& vID, int zoneID)
    : vehicleID(vID), requestedZoneID(zoneID), requestTime(DateTime()),
      currentStatus(RequestState::REQUESTED), penaltyCost(0.0) {
}

ParkingRequest::~ParkingRequest() {
}

// ========================================================================
// STATE MACHINE - VALID TRANSITIONS
// ========================================================================

bool ParkingRequest::isValidTransition(RequestState from, RequestState to) {
    // Define valid state transitions
    // REQUESTED -> ALLOCATED, CANCELLED
    // ALLOCATED -> OCCUPIED, CANCELLED
    // OCCUPIED -> RELEASED
    // RELEASED -> (terminal state, no further transitions)
    // CANCELLED -> (terminal state, no further transitions)
    
    if (from == to) return false;  // Same state
    
    switch (from) {
        case RequestState::REQUESTED:
            return (to == RequestState::ALLOCATED || to == RequestState::CANCELLED);
        
        case RequestState::ALLOCATED:
            return (to == RequestState::OCCUPIED || to == RequestState::CANCELLED);
        
        case RequestState::OCCUPIED:
            return (to == RequestState::RELEASED);
        
        case RequestState::RELEASED:
            return false;  // Terminal state
        
        case RequestState::CANCELLED:
            return false;  // Terminal state
        
        default:
            return false;
    }
}

bool ParkingRequest::updateState(RequestState newState) {
    if (!isValidTransition(currentStatus, newState)) {
        std::cout << "Invalid state transition from " 
                  << statusToString(currentStatus) << " to " 
                  << statusToString(newState) << std::endl;
        return false;
    }
    
    currentStatus = newState;
    return true;
}

// ========================================================================
// GETTERS
// ========================================================================

std::string ParkingRequest::getVehicleID() const {
    return vehicleID;
}

int ParkingRequest::getRequestedZoneID() const {
    return requestedZoneID;
}

DateTime ParkingRequest::getRequestTime() const {
    return requestTime;
}

RequestState ParkingRequest::getCurrentStatus() const {
    return currentStatus;
}

double ParkingRequest::getPenaltyCost() const {
    return penaltyCost;
}

// ========================================================================
// SETTERS
// ========================================================================

void ParkingRequest::setPenaltyCost(double cost) {
    penaltyCost = cost;
}

void ParkingRequest::addPenaltyCost(double cost) {
    penaltyCost += cost;
}

// ========================================================================
// UTILITY METHODS
// ========================================================================

std::string ParkingRequest::statusToString(RequestState state) const {
    switch (state) {
        case RequestState::REQUESTED:
            return "REQUESTED";
        case RequestState::ALLOCATED:
            return "ALLOCATED";
        case RequestState::OCCUPIED:
            return "OCCUPIED";
        case RequestState::RELEASED:
            return "RELEASED";
        case RequestState::CANCELLED:
            return "CANCELLED";
        default:
            return "UNKNOWN";
    }
}

void ParkingRequest::displayInfo() const {
    std::cout << "Vehicle ID: " << vehicleID 
              << ", Zone ID: " << requestedZoneID 
              << ", Status: " << statusToString(currentStatus)
              << ", Penalty: " << penaltyCost << std::endl;
}
