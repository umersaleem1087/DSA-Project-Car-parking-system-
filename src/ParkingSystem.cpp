#include "ParkingSystem.h"
#include <iostream>

ParkingSystem::ParkingSystem() {
    engine = new AllocationEngine();
    rollbackManager = new RollbackManager();
}

ParkingSystem::~ParkingSystem() {
    if (engine) delete engine;
    if (rollbackManager) delete rollbackManager;
}

void ParkingSystem::addZone(Zone* zone) {
    if (zone != nullptr) {
        engine->addZone(zone);
    }
}

ParkingRequest* ParkingSystem::createRequest(const std::string& vehicleID, int zoneID) {
    // Check if vehicle already has an active request
    auto currentNode = activeRequests.getHead();
    while (currentNode != nullptr) {
        if (currentNode->data != nullptr && currentNode->data->getVehicleID() == vehicleID) {
            // Vehicle already has an active request
            std::cerr << "❌ ERROR: Vehicle " << vehicleID << " already has an active request!\n";
            return nullptr;
        }
        currentNode = currentNode->next;
    }
    
    ParkingRequest* req = new ParkingRequest(vehicleID, zoneID);
    activeRequests.insertBack(req);
    masterHistoryList.insertBack(req);
    return req;
}

bool ParkingSystem::occupyRequest(const std::string& vehicleID) {
    return true;
}

bool ParkingSystem::releaseRequest(const std::string& vehicleID) {
    // Find the request for this vehicle
    auto currentNode = activeRequests.getHead();
    while (currentNode != nullptr) {
        if (currentNode->data != nullptr && currentNode->data->getVehicleID() == vehicleID) {
            ParkingRequest* request = currentNode->data;
            
            // Check if vehicle is actually occupying a slot (OCCUPIED state)
            if (request->getCurrentStatus() != RequestState::OCCUPIED) {
                std::cerr << "❌ ERROR: Vehicle " << vehicleID << " is not currently occupying a slot!\n";
                std::cerr << "   Current status: " << request->statusToString(request->getCurrentStatus()) << "\n";
                return false;
            }
            
            // Update the request status to RELEASED
            request->updateState(RequestState::RELEASED);
            
            // Remove from active requests since it's released
            auto nodeToRemove = currentNode;
            currentNode = currentNode->next;  // Move to next before deletion
            activeRequests.removeNode(nodeToRemove);
            
            return true;
        }
        currentNode = currentNode->next;
    }
    
    std::cerr << "❌ ERROR: Vehicle " << vehicleID << " not found in system!\n";
    return false;
}

bool ParkingSystem::cancelRequest(const std::string& vehicleID) {
    return true;
}

DashboardStats ParkingSystem::getDashboardStats() const {
    DashboardStats stats;
    stats.totalRequests = masterHistoryList.getSize();
    stats.requestsAllocated = 0;  // Would need proper counting logic
    stats.requestsOccupied = 0;   // Would need proper counting logic
    stats.requestsReleased = 0;   // Would need proper counting logic
    stats.requestsCancelled = 0;  // Would need proper counting logic
    stats.averageParkingDuration = calculateAverageDuration();
    stats.totalZones = engine->getAllZones().getSize();
    stats.systemUtilization = 68.0;
    
    // Populate zone slot statuses
    auto zoneNode = engine->getAllZones().getHead();
    while (zoneNode != nullptr) {
        if (zoneNode->data != nullptr) {
            Zone* zone = zoneNode->data;
            ZoneSlotStatus zoneStatus;
            zoneStatus.zoneID = zone->getZoneID();
            zoneStatus.totalSlots = zone->getTotalCapacity();
            zoneStatus.availableSlots = zone->getAvailableSlots();
            zoneStatus.occupiedSlots = zoneStatus.totalSlots - zoneStatus.availableSlots;
            zoneStatus.utilization = (zoneStatus.totalSlots > 0) ? 
                (100.0 * zoneStatus.occupiedSlots / zoneStatus.totalSlots) : 0.0;
            
            stats.zoneStatuses.insertBack(zoneStatus);
        }
        zoneNode = zoneNode->next;
    }
    
    return stats;
}

double ParkingSystem::getZoneUtilization(int zoneID) const {
    return 65.0;
}

DoublyLinkedList<ParkingRequest*>& ParkingSystem::getMasterHistory() {
    return masterHistoryList;
}

bool ParkingSystem::rollbackOperations(int k) {
    return rollbackManager->performRollback(k);
}

void ParkingSystem::displayRollbackStatus() const {
    std::cout << "Rollback Status: " << rollbackManager->getTotalRollbacksPerformed() 
              << " rollbacks performed." << std::endl;
}

ParkingRequest* ParkingSystem::findRequestByVehicleID(const std::string& vehicleID) {
    auto currentNode = activeRequests.getHead();
    while (currentNode != nullptr) {
        if (currentNode->data != nullptr && currentNode->data->getVehicleID() == vehicleID) {
            return currentNode->data;
        }
        currentNode = currentNode->next;
    }
    return nullptr;
}

ParkingRequest* ParkingSystem::getRequestByVehicleID(const std::string& vehicleID) {
    return findRequestByVehicleID(vehicleID);
}

double ParkingSystem::calculateAverageDuration() const {
    return 45.0;
}
void ParkingSystem::displaySystemStatus() const {
    std::cout << "System Status:\n";
    std::cout << "Active Requests: " << activeRequests.getSize() << "\n";
    std::cout << "Total History: " << masterHistoryList.getSize() << "\n";
}

void ParkingSystem::displayZoneAnalytics() const {
    std::cout << "Zone Analytics:\n";
    std::cout << "Total Zones: " << engine->getAllZones().getSize() << "\n";
}

void ParkingSystem::displayFullHistory() const {
    std::cout << "Full Parking History:\n";
    std::cout << "Total requests: " << masterHistoryList.getSize() << "\n";
}