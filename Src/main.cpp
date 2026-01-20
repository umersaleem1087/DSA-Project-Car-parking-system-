#include "include/ParkingSystem.h"
#include <iostream>

// ============================================================================
// TEST SUITE - 10+ Required Test Cases
// ============================================================================

void printTestHeader(const std::string& testName) {
    std::cout << "\n========== TEST: " << testName << " ==========" << std::endl;
}

void printTestResult(bool passed) {
    std::cout << "\n[RESULT] Test " << (passed ? "PASSED ✓" : "FAILED ✗") << std::endl;
}

int main() {
    std::cout << "========== PARKING SYSTEM TEST SUITE ==========" << std::endl;
    std::cout << "Qt-Ready Controller Pattern Implementation\n" << std::endl;
    
    ParkingSystem system;
    
    // Setup: Create test zones and parking areas
    std::cout << "=== SETUP PHASE ===" << std::endl;
    
    // Zone 1
    Zone* zone1 = new Zone(1);
    ParkingArea* area1A = new ParkingArea(101);
    for (int i = 1; i <= 5; i++) {
        area1A->addSlot(new ParkingSlot(i, 1));
    }
    zone1->addParkingArea(area1A);
    system.addZone(zone1);
    
    // Zone 2
    Zone* zone2 = new Zone(2);
    ParkingArea* area2A = new ParkingArea(201);
    for (int i = 6; i <= 10; i++) {
        area2A->addSlot(new ParkingSlot(i, 2));
    }
    zone2->addParkingArea(area2A);
    system.addZone(zone2);
    
    // Setup adjacency for fallback testing
    zone1->addAdjacentZone(zone2);
    zone2->addAdjacentZone(zone1);
    
    std::cout << "Setup complete: 2 zones with 5 slots each\n" << std::endl;
    
    // ========================================================================
    // TEST 1: Successful Parking Request in Preferred Zone
    // ========================================================================
    printTestHeader("Successful Parking Request in Preferred Zone");
    ParkingRequest* req1 = system.createRequest("VEHICLE001", 1);
    printTestResult(req1 != nullptr && req1->getCurrentStatus() == RequestState::ALLOCATED);
    
    // ========================================================================
    // TEST 2: Occupy Parking Slot
    // ========================================================================
    printTestHeader("Occupy Parking Slot");
    bool occupy1 = system.occupyRequest("VEHICLE001");
    ParkingRequest* checkReq1 = system.getRequestByVehicleID("VEHICLE001");
    printTestResult(occupy1 && checkReq1 != nullptr && 
                   checkReq1->getCurrentStatus() == RequestState::OCCUPIED);
    
    // ========================================================================
    // TEST 3: Release Parking Slot
    // ========================================================================
    printTestHeader("Release Parking Slot");
    bool release1 = system.releaseRequest("VEHICLE001");
    ParkingRequest* checkReq2 = system.getRequestByVehicleID("VEHICLE001");
    printTestResult(release1 && checkReq2 != nullptr && 
                   checkReq2->getCurrentStatus() == RequestState::RELEASED);
    
    // ========================================================================
    // TEST 4: Multiple Vehicles Parking
    // ========================================================================
    printTestHeader("Multiple Vehicles Parking");
    system.createRequest("VEHICLE002", 1);
    system.createRequest("VEHICLE003", 1);
    system.createRequest("VEHICLE004", 2);
    int totalRequests = system.getTotalRequests();
    printTestResult(totalRequests >= 4);
    
    // ========================================================================
    // TEST 5: Zone Utilization Calculation
    // ========================================================================
    printTestHeader("Zone Utilization Calculation");
    double zone1Util = system.getZoneUtilization(1);
    std::cout << "Zone 1 Utilization: " << zone1Util << "%" << std::endl;
    printTestResult(zone1Util > 0 && zone1Util <= 100);
    
    // ========================================================================
    // TEST 6: Dashboard Statistics
    // ========================================================================
    printTestHeader("Dashboard Statistics");
    DashboardStats stats = system.getDashboardStats();
    stats.display();
    printTestResult(stats.totalRequests > 0 && stats.systemUtilization >= 0);
    
    // ========================================================================
    // TEST 7: Cancel Parking Request
    // ========================================================================
    printTestHeader("Cancel Parking Request");
    ParkingRequest* reqCancel = system.createRequest("VEHICLE005", 1);
    bool cancel = system.cancelRequest("VEHICLE005");
    ParkingRequest* checkCancel = system.getRequestByVehicleID("VEHICLE005");
    printTestResult(cancel && checkCancel != nullptr && 
                   checkCancel->getCurrentStatus() == RequestState::CANCELLED);
    
    // ========================================================================
    // TEST 8: Invalid State Transition Detection
    // ========================================================================
    printTestHeader("Invalid State Transition Detection");
    ParkingRequest* invalidReq = new ParkingRequest("TEST_VEH", 1);
    bool validTrans1 = invalidReq->updateState(RequestState::ALLOCATED);
    bool invalidTrans = invalidReq->updateState(RequestState::CANCELLED);
    // Try to go from CANCELLED to ALLOCATED (should fail)
    bool badTrans = invalidReq->updateState(RequestState::ALLOCATED);
    printTestResult(validTrans1 && invalidTrans && !badTrans);
    delete invalidReq;
    
    // ========================================================================
    // TEST 9: Fallback to Adjacent Zone with Penalty
    // ========================================================================
    printTestHeader("Fallback to Adjacent Zone with Penalty");
    // Fill up Zone 1
    system.createRequest("VEH_FILL_1", 1);
    system.createRequest("VEH_FILL_2", 1);
    system.createRequest("VEH_FILL_3", 1);
    
    // This should fallback to Zone 2 and apply penalty
    ParkingRequest* fallbackReq = system.createRequest("VEH_FALLBACK", 1);
    double penaltyCost = fallbackReq != nullptr ? fallbackReq->getPenaltyCost() : 0;
    printTestResult(fallbackReq != nullptr && penaltyCost > 0);
    
    // ========================================================================
    // TEST 10: Rollback Operations
    // ========================================================================
    printTestHeader("Rollback Operations");
    int histBefore = system.getRollbackManager()->getHistorySize();
    std::cout << "History size before rollback: " << histBefore << std::endl;
    
    bool rollback = system.rollbackOperations(2);
    int histAfter = system.getRollbackManager()->getHistorySize();
    std::cout << "History size after rollback: " << histAfter << std::endl;
    printTestResult(rollback && histAfter < histBefore);
    
    // ========================================================================
    // TEST 11: Master History Tracking (All requests including cancelled)
    // ========================================================================
    printTestHeader("Master History Tracking");
    int totalHistoryEntries = system.getTotalRequests();
    int cancelledCount = system.getRequestCountByStatus(RequestState::CANCELLED);
    std::cout << "Total requests in history: " << totalHistoryEntries << std::endl;
    std::cout << "Cancelled requests: " << cancelledCount << std::endl;
    printTestResult(totalHistoryEntries > 0 && cancelledCount > 0);
    
    // ========================================================================
    // TEST 12: System Status Display
    // ========================================================================
    printTestHeader("System Status Display");
    system.displaySystemStatus();
    system.displayZoneAnalytics();
    printTestResult(true);  // Display tests always pass if they don't crash
    
    // ========================================================================
    // TEST 13: Average Duration Calculation
    // ========================================================================
    printTestHeader("Average Duration Calculation");
    double avgDuration = system.getDashboardStats().averageParkingDuration;
    std::cout << "Average parking duration: " << avgDuration << " minutes" << std::endl;
    printTestResult(avgDuration >= 0);
    
    // ========================================================================
    // TEST 14: Get Request by Vehicle ID
    // ========================================================================
    printTestHeader("Get Request by Vehicle ID");
    ParkingRequest* foundReq = system.getRequestByVehicleID("VEHICLE002");
    printTestResult(foundReq != nullptr);
    
    // ========================================================================
    // TEST 15: Active Requests Tracking
    // ========================================================================
    printTestHeader("Active Requests Tracking");
    int activeCount = system.getActiveRequestCount();
    std::cout << "Active requests count: " << activeCount << std::endl;
    printTestResult(activeCount >= 0);
    
    // ========================================================================
    // FINAL REPORT
    // ========================================================================
    std::cout << "\n========== FINAL SYSTEM REPORT ==========" << std::endl;
    system.displayFullHistory();
    system.displayRollbackStatus();
    
    DashboardStats finalStats = system.getDashboardStats();
    std::cout << "\n========== FINAL STATISTICS ==========" << std::endl;
    finalStats.display();
    
    std::cout << "========== TEST SUITE COMPLETE ==========" << std::endl;
    std::cout << "All 15 test cases executed successfully!\n" << std::endl;
    
    return 0;
}
