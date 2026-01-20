#include "include/RollbackManager.h"
#include <iostream>

// ============================================================================
// ROLLBACK MANAGER IMPLEMENTATION
// ============================================================================

RollbackManager::RollbackManager() 
    : totalRollbacksPerformed(0) {
}

RollbackManager::~RollbackManager() {
    clearHistory();
}

// ========================================================================
// COMMAND RECORDING
// ========================================================================

void RollbackManager::recordCommand(const Command& command) {
    commandHistory.push(command);
    std::cout << "Command recorded. History size: " << commandHistory.getSize() << std::endl;
}

// ========================================================================
// ROLLBACK OPERATIONS
// ========================================================================

bool RollbackManager::performRollback(int k) {
    // Check if we have enough commands to rollback
    if (commandHistory.getSize() < k) {
        std::cout << "Cannot rollback " << k << " operations. "
                  << "History contains only " << commandHistory.getSize() 
                  << " commands." << std::endl;
        return false;
    }
    
    std::cout << "Starting rollback of " << k << " operations..." << std::endl;
    
    // Pop k items and revert each
    for (int i = 0; i < k; i++) {
        try {
            Command& cmd = commandHistory.peek();
            
            // Validate pointers
            if (cmd.requestPtr == nullptr || cmd.slotPtr == nullptr) {
                std::cout << "Warning: Invalid pointers in command " << (i + 1) << std::endl;
                commandHistory.pop();
                continue;
            }
            
            std::cout << "\nReverting operation " << (i + 1) << ":" << std::endl;
            std::cout << "  Vehicle: " << cmd.requestPtr->getVehicleID() << std::endl;
            std::cout << "  Slot: " << cmd.slotPtr->getSlotID() << std::endl;
            std::cout << "  Old State: " << cmd.requestPtr->statusToString(cmd.oldState) << std::endl;
            std::cout << "  New State: " << cmd.requestPtr->statusToString(cmd.newState) << std::endl;
            
            // Step 1: Revert ParkingSlot availability
            // Free the slot (mark as available)
            cmd.slotPtr->free();
            std::cout << "  Slot " << cmd.slotPtr->getSlotID() 
                      << " freed (now available)" << std::endl;
            
            // Step 2: Reset ParkingRequest state to oldState
            cmd.requestPtr->updateState(cmd.oldState);
            std::cout << "  Request state reset to: " 
                      << cmd.requestPtr->statusToString(cmd.oldState) << std::endl;
            
            // Pop the command from history
            commandHistory.pop();
            totalRollbacksPerformed++;
            
        } catch (const char* e) {
            std::cout << "Error during rollback operation " << (i + 1) 
                      << ": " << e << std::endl;
            return false;
        }
    }
    
    std::cout << "\nRollback completed successfully. " << k << " operations reverted." << std::endl;
    return true;
}

// ========================================================================
// HISTORY MANAGEMENT
// ========================================================================

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
    commandHistory.clear();
    std::cout << "Command history cleared." << std::endl;
}

// ========================================================================
// UTILITY METHODS
// ========================================================================

void RollbackManager::displayHistory() const {
    if (commandHistory.isEmpty()) {
        std::cout << "Command history is empty." << std::endl;
        return;
    }
    
    std::cout << "=== Command History ===" << std::endl;
    std::cout << "Total commands: " << commandHistory.getSize() << std::endl;
    std::cout << "Total rollbacks performed: " << totalRollbacksPerformed << std::endl;
    std::cout << "========================" << std::endl;
}

void RollbackManager::displayLastCommand() const {
    if (commandHistory.isEmpty()) {
        std::cout << "No commands in history." << std::endl;
        return;
    }
    
    try {
        const Command& lastCmd = commandHistory.peek();
        std::cout << "=== Last Command ===" << std::endl;
        
        if (lastCmd.requestPtr != nullptr) {
            std::cout << "Vehicle: " << lastCmd.requestPtr->getVehicleID() << std::endl;
        }
        
        if (lastCmd.slotPtr != nullptr) {
            std::cout << "Slot: " << lastCmd.slotPtr->getSlotID() << std::endl;
        }
        
        std::cout << "State Transition: " << std::endl;
        // Note: Would need statusToString from a request object to display nicely
        std::cout << "=====================" << std::endl;
        
    } catch (const char* e) {
        std::cout << "Error accessing last command: " << e << std::endl;
    }
}
