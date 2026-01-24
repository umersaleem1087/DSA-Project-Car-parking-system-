#include "ParkingSystem.h"
#include "ParkingArea.h"
#include "ParkingSlot.h"
#include "Zone.h"
#include <iostream>
#include <cstdint>
#include <vector>

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

bool ParkingSystem::createZone(int zoneID, int numSlots) {
    // Check if zone already exists
    auto zones = engine->getAllZones();
    auto checkNode = zones.getHead();
    while (checkNode != nullptr) {
        if (checkNode->data != nullptr && checkNode->data->getZoneID() == zoneID) {
            std::cerr << "❌ ERROR: Zone " << zoneID << " already exists!\n";
            return false;
        }
        checkNode = checkNode->next;
    }
    
    // Validate input
    if (numSlots <= 0) {
        std::cerr << "❌ ERROR: Number of slots must be greater than 0!\n";
        return false;
    }
    
    try {
        // Create new zone
        Zone* newZone = new Zone(zoneID);
        
        // Create parking area and add slots
        ParkingArea* area = new ParkingArea(1);  // Area ID 1 for simplicity
        for (int i = 1; i <= numSlots; i++) {
            ParkingSlot* slot = new ParkingSlot(i, zoneID);
            area->addSlot(slot);
        }
        newZone->addParkingArea(area);
        
        // Refresh capacity to ensure accurate counts
        newZone->refreshCapacity();
        
        // Add to engine
        engine->addZone(newZone);
        
        // Store zone info for rollback
        zoneCreationHistory.insertBack(newZone);
        
        std::cout << "✅ Zone " << zoneID << " created successfully with " << numSlots << " slots\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ ERROR creating zone: " << e.what() << "\n";
        return false;
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
    
    // Get the zone for the command
    Zone* zone = getZoneByID(zoneID);
    
    // Record the creation as a command for rollback
    // oldState is REQUESTED, newState is also REQUESTED (just created)
    // This allows us to identify creation operations during rollback
    Command createCmd;
    createCmd.requestPtr = req;
    createCmd.slotPtr = nullptr;
    createCmd.zonePtr = zone;
    createCmd.oldState = RequestState::REQUESTED;  // Marker for "before creation"
    createCmd.newState = RequestState::REQUESTED;  // Just created
    rollbackManager->recordCommand(createCmd);
    
    std::cout << "✅ Request created for Vehicle " << vehicleID << " in Zone " << zoneID << "\n";
    return req;
}

bool ParkingSystem::allocateSlotForRequest(const std::string& vehicleID) {
    // Find the request for this vehicle
    auto currentNode = activeRequests.getHead();
    while (currentNode != nullptr) {
        if (currentNode->data != nullptr && currentNode->data->getVehicleID() == vehicleID) {
            ParkingRequest* request = currentNode->data;
            
            // Check if request is in REQUESTED state (not already allocated)
            if (request->getCurrentStatus() != RequestState::REQUESTED) {
                std::cerr << "❌ ERROR: Vehicle " << vehicleID << " request is not in REQUESTED state!\n";
                std::cerr << "   Current status: " << request->statusToString(request->getCurrentStatus()) << "\n";
                return false;
            }
            
            // Create a temporary vehicle object for allocation engine
            // Note: We need the preferred zone ID from the request
            Vehicle tempVehicle(vehicleID, request->getRequestedZoneID());
            
            // Call allocation engine to find an available slot
            ParkingSlot* allocatedSlot = engine->allocateSlot(&tempVehicle, request);
            
            if (allocatedSlot != nullptr) {
                // Get the zone for the command
                Zone* zone = getZoneByID(request->getRequestedZoneID());
                
                // Record command for rollback
                Command cmd;
                cmd.requestPtr = request;
                cmd.slotPtr = allocatedSlot;
                cmd.zonePtr = zone;
                cmd.oldState = RequestState::REQUESTED;
                cmd.newState = RequestState::ALLOCATED;
                rollbackManager->recordCommand(cmd);
                
                std::cout << "✅ Slot allocated for Vehicle " << vehicleID << "\n";
                return true;
            } else {
                std::cerr << "❌ ERROR: No parking slots available for Vehicle " << vehicleID << "\n";
                return false;
            }
        }
        currentNode = currentNode->next;
    }
    
    std::cerr << "❌ ERROR: Vehicle " << vehicleID << " not found in system!\n";
    return false;
}

bool ParkingSystem::occupyRequest(const std::string& vehicleID) {
    // Find the request for this vehicle
    auto currentNode = activeRequests.getHead();
    while (currentNode != nullptr) {
        if (currentNode->data != nullptr && currentNode->data->getVehicleID() == vehicleID) {
            ParkingRequest* request = currentNode->data;
            
            // Check if vehicle is in ALLOCATED state (can occupy)
            if (request->getCurrentStatus() != RequestState::ALLOCATED) {
                std::cerr << "❌ ERROR: Vehicle " << vehicleID << " does not have an allocated slot!\n";
                std::cerr << "   Current status: " << request->statusToString(request->getCurrentStatus()) << "\n";
                return false;
            }
            
            // Record command for rollback
            // Get the zone for the command (need to get the slot from the request somehow)
            Zone* zone = getZoneByID(request->getRequestedZoneID());
            Command cmd;
            cmd.requestPtr = request;
            cmd.slotPtr = nullptr;  // No slot change during occupy
            cmd.zonePtr = zone;
            cmd.oldState = RequestState::ALLOCATED;
            cmd.newState = RequestState::OCCUPIED;
            rollbackManager->recordCommand(cmd);
            
            // Transition to OCCUPIED state
            request->updateState(RequestState::OCCUPIED);
            std::cout << "✅ Vehicle " << vehicleID << " is now occupying the slot\n";
            return true;
        }
        currentNode = currentNode->next;
    }
    
    std::cerr << "❌ ERROR: Vehicle " << vehicleID << " not found in system!\n";
    return false;
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
            
            // Get allocated slot before freeing (for rollback)
            int slotID = request->getAllocatedSlotID();
            RequestState oldState = request->getCurrentStatus();
            if (slotID != -1) {
                engine->freeSlot(slotID);
            }
            
            // Record command for rollback
            Command cmd;
            cmd.requestPtr = request;
            cmd.slotPtr = nullptr;
            cmd.oldState = oldState;
            cmd.newState = RequestState::RELEASED;
            rollbackManager->recordCommand(cmd);
            
            // Update the request status to RELEASED
            request->updateState(RequestState::RELEASED);
            
            // Remove from active requests since it's released
            auto nodeToRemove = currentNode;
            currentNode = currentNode->next;  // Move to next before deletion
            activeRequests.removeNode(nodeToRemove);
            
            std::cout << "✅ Vehicle " << vehicleID << " has released parking slot " << slotID << "\n";
            return true;
        }
        currentNode = currentNode->next;
    }
    
    std::cerr << "❌ ERROR: Vehicle " << vehicleID << " not found in system!\n";
    return false;
}

bool ParkingSystem::cancelRequest(const std::string& vehicleID) {
    // Find the request for this vehicle
    auto currentNode = activeRequests.getHead();
    while (currentNode != nullptr) {
        if (currentNode->data != nullptr && currentNode->data->getVehicleID() == vehicleID) {
            ParkingRequest* request = currentNode->data;
            
            // Check if request is not already RELEASED or CANCELLED
            RequestState currentStatus = request->getCurrentStatus();
            if (currentStatus == RequestState::RELEASED || currentStatus == RequestState::CANCELLED) {
                std::cerr << "❌ ERROR: Vehicle " << vehicleID << " request has already been cancelled or released!\n";
                std::cerr << "   Current status: " << request->statusToString(currentStatus) << "\n";
                return false;
            }
            
            // Get allocated slot before freeing (for rollback)
            int slotID = request->getAllocatedSlotID();
            RequestState oldState = currentStatus;
            if (slotID != -1) {
                engine->freeSlot(slotID);
            }
            
            // Record command for rollback
            Command cmd;
            cmd.requestPtr = request;
            cmd.slotPtr = nullptr;
            cmd.oldState = oldState;
            cmd.newState = RequestState::CANCELLED;
            rollbackManager->recordCommand(cmd);
            
            // Update the request status to CANCELLED
            request->updateState(RequestState::CANCELLED);
            
            // Remove from active requests - vehicle is out of the system
            auto nodeToRemove = currentNode;
            currentNode = currentNode->next;  // Move to next before deletion
            activeRequests.removeNode(nodeToRemove);
            
            std::cout << "✅ Vehicle " << vehicleID << " request cancelled and removed from system\n";
            return true;
        }
        currentNode = currentNode->next;
    }
    
    std::cerr << "❌ ERROR: Vehicle " << vehicleID << " not found in system!\n";
    return false;
}

DashboardStats ParkingSystem::getDashboardStats() const {
    DashboardStats stats;
    
    try {
        // Count total requests from master history (excluding cancelled from active counts)
        stats.totalRequests = masterHistoryList.getSize();
        stats.requestsAllocated = 0;
        stats.requestsOccupied = 0;
        stats.requestsReleased = 0;
        stats.requestsCancelled = 0;
        
        // Count requests by status - only count active (non-cancelled) vehicles
        auto reqNode = masterHistoryList.getHead();
        while (reqNode != nullptr) {
            if (reqNode->data != nullptr) {
                ParkingRequest* req = reqNode->data;
                RequestState status = req->getCurrentStatus();
                
                // Only count active vehicles (not cancelled or released)
                if (status == RequestState::ALLOCATED) {
                    stats.requestsAllocated++;
                } else if (status == RequestState::OCCUPIED) {
                    stats.requestsOccupied++;
                } else if (status == RequestState::RELEASED) {
                    stats.requestsReleased++;
                } else if (status == RequestState::CANCELLED) {
                    stats.requestsCancelled++;
                }
            }
            reqNode = reqNode->next;
        }
        
        stats.averageParkingDuration = calculateAverageDuration();
        
        // Get zones list and immediately collect zone pointers to avoid pointer corruption
        const DoublyLinkedList<Zone*>& zonesList = engine->getAllZones();
        stats.totalZones = zonesList.getSize();
        
        // Create a safe copy of zone pointers first
        std::vector<Zone*> zonePtrs;
        auto tempNode = zonesList.getHead();
        int tempCount = 0;
        
        while (tempNode != nullptr && tempCount < 10000) {
            // Validate tempNode pointer
            if ((intptr_t)tempNode < 0x1000 || (intptr_t)tempNode > 0x7FFFFFFF0000) {
                break;
            }
            
            if (tempNode->data != nullptr) {
                // Double-check the data pointer
                if ((intptr_t)(tempNode->data) >= 0x1000 && (intptr_t)(tempNode->data) <= 0x7FFFFFFF0000) {
                    zonePtrs.push_back(tempNode->data);
                }
            }
            
            // Get next pointer safely
            auto nextPtr = tempNode->next;
            tempNode = nextPtr;
            tempCount++;
        }
        
        // Now process the copied pointers (safer approach)
        int totalSlots = 0;
        int occupiedSlots = 0;
        
        for (size_t i = 0; i < zonePtrs.size() && i < 10000; ++i) {
            Zone* zone = zonePtrs[i];
            
            try {
                // Check zone pointer validity
                if ((intptr_t)zone < 0x1000 || (intptr_t)zone > 0x7FFFFFFF0000) {
                    continue;
                }
                
                int zoneID = zone->getZoneID();
                
                // Check if zoneID is reasonable
                if (zoneID < 1 || zoneID > 999) {
                    continue;
                }
                
                int zoneTotalSlots = zone->getTotalCapacity();
                
                if (zoneTotalSlots <= 0 || zoneTotalSlots > 100000) {
                    continue;
                }
                
                int zoneAvailableSlots = zone->getAvailableSlots();
                
                if (zoneAvailableSlots < 0 || zoneAvailableSlots > zoneTotalSlots) {
                    continue;
                }
                
                int zoneOccupiedSlots = zoneTotalSlots - zoneAvailableSlots;
                
                totalSlots += zoneTotalSlots;
                occupiedSlots += zoneOccupiedSlots;
                
                // Create zone status
                ZoneSlotStatus zoneStatus;
                zoneStatus.zoneID = zoneID;
                zoneStatus.totalSlots = zoneTotalSlots;
                zoneStatus.availableSlots = zoneAvailableSlots;
                zoneStatus.occupiedSlots = zoneOccupiedSlots;
                zoneStatus.utilization = (zoneTotalSlots > 0) ? 
                    (100.0 * zoneOccupiedSlots / zoneTotalSlots) : 0.0;
                
                stats.zoneStatuses.insertBack(zoneStatus);
            } catch (const std::exception& e) {
                std::cerr << "Exception processing zone: " << e.what() << "\n";
            } catch (...) {
                std::cerr << "Unknown exception processing zone\n";
            }
        }
        
        // Set actual occupied slots from zones (cancelled vehicles have freed their slots)
        stats.actualOccupiedSlots = occupiedSlots;
        
        // Calculate overall system utilization
        stats.systemUtilization = (totalSlots > 0) ? (100.0 * occupiedSlots / totalSlots) : 0.0;
        
        return stats;
    } catch (const std::exception& e) {
        std::cerr << "Exception in getDashboardStats: " << e.what() << "\n";
        // Don't re-throw, return partial stats to avoid crash
        return stats;
    } catch (...) {
        std::cerr << "Unknown exception in getDashboardStats\n";
        // Don't re-throw, return partial stats to avoid crash
        return stats;
    }
}

double ParkingSystem::getZoneUtilization(int zoneID) const {
    return 65.0;
}

DoublyLinkedList<ParkingRequest*>& ParkingSystem::getMasterHistory() {
    return masterHistoryList;
}

bool ParkingSystem::rollbackOperations(int k) {
    // Perform rollback using the rollback manager
    if (!rollbackManager->performRollback(k)) {
        return false;
    }
    
    // After rollback, clean up and restore system state
    
    // STEP 1: Remove rolled-back creation operations
    // Vehicles that were created then rolled back are marked as CANCELLED
    // Remove them from both activeRequests and masterHistoryList
    auto activeNode = activeRequests.getHead();
    while (activeNode != nullptr) {
        if (activeNode->data != nullptr) {
            ParkingRequest* req = activeNode->data;
            RequestState status = req->getCurrentStatus();
            
            // If a vehicle is CANCELLED and was just rolled back from REQUESTED state,
            // it means its creation was rolled back - remove it completely
            if (status == RequestState::CANCELLED) {
                std::string vehicleID = req->getVehicleID();
                
                // Free any allocated slot before removing the vehicle
                int slotID = req->getAllocatedSlotID();
                if (slotID != -1) {
                    engine->freeSlot(slotID);
                    std::cout << "  ✓ Freed slot " << slotID << " for Vehicle " << vehicleID << "\n";
                }
                
                auto nodeToRemove = activeNode;
                activeNode = activeNode->next;  // Move to next before removal
                activeRequests.removeNode(nodeToRemove);
                std::cout << "✓ Vehicle " << vehicleID << " completely removed from system\n";
                continue;
            }
        }
        activeNode = activeNode->next;
    }
    
    // STEP 2: Handle REQUESTED state vehicles from rollback
    // If a vehicle rolls back to REQUESTED (but creation wasn't rolled back),
    // it stays in activeRequests
    
    // STEP 3: Restore any cancelled requests that should be active again
    // Scan masterHistory and add any ALLOCATED/OCCUPIED requests to activeRequests
    auto masterNode = masterHistoryList.getHead();
    while (masterNode != nullptr) {
        if (masterNode->data != nullptr) {
            ParkingRequest* req = masterNode->data;
            RequestState status = req->getCurrentStatus();
            
            // Only restore ALLOCATED or OCCUPIED states (not REQUESTED, RELEASED, or CANCELLED)
            if (status == RequestState::ALLOCATED || status == RequestState::OCCUPIED) {
                
                // Check if already in activeRequests
                auto activeCheckNode = activeRequests.getHead();
                bool found = false;
                while (activeCheckNode != nullptr) {
                    if (activeCheckNode->data == req) {
                        found = true;
                        break;
                    }
                    activeCheckNode = activeCheckNode->next;
                }
                
                // If not found, add it back to active
                if (!found) {
                    activeRequests.insertBack(req);
                    std::cout << "✓ Restored request for Vehicle " << req->getVehicleID() 
                             << " to active requests\n";
                }
            }
        }
        masterNode = masterNode->next;
    }
    
    return true;
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

RollbackManager* ParkingSystem::getRollbackManager() const {
    return rollbackManager;
}

DoublyLinkedList<ParkingRequest*>& ParkingSystem::getActiveRequests() {
    return activeRequests;
}

AllocationEngine* ParkingSystem::getEngine() const {
    return engine;
}

Zone* ParkingSystem::getZoneByID(int zoneID) const {
    if (engine != nullptr) {
        return engine->findZoneByID(zoneID);
    }
    return nullptr;
}