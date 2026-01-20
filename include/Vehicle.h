#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

// ============================================================================
// VEHICLE CLASS
// ============================================================================
class Vehicle {
private:
    std::string vehicleID;
    int preferredZoneID;

public:
    // Constructor
    Vehicle(const std::string& id, int zoneID);
    
    // Destructor
    ~Vehicle();
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    std::string getVehicleID() const;
    int getPreferredZoneID() const;
    
    // ========================================================================
    // SETTERS
    // ========================================================================
    void setPreferredZoneID(int zoneID);
    
    // ========================================================================
    // UTILITY METHODS
    // ========================================================================
    void displayInfo() const;
};

#endif // VEHICLE_H
