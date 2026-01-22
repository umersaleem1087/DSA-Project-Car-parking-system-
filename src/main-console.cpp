#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include "ParkingSystem.h"
#include "Vehicle.h"

// ============================================================================
// SIMPLE CONSOLE-BASED UI FOR PARKING SYSTEM (No Qt needed)
// ============================================================================

class ParkingSystemUI {
private:
    ParkingSystem* parkingSystem;
    
public:
    ParkingSystemUI() {
        parkingSystem = new ParkingSystem();
        initializeSystem();
    }
    
    ~ParkingSystemUI() {
        delete parkingSystem;
    }
    
    void initializeSystem() {
        std::cout << "\n╔════════════════════════════════════════════════════╗\n";
        std::cout << "║   DSA PARKING SYSTEM - CONSOLE UI (No Qt Version)   ║\n";
        std::cout << "║              Version 1.0 (Demo Build)               ║\n";
        std::cout << "╚════════════════════════════════════════════════════╝\n\n";
        
        std::cout << "📋 Initializing System...\n";
        
        // Add zones
        parkingSystem->addZone(new Zone(1));  // Zone 1
        parkingSystem->addZone(new Zone(2)); // Zone 2
        parkingSystem->addZone(new Zone(3));  // Zone 3
        
        std::cout << "✅ System initialized successfully!\n";
        std::cout << "✅ Zones configured: 3 zones with parking slots\n\n";
    }
    
    void displayMainMenu() {
        std::cout << "\n╔════════════════════════════════════════════════════╗\n";
        std::cout << "║              PARKING SYSTEM MENU                    ║\n";
        std::cout << "╠════════════════════════════════════════════════════╣\n";
        std::cout << "║ 1. Create Parking Request                          ║\n";
        std::cout << "║ 2. Occupy Parking Slot                             ║\n";
        std::cout << "║ 3. Release Parking Slot                            ║\n";
        std::cout << "║ 4. Cancel Parking Request                          ║\n";
        std::cout << "║ 5. View Dashboard Statistics                       ║\n";
        std::cout << "║ 6. View Zone Analytics                             ║\n";
        std::cout << "║ 7. View Full History                               ║\n";
        std::cout << "║ 8. Rollback Operations                             ║\n";
        std::cout << "║ 9. Exit                                            ║\n";
        std::cout << "╚════════════════════════════════════════════════════╝\n";
        std::cout << "Enter your choice (1-9): ";
    }
    
    void createRequest() {
        std::cout << "\n🅿️  CREATE PARKING REQUEST\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        
        std::string vehicleID;
        int zoneID;
        
        std::cout << "Enter Vehicle ID (e.g., V001, CAR123): ";
        std::getline(std::cin, vehicleID);
        
        if (vehicleID.empty()) {
            std::cout << "❌ Invalid input! Vehicle ID cannot be empty.\n";
            return;
        }
        
        std::cout << "Select Zone (1, 2, or 3): ";
        std::cin >> zoneID;
        std::cin.ignore();
        
        if (zoneID < 1 || zoneID > 3) {
            std::cout << "❌ Invalid zone! Please select 1, 2, or 3.\n";
            return;
        }
        
        ParkingRequest* request = parkingSystem->createRequest(vehicleID, zoneID);
        
        if (request) {
            std::cout << "✅ Request created successfully!\n";
            std::cout << "   Vehicle ID: " << vehicleID << "\n";
            std::cout << "   Zone ID: " << zoneID << "\n";
        } else {
            std::cout << "❌ Failed to create request! Zone may be full.\n";
        }
    }
    
    void occupyParking() {
        std::cout << "\n🚗 OCCUPY PARKING SLOT\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        
        std::string vehicleID;
        std::cout << "Enter Vehicle ID: ";
        std::getline(std::cin, vehicleID);
        
        if (vehicleID.empty()) {
            std::cout << "❌ Invalid input! Vehicle ID cannot be empty.\n";
            return;
        }
        
        ParkingRequest* request = parkingSystem->getRequestByVehicleID(vehicleID);
        if (!request) {
            std::cout << "❌ Vehicle " << vehicleID << " has no active parking request!\n";
            return;
        }
        
        if (request->getCurrentStatus() != RequestState::ALLOCATED) {
            std::cout << "❌ Vehicle " << vehicleID << " request is not in ALLOCATED state!\n";
            std::cout << "   Current status: " << request->statusToString(request->getCurrentStatus()) << "\n";
            return;
        }
        
        bool success = parkingSystem->occupyRequest(vehicleID);
        
        if (success) {
            std::cout << "✅ Vehicle " << vehicleID << " occupied parking slot!\n";
        } else {
            std::cout << "❌ Failed to occupy parking slot for " << vehicleID << "\n";
        }
    }
    
    void releaseParking() {
        std::cout << "\n🚪 RELEASE PARKING SLOT\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        
        std::string vehicleID;
        std::cout << "Enter Vehicle ID: ";
        std::getline(std::cin, vehicleID);
        
        if (vehicleID.empty()) {
            std::cout << "❌ Invalid input! Vehicle ID cannot be empty.\n";
            return;
        }
        
        ParkingRequest* request = parkingSystem->getRequestByVehicleID(vehicleID);
        if (!request) {
            std::cout << "❌ Vehicle " << vehicleID << " not found in system!\n";
            return;
        }
        
        bool success = parkingSystem->releaseRequest(vehicleID);
        
        if (success) {
            std::cout << "✅ Vehicle " << vehicleID << " released parking slot!\n";
        } else {
            std::cout << "❌ Failed to release parking slot for " << vehicleID << "\n";
        }
    }
    
    void cancelRequest() {
        std::cout << "\n❌ CANCEL PARKING REQUEST\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        
        std::string vehicleID;
        std::cout << "Enter Vehicle ID: ";
        std::getline(std::cin, vehicleID);
        
        bool success = parkingSystem->cancelRequest(vehicleID);
        
        if (success) {
            std::cout << "✅ Request cancelled for vehicle " << vehicleID << "\n";
        } else {
            std::cout << "❌ Failed to cancel request for " << vehicleID << "\n";
        }
    }
    
    void displayDashboard() {
        std::cout << "\n📊 DASHBOARD - SYSTEM STATISTICS\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        
        DashboardStats stats = parkingSystem->getDashboardStats();
        
        std::cout << std::left << std::setw(30) << "Total Requests:" 
                  << stats.totalRequests << "\n";
        std::cout << std::left << std::setw(30) << "Requests Allocated:" 
                  << stats.requestsAllocated << "\n";
        std::cout << std::left << std::setw(30) << "Requests Occupied:" 
                  << stats.requestsOccupied << "\n";
        std::cout << std::left << std::setw(30) << "Requests Released:" 
                  << stats.requestsReleased << "\n";
        std::cout << std::left << std::setw(30) << "Requests Cancelled:" 
                  << stats.requestsCancelled << "\n";
        std::cout << std::left << std::setw(30) << "Average Duration (min):" 
                  << std::fixed << std::setprecision(2) << stats.averageParkingDuration << "\n";
        std::cout << std::left << std::setw(30) << "Total Zones:" 
                  << stats.totalZones << "\n";
        std::cout << std::left << std::setw(30) << "System Utilization:" 
                  << std::fixed << std::setprecision(2) << stats.systemUtilization << "%\n";
        
        // Progress bar visualization
        int utilPercent = static_cast<int>(stats.systemUtilization);
        std::cout << "\nUtilization Bar: [";
        for (int i = 0; i < 50; i++) {
            if (i < utilPercent / 2) {
                std::cout << "█";
            } else {
                std::cout << "░";
            }
        }
        std::cout << "] " << utilPercent << "%\n";
        
        // Display available slots per zone
        std::cout << "\n🅿️  AVAILABLE SLOTS BY ZONE/AREA:\n";
        std::cout << "──────────────────────────────────────────────────\n";
        std::cout << std::left << std::setw(8) << "Zone ID"
                  << std::setw(15) << "Total Slots"
                  << std::setw(15) << "Available"
                  << std::setw(15) << "Occupied"
                  << "Utilization %\n";
        std::cout << "──────────────────────────────────────────────────\n";
        
        auto zoneNode = stats.zoneStatuses.getHead();
        while (zoneNode != nullptr) {
            ZoneSlotStatus zoneStatus = zoneNode->data;
            std::cout << std::left << std::setw(8) << zoneStatus.zoneID
                      << std::setw(15) << zoneStatus.totalSlots
                      << std::setw(15) << zoneStatus.availableSlots
                      << std::setw(15) << zoneStatus.occupiedSlots
                      << std::fixed << std::setprecision(1) << zoneStatus.utilization << "%\n";
            zoneNode = zoneNode->next;
        }
        std::cout << "──────────────────────────────────────────────────\n";
    }
    
    void displayZoneAnalytics() {
        std::cout << "\n📈 ZONE ANALYTICS\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        
        std::cout << std::left << std::setw(10) << "Zone"
                  << std::setw(20) << "Utilization %"
                  << "\n";
        std::cout << "──────────────────────────────────────────────────\n";
        
        for (int zoneID = 1; zoneID <= 3; ++zoneID) {
            double utilization = parkingSystem->getZoneUtilization(zoneID);
            std::cout << std::left << std::setw(10) << zoneID
                      << std::fixed << std::setprecision(2) << utilization << "%\n";
        }
    }
    
    void displayFullHistory() {
        std::cout << "\n📋 FULL REQUEST HISTORY\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        
        parkingSystem->displayFullHistory();
    }
    
    void performRollback() {
        std::cout << "\n↩️  ROLLBACK OPERATIONS\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        
        int k;
        std::cout << "Enter number of operations to rollback (1-100): ";
        std::cin >> k;
        std::cin.ignore();
        
        if (k < 1 || k > 100) {
            std::cout << "❌ Invalid input! Please enter between 1 and 100.\n";
            return;
        }
        
        bool success = parkingSystem->rollbackOperations(k);
        
        if (success) {
            std::cout << "✅ Successfully rolled back " << k << " operation(s)!\n";
        } else {
            std::cout << "❌ Failed to rollback operations.\n";
        }
    }
    
    void run() {
        int choice;
        
        while (true) {
            displayMainMenu();
            std::cin >> choice;
            std::cin.ignore();
            
            switch (choice) {
                case 1:
                    createRequest();
                    break;
                case 2:
                    occupyParking();
                    break;
                case 3:
                    releaseParking();
                    break;
                case 4:
                    cancelRequest();
                    break;
                case 5:
                    displayDashboard();
                    break;
                case 6:
                    displayZoneAnalytics();
                    break;
                case 7:
                    displayFullHistory();
                    break;
                case 8:
                    performRollback();
                    break;
                case 9:
                    std::cout << "\n👋 Thank you for using the Parking System!\n";
                    std::cout << "System shutting down...\n\n";
                    return;
                default:
                    std::cout << "❌ Invalid choice! Please enter a number between 1 and 9.\n";
            }
        }
    }
};

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================

int main() {
    try {
        ParkingSystemUI ui;
        ui.run();
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
