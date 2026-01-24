#include "ParkingRequest.h"
#include <iostream>

ParkingRequest::ParkingRequest(const std::string& vID, int zoneID) 
    : vehicleID(vID), requestedZoneID(zoneID), allocatedSlotID(-1), currentStatus(RequestState::REQUESTED), penaltyCost(0.0) {
    // Initialize request time to current time (simplified)
}

ParkingRequest::~ParkingRequest() {}

bool ParkingRequest::isValidTransition(RequestState from, RequestState to) {
    // Normal forward transitions
    if (from == RequestState::REQUESTED && (to == RequestState::ALLOCATED || to == RequestState::CANCELLED)) return true;
    if (from == RequestState::ALLOCATED && (to == RequestState::OCCUPIED || to == RequestState::CANCELLED)) return true;
    if (from == RequestState::OCCUPIED && (to == RequestState::RELEASED || to == RequestState::ALLOCATED)) return true;
    if (from == RequestState::RELEASED && to == RequestState::REQUESTED) return true;
    
    // Rollback transitions (going backwards)
    if (from == RequestState::ALLOCATED && to == RequestState::REQUESTED) return true;
    if (from == RequestState::OCCUPIED && to == RequestState::ALLOCATED) return true;
    if (from == RequestState::RELEASED && to == RequestState::OCCUPIED) return true;
    if (from == RequestState::CANCELLED && to == RequestState::REQUESTED) return true;
    if (from == RequestState::CANCELLED && to == RequestState::ALLOCATED) return true;
    if (from == RequestState::CANCELLED && to == RequestState::OCCUPIED) return true;
    
    return false;
}

bool ParkingRequest::updateState(RequestState newState) {
    if (isValidTransition(currentStatus, newState)) {
        currentStatus = newState;
        return true;
    }
    return false;
}

std::string ParkingRequest::getVehicleID() const { 
    return vehicleID; 
}

int ParkingRequest::getRequestedZoneID() const { 
    return requestedZoneID; 
}

int ParkingRequest::getAllocatedSlotID() const { 
    return allocatedSlotID; 
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

void ParkingRequest::setPenaltyCost(double cost) { 
    penaltyCost = cost; 
}

void ParkingRequest::addPenaltyCost(double cost) { 
    penaltyCost += cost; 
}

void ParkingRequest::setAllocatedSlotID(int slotID) { 
    allocatedSlotID = slotID; 
}

void ParkingRequest::displayInfo() const {
    std::cout << "Vehicle ID: " << vehicleID << ", Zone: " << requestedZoneID 
              << ", Status: " << statusToString(currentStatus) 
              << ", Penalty: " << penaltyCost << std::endl;
}

std::string ParkingRequest::statusToString(RequestState state) const {
    switch(state) {
        case RequestState::REQUESTED: return "REQUESTED";
        case RequestState::ALLOCATED: return "ALLOCATED";
        case RequestState::OCCUPIED: return "OCCUPIED";
        case RequestState::RELEASED: return "RELEASED";
        case RequestState::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}