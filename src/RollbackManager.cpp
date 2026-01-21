#include "RollbackManager.h"
#include <iostream>

RollbackManager::RollbackManager() : totalRollbacksPerformed(0) {}

RollbackManager::~RollbackManager() {}

void RollbackManager::recordCommand(const Command& command) {
    commandHistory.push(command);
}

bool RollbackManager::performRollback(int k) {
    if (commandHistory.getSize() < k) {
        return false;
    }

    for (int i = 0; i < k; i++) {
        if (commandHistory.getSize() > 0) {
            Command cmd = commandHistory.peek();
            // Revert the parking slot
            if (cmd.slotPtr != nullptr) {
                cmd.slotPtr->free();
            }
            // Revert the request state
            if (cmd.requestPtr != nullptr) {
                cmd.requestPtr->updateState(cmd.oldState);
            }
            commandHistory.pop();
            totalRollbacksPerformed++;
        }
    }
    return true;
}

int RollbackManager::getHistorySize() const {
    return commandHistory.getSize();
}

int RollbackManager::getTotalRollbacksPerformed() const {
    return totalRollbacksPerformed;
}

bool RollbackManager::hasHistory() const {
    return !commandHistory.isEmpty();
}

void RollbackManager::clearHistory() {
    while (!commandHistory.isEmpty()) {
        commandHistory.pop();
    }
}

void RollbackManager::displayHistory() const {
    std::cout << "Command History Size: " << commandHistory.getSize() << std::endl;
    std::cout << "Total Rollbacks Performed: " << totalRollbacksPerformed << std::endl;
}

void RollbackManager::displayLastCommand() const {
    if (commandHistory.getSize() > 0) {
        std::cout << "Last command recorded in history." << std::endl;
    } else {
        std::cout << "No command history available." << std::endl;
    }
}
