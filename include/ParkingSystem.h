#ifndef PARKINGSYSTEM_H
#define PARKINGSYSTEM_H

#include <vector>
#include "zone.h"
#include "AllocationEngine.h"
#include "RollbackManager.h"

class ParkingSystem {
private:
    std::vector<Zone> zones;
    AllocationEngine engine;
    RollbackManager rollbackManager;
    bool verbose;

public:
    ParkingSystem();
    void addZone(Zone z);
    void processRequest(ParkingRequest& req);
    void setVerbose(bool v);
};
#endif