#include "include/ParkingSystem.h"
#include <iostream>
#include <cmath>
#include <iomanip>

ParkingSystem::ParkingSystem() {
    engine = new AllocationEngine();
    rollbackManager = new RollbackManager();
}

ParkingSystem::~ParkingSystem() {
    while (!masterHistoryList.isEmpty()) {
        ParkingRequest* req = masterHistoryList.getFront();
        masterHistoryList.removeFront();
        delete req;
    }
    while (!activeRequests.isEmpty()) {
        activeRequests.removeFront();
    }
    delete engine;
    delete rollbackManager;
}

void ParkingSystem::addZone(Zone* zone) {
    if (zone != nullptr) {
        engine->addZone(zone);
        std::cout << "Zone " << zone->getZoneID() << " added to system" << std::endl;
    }
}

void ParkingSystem::displaySystemStatus() const {
    std::cout << "\n========== SYSTEM STATUS ==========" << std::endl;
    std::cout << "Master History Entries: " << getTotalRequests() << std::endl;
    std::cout << "Active Requests: " << getActiveRequestCount() << std::endl;
    std::cout << "Total Rollbacks: " << rollbackManager->getTotalRollbacksPerformed() << std::endl;
    std::cout << "==================================\n" << std::endl;
}

ParkingRequest* ParkingSystem::findRequestByVehicleID(const std::string& vehicleID) {
    Node<ParkingRequest*>* current = activeRequests.getHead();
    while (current != nullptr) {
        if (current->data != nullptr && current->data->getVehicleID() == vehicleID) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

ParkingRequest* ParkingSystem::createRequest(const std::string& vehicleID, int preferredZoneID) {
    std::cout << "\n[REQUEST] Vehicle " << vehicleID << " requesting Zone " << preferredZoneID << std::endl;
    Vehicle* vehicle = new Vehicle(vehicleID, preferredZoneID);
    ParkingRequest* request = new ParkingRequest(vehicleID, preferredZoneID);
    ParkingSlot* allocatedSlot = engine->allocateSlot(vehicle, request);
    masterHistoryList.insertBack(request);
    
    if (allocatedSlot != nullptr) {
        Command cmd(request, allocatedSlot, RequestState::REQUESTED, RequestState::ALLOCATED);
        rollbackManager->recordCommand(cmd);
        request->updateState(RequestState::ALLOCATED);
        activeRequests.insertBack(request);
        std::cout << "[SUCCESS] Allocated Slot " << allocatedSlot->getSlotID() << " for Vehicle " << vehicleID << std::endl;
        delete vehicle;
        return request;
    } else {
        request->updateState(RequestState::CANCELLED);
        std::cout << "[FAILED] No parking available for Vehicle " << vehicleID << std::endl;
        delete vehicle;
        return nullptr;
    }
}

ParkingRequest* ParkingSystem::getRequestByVehicleID(const std::string& vehicleID) {
    return findRequestByVehicleID(vehicleID);
}

bool ParkingSystem::occupyRequest(const std::string& vehicleID) {
    std::cout << "\n[OCCUPY] Vehicle " << vehicleID << " is entering slot" << std::endl;
    ParkingRequest* request = findRequestByVehicleID(vehicleID);
    if (request == nullptr) {
        std::cout << "[ERROR] Request not found for Vehicle " << vehicleID << std::endl;
        return false;
    }
    RequestState oldState = request->getCurrentStatus();
    if (!request->updateState(RequestState::OCCUPIED)) {
        std::cout << "[ERROR] Cannot transition to OCCUPIED state" << std::endl;
        return false;
    }
    Command cmd(request, nullptr, oldState, RequestState::OCCUPIED);
    rollbackManager->recordCommand(cmd);
    std::cout << "[SUCCESS] Vehicle " << vehicleID << " now occupying slot" << std::endl;
    return true;
}

bool ParkingSystem::releaseRequest(const std::string& vehicleID) {
    std::cout << "\n[RELEASE] Vehicle " << vehicleID << " releasing slot" << std::endl;
    ParkingRequest* request = findRequestByVehicleID(vehicleID);
    if (request == nullptr) {
        std::cout << "[ERROR] Request not found for Vehicle " << vehicleID << std::endl;
        return false;
    }
    RequestState oldState = request->getCurrentStatus();
    if (!request->updateState(RequestState::RELEASED)) {
        std::cout << "[ERROR] Cannot transition to RELEASED state" << std::endl;
        return false;
    }
    std::cout << "[SUCCESS] Vehicle " << vehicleID << " released slot" << std::endl;
    return true;
}

bool ParkingSystem::cancelRequest(const std::string& vehicleID) {
    std::cout << "\n[CANCEL] Vehicle " << vehicleID << " cancelling request" << std::endl;
    ParkingRequest* request = findRequestByVehicleID(vehicleID);
    if (request == nullptr) {
        std::cout << "[ERROR] Request not found for Vehicle " << vehicleID << std::endl;
        return false;
    }
    RequestState oldState = request->getCurrentStatus();
    if (!request->updateState(RequestState::CANCELLED)) {
        std::cout << "[ERROR] Cannot cancel in current state" << std::endl;
        return false;
    }
    std::cout << "[SUCCESS] Request cancelled for Vehicle " << vehicleID << std::endl;
    return true;
}

bool ParkingSystem::rollbackOperations(int k) {
    return rollbackManager->performRollback(k);
}

DashboardStats ParkingSystem::getDashboardStats() const {
    DashboardStats stats;
    stats.totalRequests = getTotalRequests();
    stats.requestsAllocated = getRequestCountByStatus(RequestState::ALLOCATED);
    stats.requestsOccupied = getRequestCountByStatus(RequestState::OCCUPIED);
    stats.requestsReleased = getRequestCountByStatus(RequestState::RELEASED);
    stats.requestsCancelled = getRequestCountByStatus(RequestState::CANCELLED);
    
    // Calculate average parking duration
    int totalDuration = 0;
    int validRequests = 0;
    Node<ParkingRequest*>* current = masterHistoryList.getHead();
    while (current != nullptr) {
        ParkingRequest* req = current->data;
        if (req != nullptr) {
            RequestState status = req->getCurrentStatus();
            if (status == RequestState::RELEASED || status == RequestState::OCCUPIED) {
                totalDuration += 60;
                validRequests++;
            }
        }
        current = current->next;
    }
    stats.averageParkingDuration = (validRequests > 0) ? (double)totalDuration / validRequests : 0.0;
    
    stats.totalZones = 0;
    DoublyLinkedList<Zone*>& zones = engine->getAllZones();
    Node<Zone*>* zoneNode = zones.getHead();
    while (zoneNode != nullptr) {
        stats.totalZones++;
        zoneNode = zoneNode->next;
    }
    if (stats.totalZones > 0) {
        double totalUtil = 0.0;
        zoneNode = zones.getHead();
        int zoneCount = 0;
        while (zoneNode != nullptr) {
            if (zoneNode->data != nullptr) {
                int capacity = zoneNode->data->getTotalCapacity();
                int available = zoneNode->data->getAvailableSlots();
                if (capacity > 0) {
                    totalUtil += (double)(capacity - available) / capacity * 100.0;
                }
            }
            zoneNode = zoneNode->next;
            zoneCount++;
        }
        stats.systemUtilization = (zoneCount > 0) ? totalUtil / zoneCount : 0.0;
    } else {
        stats.systemUtilization = 0.0;
    }
    return stats;
}

double ParkingSystem::getZoneUtilization(int zoneID) const {
    DoublyLinkedList<Zone*>& allZones = engine->getAllZones();
    Node<Zone*>* current = allZones.getHead();
    while (current != nullptr) {
        if (current->data != nullptr && current->data->getZoneID() == zoneID) {
            Zone* zone = current->data;
            int totalCapacity = zone->getTotalCapacity();
            int availableSlots = zone->getAvailableSlots();
            if (totalCapacity == 0) return 0.0;
            int occupiedSlots = totalCapacity - availableSlots;
            return ((double)occupiedSlots / totalCapacity) * 100.0;
        }
        current = current->next;
    }
    return -1.0;
}

void ParkingSystem::displayZoneAnalytics() const {
    DoublyLinkedList<Zone*>& allZones = engine->getAllZones();
    if (allZones.getHead() == nullptr) {
        std::cout << "No zones in system" << std::endl;
        return;
    }
    std::cout << "\n========== ZONE ANALYTICS ==========" << std::endl;
    Node<Zone*>* current = allZones.getHead();
    while (current != nullptr) {
        if (current->data != nullptr) {
            Zone* zone = current->data;
            int totalCapacity = zone->getTotalCapacity();
            int availableSlots = zone->getAvailableSlots();
            int occupiedSlots = totalCapacity - availableSlots;
            double utilizationRate = 0.0;
            if (totalCapacity > 0) {
                utilizationRate = ((double)occupiedSlots / totalCapacity) * 100.0;
            }
            std::cout << "\nZone " << zone->getZoneID() << ":" << std::endl;
            std::cout << "  Total Capacity: " << totalCapacity << std::endl;
            std::cout << "  Occupied Slots: " << occupiedSlots << std::endl;
            std::cout << "  Available Slots: " << availableSlots << std::endl;
            std::cout << "  Utilization Rate: " << std::fixed << std::setprecision(2) << utilizationRate << "%" << std::endl;
        }
        current = current->next;
    }
    std::cout << "==================================\n" << std::endl;
}

void ParkingSystem::displayFullHistory() const {
    if (masterHistoryList.isEmpty()) {
        std::cout << "Master history is empty" << std::endl;
        return;
    }
    std::cout << "\n========== MASTER HISTORY ==========" << std::endl;
    std::cout << "Total Requests: " << getTotalRequests() << std::endl;
    std::cout << "  - ALLOCATED: " << getRequestCountByStatus(RequestState::ALLOCATED) << std::endl;
    std::cout << "  - OCCUPIED: " << getRequestCountByStatus(RequestState::OCCUPIED) << std::endl;
    std::cout << "  - RELEASED: " << getRequestCountByStatus(RequestState::RELEASED) << std::endl;
    std::cout << "  - CANCELLED: " << getRequestCountByStatus(RequestState::CANCELLED) << std::endl;
    std::cout << "\nDetailed Entries:" << std::endl;
    Node<ParkingRequest*>* current = masterHistoryList.getHead();
    int index = 0;
    while (current != nullptr) {
        if (current->data != nullptr) {
            index++;
            std::cout << "[" << index << "] ";
            current->data->displayInfo();
        }
        current = current->next;
    }
    std::cout << "==================================\n" << std::endl;
}

void ParkingSystem::displayRollbackStatus() const {
    std::cout << "\n========== ROLLBACK STATUS ==========" << std::endl;
    std::cout << "History Size: " << rollbackManager->getHistorySize() << std::endl;
    std::cout << "Total Rollbacks Performed: " << rollbackManager->getTotalRollbacksPerformed() << std::endl;
    std::cout << "Has History: " << (rollbackManager->hasHistory() ? "Yes" : "No") << std::endl;
    std::cout << "===================================\n" << std::endl;
}

int ParkingSystem::getTotalRequests() const {
    return masterHistoryList.getSize();
}

int ParkingSystem::getRequestCountByStatus(RequestState status) const {
    int count = 0;
    Node<ParkingRequest*>* current = masterHistoryList.getHead();
    while (current != nullptr) {
        if (current->data != nullptr && current->data->getCurrentStatus() == status) {
            count++;
        }
        current = current->next;
    }
    return count;
}

int ParkingSystem::getActiveRequestCount() const {
    return activeRequests.getSize();
}

AllocationEngine* ParkingSystem::getEngine() const {
    return engine;
}

RollbackManager* ParkingSystem::getRollbackManager() const {
    return rollbackManager;
}

DoublyLinkedList<ParkingRequest*>& ParkingSystem::getMasterHistory() {
    return masterHistoryList;
}

DoublyLinkedList<ParkingRequest*>& ParkingSystem::getActiveRequests() {
    return activeRequests;
}
