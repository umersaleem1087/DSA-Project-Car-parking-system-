#include <iostream>
#include "ParkingSystem.h"
#include "Vehicle.h"
#include "ParkingRequest.h"

int main() {
    std::cout << "=== DSA Parking System Starting ===" << std::endl;

    // 1. Initialize System
    ParkingSystem citySystem;

    // 2. Create Infrastructure
    // Zone 1: 2 Areas, 5 Slots per Area
    citySystem.addZone(Zone(1, 2, 5)); 
    // Zone 2: 1 Area, 10 Slots per Area
    citySystem.addZone(Zone(2, 1, 10));

    // 3. Create Traffic
    Vehicle v1(101, 1); // Wants Zone 1
    ParkingRequest req1(1, v1);

    Vehicle v2(202, 1); // Wants Zone 1
    ParkingRequest req2(2, v2);

    // 4. Process
    citySystem.processRequest(req1);
    citySystem.processRequest(req2);

    std::cout << "=== System Simulation Finished ===" << std::endl;
    return 0;
}