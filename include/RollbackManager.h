#ifndef ROLLBACKMANAGER_H
#define ROLLBACKMANAGER_H

#include "Stack.h"
#include "Common.h"
#include "ParkingRequest.h"
#include "ParkingSlot.h"

// ============================================================================
// ROLLBACK MANAGER CLASS
// ============================================================================
class RollbackManager {
private:
    Stack<Command> commandHistory;
    int totalRollbacksPerformed;

public:
    // Constructor
    RollbackManager();
    
    // Destructor
    ~RollbackManager();
    
    // ========================================================================
    // COMMAND RECORDING
    // ========================================================================
    
    /**
     * Record a command for potential rollback
     * 
     * @param command - The Command struct containing request, slot, and state info
     */
    void recordCommand(const Command& command);
    
    // ========================================================================
    // ROLLBACK OPERATIONS
    // ========================================================================
    
    /**
     * Perform rollback of last k operations
     * 
     * Algorithm:
     * 1. Pop k items from the command history stack
     * 2. For each popped item:
     *    - Revert ParkingSlot availability (free the slot)
     *    - Reset ParkingRequest state to oldState
     * 3. Track rollback count
     * 
     * @param k - Number of operations to rollback
     * @return bool - Returns true if successful, false if stack has fewer than k items
     */
    bool performRollback(int k);
    
    // ========================================================================
    // HISTORY MANAGEMENT
    // ========================================================================
    int getHistorySize() const;
    int getTotalRollbacksPerformed() const;
    bool hasHistory() const;
    void clearHistory();
    
    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    void displayHistory() const;
    void displayLastCommand() const;
};

#endif // ROLLBACKMANAGER_H
