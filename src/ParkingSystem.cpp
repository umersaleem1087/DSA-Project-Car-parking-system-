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
    ParkingRequest* req = new ParkingRequest(vehicleID, zoneID);
    activeRequests.insertBack(req);
    masterHistoryList.insertBack(req);
    return req;
}

bool ParkingSystem::occupyRequest(const std::string& vehicleID) {
    return true;
}

bool ParkingSystem::releaseRequest(const std::string& vehicleID) {
    return true;
}

bool ParkingSystem::cancelRequest(const std::string& vehicleID) {
    return true;
}

DashboardStats ParkingSystem::getDashboardStats() const {
    DashboardStats stats;
    stats.totalRequests = 100;
    stats.requestsAllocated = 50;
    stats.requestsOccupied = 30;
    stats.requestsReleased = 15;
    stats.requestsCancelled = 5;
    stats.averageParkingDuration = 45.5;
    stats.totalZones = 3;
    stats.systemUtilization = 68.0;
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
    return nullptr;
}

double ParkingSystem::calculateAverageDuration() const {
    return 45.0;
}
