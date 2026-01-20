#ifndef ROLLBACKMANAGER_H
#define ROLLBACKMANAGER_H

#include <vector>
#include "ParkingRequest.h"

// Simple struct to store what happened
struct Operation {
    int requestID;
    int slotID;
    RequestState prevState;
};

class RollbackManager {
private:
    std::vector<Operation> historyStack; // Acts as a Stack

public:
    void logAllocation(int reqID, int slotID);
    // Returns the last operation to undo, or dummy if empty
    bool hasOperations() const;
    Operation popLastOperation();
};
#endif