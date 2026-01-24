#include "RollbackManager.h"
#include "Zone.h"
#include <iostream>

RollbackManager::RollbackManager() : totalRollbacksPerformed(0) {}

RollbackManager::~RollbackManager() {}

void RollbackManager::recordCommand(const Command& command) {
    commandHistory.push(command);
}

bool RollbackManager::performRollback(int k) {
    if (commandHistory.getSize() < k) {
        std::cerr << "❌ Not enough operations to rollback. History size: " 
                 << commandHistory.getSize() << ", Requested: " << k << "\n";
        return false;
    }

    std::cout << "\n🔄 STARTING ROLLBACK OF " << k << " OPERATION(S)\n";
    std::cout << "================================================\n";
    
    for (int i = 0; i < k; i++) {
        if (commandHistory.getSize() > 0) {
            Command cmd = commandHistory.peek();
            
            // Revert the request state to its old state
            if (cmd.requestPtr != nullptr) {
                std::string vehicleID = cmd.requestPtr->getVehicleID();
                std::string oldStateStr = cmd.requestPtr->statusToString(cmd.oldState);
                std::string newStateStr = cmd.requestPtr->statusToString(cmd.newState);
                
                // Check if this is a creation command (oldState == newState == REQUESTED)
                // This means rolling back a creation, so we mark vehicle for removal
                if (cmd.oldState == RequestState::REQUESTED && 
                    cmd.newState == RequestState::REQUESTED) {
                    // This is a creation operation - mark it for removal
                    cmd.requestPtr->updateState(RequestState::CANCELLED);  // Mark as removed
                    
                    // Free the slot if one was allocated
                    if (cmd.slotPtr != nullptr) {
                        cmd.slotPtr->free();
                        std::cout << "  ✓ Slot " << cmd.slotPtr->getSlotID() 
                                 << " freed\n";
                    }
                    
                    // Refresh zone to update available slot counts
                    if (cmd.zonePtr != nullptr) {
                        cmd.zonePtr->refreshCapacity();
                    }
                    
                    std::cout << "  ✓ Vehicle " << vehicleID 
                             << " creation rolled back - REMOVED from system\n";
                } else {
                    // Regular state reversion
                    // If rolling back from ALLOCATED or OCCUPIED, we need to free the slot
                    if ((cmd.newState == RequestState::ALLOCATED || 
                         cmd.newState == RequestState::OCCUPIED) && 
                        cmd.slotPtr != nullptr) {
                        // Free the slot since the request is being rolled back
                        cmd.slotPtr->free();
                        std::cout << "  ✓ Slot " << cmd.slotPtr->getSlotID() 
                                 << " freed\n";
                    }
                    
                    // Refresh zone to update available slot counts
                    if (cmd.zonePtr != nullptr && cmd.slotPtr != nullptr) {
                        cmd.zonePtr->refreshCapacity();
                    }
                    
                    // Update request to its previous state
                    bool stateUpdated = cmd.requestPtr->updateState(cmd.oldState);
                    if (stateUpdated) {
                        std::cout << "  ✓ Vehicle " << vehicleID << " reverted: " 
                                 << newStateStr << " → " << oldStateStr << "\n";
                    } else {
                        std::cerr << "  ❌ FAILED to update Vehicle " << vehicleID 
                                 << " state from " << newStateStr << " to " << oldStateStr << "\n";
                    }
                }
            }
            
            commandHistory.pop();
            totalRollbacksPerformed++;
        }
    }
    
    std::cout << "================================================\n";
    std::cout << "✓ ROLLBACK COMPLETED - Total rollbacks: " << totalRollbacksPerformed << "\n\n";
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
