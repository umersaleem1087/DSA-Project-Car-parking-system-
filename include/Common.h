#ifndef COMMON_H
#define COMMON_H

#include <ctime>
#include <string>

// Forward declarations for circular dependency prevention
class ParkingRequest;
class ParkingSlot;

// ============================================================================
// REQUEST STATE ENUM (Must be before Command struct)
// ============================================================================
enum class RequestState {
    REQUESTED,   // Request has been made but not yet allocated
    ALLOCATED,   // Parking slot has been allocated
    OCCUPIED,    // Vehicle is currently occupying the slot
    RELEASED,    // Vehicle has left and slot is now free
    CANCELLED    // Request was cancelled
};

// ============================================================================
// COMMAND STRUCT FOR ROLLBACK
// ============================================================================
struct Command {
    ParkingRequest* requestPtr;
    ParkingSlot* slotPtr;
    RequestState oldState;
    RequestState newState;
    
    Command() : requestPtr(nullptr), slotPtr(nullptr), 
                oldState(RequestState::REQUESTED), newState(RequestState::REQUESTED) {}
    
    Command(ParkingRequest* req, ParkingSlot* slot, 
            RequestState old, RequestState newS) 
        : requestPtr(req), slotPtr(slot), oldState(old), newState(newS) {}
};

// ============================================================================
// DATE/TIME STRUCTURES
// ============================================================================
struct DateTime {
    time_t timestamp;  // Unix timestamp for the request/entry/exit time
    
    DateTime() : timestamp(std::time(nullptr)) {}
    DateTime(time_t ts) : timestamp(ts) {}
    
    std::string toString() const {
        return std::string(std::ctime(&timestamp));
    }
};

// ============================================================================
// PENALTY COST CONSTANTS
// ============================================================================
namespace PenaltyCosts {
    // Penalty rates (in currency units per hour or per occurrence)
    const double OVERSTAY_PENALTY_PER_HOUR = 50.0;      // Penalty for exceeding parking duration
    const double LATE_RELEASE_PENALTY_PER_HOUR = 25.0;  // Penalty for not releasing slot on time
    const double CANCELLATION_PENALTY = 100.0;          // Penalty for cancelling a confirmed allocation
    const double INVALID_ALLOCATION_PENALTY = 75.0;     // Penalty for invalid allocation attempt
    
    // Grace periods (in hours)
    const int OVERSTAY_GRACE_PERIOD_HOURS = 1;
    const int RELEASE_GRACE_PERIOD_HOURS = 0.5;  // 30 minutes
}

// ============================================================================
// VEHICLE TYPE CONSTANTS
// ============================================================================
enum class VehicleType {
    SEDAN,
    SUV,
    VAN,
    TRUCK,
    MOTORCYCLE,
    COMPACT
};

// ============================================================================
// PARKING SLOT SIZE ENUM
// ============================================================================
enum class SlotSize {
    COMPACT,
    STANDARD,
    LARGE
};

#endif // COMMON_H
