#include "ParkingRequest.h"

ParkingRequest::ParkingRequest(int rID, Vehicle v) 
    : requestID(rID), vehicle(v), state(REQUESTED), allocatedSlotID(-1) {}

int ParkingRequest::getID() const { return requestID; }
Vehicle ParkingRequest::getVehicle() const { return vehicle; }
RequestState ParkingRequest::getState() const { return state; }
int ParkingRequest::getAllocatedSlotID() const { return allocatedSlotID; }
void ParkingRequest::setAllocatedSlot(int slotID) { allocatedSlotID = slotID; }

bool ParkingRequest::transitionTo(RequestState newState) {
    // Strict lifecycle enforcement [cite: 44-50]
    bool valid = false;
    if (state == REQUESTED && (newState == ALLOCATED || newState == CANCELLED)) valid = true;
    else if (state == ALLOCATED && (newState == OCCUPIED || newState == CANCELLED)) valid = true;
    else if (state == OCCUPIED && newState == RELEASED) valid = true;

    if (valid) {
        state = newState;
        return true;
    }
    return false;
}

std::string ParkingRequest::getStateString() const {
    switch(state) {
        case REQUESTED: return "REQUESTED";
        case ALLOCATED: return "ALLOCATED";
        case OCCUPIED: return "OCCUPIED";
        case RELEASED: return "RELEASED";
        case CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}