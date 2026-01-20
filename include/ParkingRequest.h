#ifndef PARKING_REQUEST_H
#define PARKING_REQUEST_H

#include "Vehicle.h"
#include <string>

enum RequestState { REQUESTED, ALLOCATED, OCCUPIED, RELEASED, CANCELLED };

class ParkingRequest {
private:
    int requestID;
    Vehicle vehicle;
    RequestState state;
    int allocatedSlotID;

public:
    ParkingRequest(int rID, Vehicle v);
    int getID() const;
    Vehicle getVehicle() const;
    RequestState getState() const;
    int getAllocatedSlotID() const;

    void setAllocatedSlot(int slotID);
    bool transitionTo(RequestState newState);
    std::string getStateString() const;
};
#endif