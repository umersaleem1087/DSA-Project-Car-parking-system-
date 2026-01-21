#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

// ============================================================================
// INTERACTIVE PARKING SYSTEM UI - WITH USER INPUT
// ============================================================================

struct Request {
    int id;
    std::string vehicleID;
    int zoneID;
    std::string status; // ALLOCATED, OCCUPIED, RELEASED, CANCELLED
};

class InteractiveParkingUI {
private:
    std::vector<Request> requests;
    int requestCounter = 1;
    std::map<int, int> zoneOccupancy; // zone -> occupied count
    std::map<int, int> zoneCapacity;  // zone -> total capacity
    
public:
    InteractiveParkingUI() {
        // Initialize zones
        zoneCapacity[1] = 10;
        zoneCapacity[2] = 10;
        zoneCapacity[3] = 24;
        
        zoneOccupancy[1] = 0;
        zoneOccupancy[2] = 0;
        zoneOccupancy[3] = 0;
    }
    
    void displayHeader() {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                                                            ║\n";
        std::cout << "║  🅿️  DSA PARKING SYSTEM - INTERACTIVE CONSOLE UI           ║\n";
        std::cout << "║  Real-time Request Processing & Statistics                ║\n";
        std::cout << "║                                                            ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    }
    
    void displayMenu() {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║              PARKING SYSTEM MAIN MENU                      ║\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ 1. Create Parking Request                                 ║\n";
        std::cout << "║ 2. Occupy Parking Slot                                    ║\n";
        std::cout << "║ 3. Release Parking Slot                                   ║\n";
        std::cout << "║ 4. Cancel Parking Request                                 ║\n";
        std::cout << "║ 5. View Dashboard                                         ║\n";
        std::cout << "║ 6. View Zone Analytics                                    ║\n";
        std::cout << "║ 7. View All Requests                                      ║\n";
        std::cout << "║ 8. Exit                                                   ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
        std::cout << "\nEnter your choice (1-8): ";
    }
    
    void createRequest() {
        std::cout << "\n═══════════════════════════════════════════════════════════\n";
        std::cout << "              CREATE PARKING REQUEST\n";
        std::cout << "═══════════════════════════════════════════════════════════\n";
        
        std::string vehicleID;
        int zoneID;
        
        std::cout << "\nEnter Vehicle ID: ";
        std::getline(std::cin, vehicleID);
        
        if (vehicleID.empty()) {
            std::cout << "❌ Error: Vehicle ID cannot be empty!\n";
            return;
        }
        
        std::cout << "Select Zone (1-3): ";
        std::cin >> zoneID;
        std::cin.ignore();
        
        if (zoneID < 1 || zoneID > 3) {
            std::cout << "❌ Error: Invalid zone! Choose 1, 2, or 3.\n";
            return;
        }
        
        if (zoneOccupancy[zoneID] >= zoneCapacity[zoneID]) {
            std::cout << "❌ Error: Zone " << zoneID << " is full!\n";
            return;
        }
        
        // Create request
        Request req;
        req.id = requestCounter++;
        req.vehicleID = vehicleID;
        req.zoneID = zoneID;
        req.status = "ALLOCATED";
        
        requests.push_back(req);
        
        std::cout << "\n✅ Request created successfully!\n";
        std::cout << "   Request ID: " << req.id << "\n";
        std::cout << "   Vehicle ID: " << vehicleID << "\n";
        std::cout << "   Zone: " << zoneID << "\n";
        std::cout << "   Status: ALLOCATED\n";
    }
    
    void occupySlot() {
        std::cout << "\n═══════════════════════════════════════════════════════════\n";
        std::cout << "              OCCUPY PARKING SLOT\n";
        std::cout << "═══════════════════════════════════════════════════════════\n";
        
        std::string vehicleID;
        std::cout << "\nEnter Vehicle ID to occupy: ";
        std::getline(std::cin, vehicleID);
        
        bool found = false;
        for (auto& req : requests) {
            if (req.vehicleID == vehicleID && req.status == "ALLOCATED") {
                req.status = "OCCUPIED";
                zoneOccupancy[req.zoneID]++;
                found = true;
                
                std::cout << "\n✅ Parking slot occupied!\n";
                std::cout << "   Vehicle ID: " << vehicleID << "\n";
                std::cout << "   Zone: " << req.zoneID << "\n";
                std::cout << "   Status: OCCUPIED\n";
                break;
            }
        }
        
        if (!found) {
            std::cout << "❌ Error: Vehicle not found or not in ALLOCATED state!\n";
        }
    }
    
    void releaseSlot() {
        std::cout << "\n═══════════════════════════════════════════════════════════\n";
        std::cout << "              RELEASE PARKING SLOT\n";
        std::cout << "═══════════════════════════════════════════════════════════\n";
        
        std::string vehicleID;
        std::cout << "\nEnter Vehicle ID to release: ";
        std::getline(std::cin, vehicleID);
        
        bool found = false;
        for (auto& req : requests) {
            if (req.vehicleID == vehicleID && req.status == "OCCUPIED") {
                req.status = "RELEASED";
                zoneOccupancy[req.zoneID]--;
                found = true;
                
                std::cout << "\n✅ Parking slot released!\n";
                std::cout << "   Vehicle ID: " << vehicleID << "\n";
                std::cout << "   Zone: " << req.zoneID << "\n";
                std::cout << "   Status: RELEASED\n";
                break;
            }
        }
        
        if (!found) {
            std::cout << "❌ Error: Vehicle not found or not in OCCUPIED state!\n";
        }
    }
    
    void cancelRequest() {
        std::cout << "\n═══════════════════════════════════════════════════════════\n";
        std::cout << "              CANCEL PARKING REQUEST\n";
        std::cout << "═══════════════════════════════════════════════════════════\n";
        
        std::string vehicleID;
        std::cout << "\nEnter Vehicle ID to cancel: ";
        std::getline(std::cin, vehicleID);
        
        bool found = false;
        for (auto& req : requests) {
            if (req.vehicleID == vehicleID && req.status == "ALLOCATED") {
                req.status = "CANCELLED";
                found = true;
                
                std::cout << "\n✅ Request cancelled!\n";
                std::cout << "   Vehicle ID: " << vehicleID << "\n";
                std::cout << "   Status: CANCELLED\n";
                break;
            }
        }
        
        if (!found) {
            std::cout << "❌ Error: Vehicle not found or already processed!\n";
        }
    }
    
    void displayDashboard() {
        std::cout << "\n═══════════════════════════════════════════════════════════\n";
        std::cout << "              DASHBOARD - SYSTEM STATISTICS\n";
        std::cout << "═══════════════════════════════════════════════════════════\n\n";
        
        int total = requests.size();
        int allocated = 0, occupied = 0, released = 0, cancelled = 0;
        
        for (const auto& req : requests) {
            if (req.status == "ALLOCATED") allocated++;
            else if (req.status == "OCCUPIED") occupied++;
            else if (req.status == "RELEASED") released++;
            else if (req.status == "CANCELLED") cancelled++;
        }
        
        std::cout << std::left << std::setw(30) << "📊 Total Requests:"
                  << std::right << std::setw(10) << total << "\n";
        std::cout << std::left << std::setw(30) << "   ├─ Allocated:"
                  << std::right << std::setw(10) << allocated << "\n";
        std::cout << std::left << std::setw(30) << "   ├─ Occupied:"
                  << std::right << std::setw(10) << occupied << "\n";
        std::cout << std::left << std::setw(30) << "   ├─ Released:"
                  << std::right << std::setw(10) << released << "\n";
        std::cout << std::left << std::setw(30) << "   └─ Cancelled:"
                  << std::right << std::setw(10) << cancelled << "\n\n";
        
        // Utilization
        int totalCapacity = 44;
        int totalOccupied = occupied;
        int utilization = (totalCapacity > 0) ? (totalOccupied * 100) / totalCapacity : 0;
        
        std::cout << std::left << std::setw(30) << "📈 System Utilization:"
                  << std::right << std::setw(10) << utilization << "%\n";
        
        std::cout << "   [";
        for (int i = 0; i < 50; i++) {
            if (i < utilization / 2) {
                std::cout << "█";
            } else {
                std::cout << "░";
            }
        }
        std::cout << "]\n";
    }
    
    void displayZoneAnalytics() {
        std::cout << "\n═══════════════════════════════════════════════════════════\n";
        std::cout << "              ZONE ANALYTICS\n";
        std::cout << "═══════════════════════════════════════════════════════════\n\n";
        
        std::cout << std::left << std::setw(8) << "Zone"
                  << std::setw(15) << "Total Slots"
                  << std::setw(12) << "Occupied"
                  << std::setw(15) << "Available"
                  << std::setw(15) << "Utilization\n";
        std::cout << "───────────────────────────────────────────────────────────\n";
        
        for (int zone = 1; zone <= 3; zone++) {
            int total = zoneCapacity[zone];
            int occupied = zoneOccupancy[zone];
            int available = total - occupied;
            int util = (total > 0) ? (occupied * 100) / total : 0;
            
            std::cout << std::left << std::setw(8) << ("Zone " + std::to_string(zone))
                      << std::setw(15) << total
                      << std::setw(12) << occupied
                      << std::setw(15) << available
                      << std::setw(15) << (std::to_string(util) + "%") << "\n";
        }
    }
    
    void displayAllRequests() {
        std::cout << "\n═══════════════════════════════════════════════════════════\n";
        std::cout << "              ALL PARKING REQUESTS\n";
        std::cout << "═══════════════════════════════════════════════════════════\n\n";
        
        if (requests.empty()) {
            std::cout << "No requests yet.\n";
            return;
        }
        
        std::cout << std::left << std::setw(8) << "ID"
                  << std::setw(15) << "Vehicle ID"
                  << std::setw(8) << "Zone"
                  << std::setw(15) << "Status\n";
        std::cout << "───────────────────────────────────────────────────────────\n";
        
        for (const auto& req : requests) {
            std::cout << std::left << std::setw(8) << req.id
                      << std::setw(15) << req.vehicleID
                      << std::setw(8) << req.zoneID
                      << std::setw(15) << req.status << "\n";
        }
    }
    
    void run() {
        displayHeader();
        int choice;
        
        while (true) {
            displayMenu();
            std::cin >> choice;
            std::cin.ignore();
            
            switch (choice) {
                case 1:
                    createRequest();
                    break;
                case 2:
                    occupySlot();
                    break;
                case 3:
                    releaseSlot();
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
                    displayAllRequests();
                    break;
                case 8:
                    std::cout << "\n👋 Thank you for using the Parking System!\n";
                    std::cout << "Exiting...\n\n";
                    return;
                default:
                    std::cout << "❌ Invalid choice! Please enter 1-8.\n";
            }
        }
    }
};

int main() {
    InteractiveParkingUI ui;
    ui.run();
    return 0;
}
