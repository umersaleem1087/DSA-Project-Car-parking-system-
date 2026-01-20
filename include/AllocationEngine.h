#ifndef ALLOCATIONENGINE_H
#define ALLOCATIONENGINE_H

#include "Zone.h"
#include "ParkingRequest.h"
#include "RollbackManager.h"
#include <vector>

class AllocationEngine {
public:
    // Now accepts RollbackManager to log actions
    bool allocateSlot(ParkingRequest& req, std::vector<Zone>& zones, RollbackManager& rollbackMsg);
};
#endif