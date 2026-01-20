#include "RollbackManager.h"

void RollbackManager::logAllocation(int reqID, int slotID) {
    // Store the state BEFORE the undo (which is Allocated)
    historyStack.push_back({reqID, slotID, REQUESTED});
}

bool RollbackManager::hasOperations() const {
    return !historyStack.empty();
}

Operation RollbackManager::popLastOperation() {
    if (!historyStack.empty()) {
        Operation op = historyStack.back();
        historyStack.pop_back();
        return op;
    }
    return {-1, -1, REQUESTED};
}