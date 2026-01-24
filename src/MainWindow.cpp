#include "MainWindow.h"
#include "ParkingSystem.h"
#include "Zone.h"
#include "ParkingRequest.h"
#include "ParkingArea.h"
#include "ParkingSlot.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QDateTime>
#include <QAbstractItemModel>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <vector>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), requestLogArea(nullptr), parkingSystem(nullptr) {
    try {
        setWindowTitle("DSA Parking System - Management Interface");
        setGeometry(100, 100, 1200, 800);
        
        // Initialize parking system with sample data
        parkingSystem = new ParkingSystem();
        
        // Add sample zones with areas and slots
        Zone* zone1 = new Zone(1);
        ParkingArea* area1 = new ParkingArea(101);
        for (int i = 1; i <= 10; i++) {
            area1->addSlot(new ParkingSlot(1000 + i, 1));
        }
        zone1->addParkingArea(area1);
        
        ParkingArea* area2 = new ParkingArea(102);
        for (int i = 1; i <= 10; i++) {
            area2->addSlot(new ParkingSlot(2000 + i, 1));
        }
        zone1->addParkingArea(area2);
        parkingSystem->addZone(zone1);
        
        Zone* zone2 = new Zone(2);
        ParkingArea* area3 = new ParkingArea(201);
        for (int i = 1; i <= 15; i++) {
            area3->addSlot(new ParkingSlot(3000 + i, 2));
        }
        zone2->addParkingArea(area3);
        
        ParkingArea* area4 = new ParkingArea(202);
        for (int i = 1; i <= 15; i++) {
            area4->addSlot(new ParkingSlot(4000 + i, 2));
        }
        zone2->addParkingArea(area4);
        parkingSystem->addZone(zone2);
        
        Zone* zone3 = new Zone(3);
        ParkingArea* area5 = new ParkingArea(301);
        for (int i = 1; i <= 20; i++) {
            area5->addSlot(new ParkingSlot(5000 + i, 3));
        }
        zone3->addParkingArea(area5);
        
        ParkingArea* area6 = new ParkingArea(302);
        for (int i = 1; i <= 20; i++) {
            area6->addSlot(new ParkingSlot(6000 + i, 3));
        }
        zone3->addParkingArea(area6);
        parkingSystem->addZone(zone3);
        
        setupUI();
        
        // Only call these if requestLogArea was successfully created
        if (requestLogArea != nullptr) {
            logMessage("System initialized successfully");
            updateZoneList();
            updateDashboard();
        }
    } catch (const std::exception& e) {
        qCritical() << "Fatal error in MainWindow constructor:" << QString::fromStdString(e.what());
    } catch (...) {
        qCritical() << "Unknown fatal error in MainWindow constructor";
    }
}

MainWindow::~MainWindow() {
    delete parkingSystem;
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QTabWidget* tabWidget = new QTabWidget(this);
    
    // Create tab widgets
    QWidget* requestTab = new QWidget();
    QWidget* dashboardTab = new QWidget();
    QWidget* historyTab = new QWidget();
    QWidget* analyticsTab = new QWidget();
    QWidget* rollbackTab = new QWidget();
    
    // Setup each tab
    QVBoxLayout* requestLayout = new QVBoxLayout(requestTab);
    setupRequestTab(requestTab, requestLayout);
    
    QVBoxLayout* dashboardLayout = new QVBoxLayout(dashboardTab);
    setupDashboardTab(dashboardTab, dashboardLayout);
    
    QVBoxLayout* historyLayout = new QVBoxLayout(historyTab);
    setupHistoryTab(historyTab, historyLayout);
    
    QVBoxLayout* analyticsLayout = new QVBoxLayout(analyticsTab);
    setupAnalyticsTab(analyticsTab, analyticsLayout);
    
    QVBoxLayout* rollbackLayout = new QVBoxLayout(rollbackTab);
    setupRollbackTab(rollbackTab, rollbackLayout);
    
    // Add tabs to widget
    tabWidget->addTab(requestTab, "Parking Requests");
    tabWidget->addTab(dashboardTab, "Dashboard");
    tabWidget->addTab(historyTab, "History");
    tabWidget->addTab(analyticsTab, "Analytics");
    tabWidget->addTab(rollbackTab, "Rollback");
    
    // Auto-refresh dashboard when tab is selected
    connect(tabWidget, QOverload<int>::of(&QTabWidget::currentChanged), 
            this, &MainWindow::onDashboardTabChanged);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(tabWidget);
    centralWidget->setLayout(mainLayout);
}

void MainWindow::setupRequestTab(QWidget* requestTab, QVBoxLayout* layout) {
    // Create Zone Section
    QGroupBox* createZoneGroup = new QGroupBox("Create New Zone", requestTab);
    QGridLayout* zoneLayout = new QGridLayout(createZoneGroup);
    
    QLabel* newZoneIDLabel = new QLabel("Zone ID:");
    newZoneIDSpinBox = new QSpinBox();
    newZoneIDSpinBox->setMinimum(1);
    newZoneIDSpinBox->setMaximum(100);
    newZoneIDSpinBox->setValue(1);
    
    QLabel* numSlotsLabel = new QLabel("Number of Slots:");
    numSlotsSpinBox = new QSpinBox();
    numSlotsSpinBox->setMinimum(1);
    numSlotsSpinBox->setMaximum(1000);
    numSlotsSpinBox->setValue(20);
    
    createZoneBtn = new QPushButton("Create Zone");
    connect(createZoneBtn, &QPushButton::clicked, this, &MainWindow::onCreateZone);
    
    zoneLayout->addWidget(newZoneIDLabel, 0, 0);
    zoneLayout->addWidget(newZoneIDSpinBox, 0, 1);
    zoneLayout->addWidget(numSlotsLabel, 1, 0);
    zoneLayout->addWidget(numSlotsSpinBox, 1, 1);
    zoneLayout->addWidget(createZoneBtn, 2, 0, 1, 2);
    
    layout->addWidget(createZoneGroup);
    
    // Create Request Section
    QGroupBox* createReqGroup = new QGroupBox("Create Parking Request", requestTab);
    QGridLayout* createLayout = new QGridLayout(createReqGroup);
    
    QLabel* vehicleLabel = new QLabel("Vehicle ID:");
    vehicleIDInput = new QLineEdit();
    vehicleIDInput->setPlaceholderText("Enter vehicle ID (e.g., V001)");
    
    QLabel* zoneLabel = new QLabel("Preferred Zone:");
    zoneComboBox = new QComboBox();
    
    createRequestBtn = new QPushButton("Create Request");
    connect(createRequestBtn, &QPushButton::clicked, this, &MainWindow::onCreateRequest);
    
    createLayout->addWidget(vehicleLabel, 0, 0);
    createLayout->addWidget(vehicleIDInput, 0, 1);
    createLayout->addWidget(zoneLabel, 1, 0);
    createLayout->addWidget(zoneComboBox, 1, 1);
    createLayout->addWidget(createRequestBtn, 2, 0, 1, 2);
    
    layout->addWidget(createReqGroup);
    
    // Parking Operations Section
    QGroupBox* operationsGroup = new QGroupBox("Parking Operations", requestTab);
    QGridLayout* opsLayout = new QGridLayout(operationsGroup);
    
    QLabel* vehicleOpsLabel = new QLabel("Vehicle ID:");
    vehicleIDForOpsInput = new QLineEdit();
    vehicleIDForOpsInput->setPlaceholderText("Enter vehicle ID");
    
    occupyBtn = new QPushButton("Occupy Parking Slot");
    releaseBtn = new QPushButton("Release Parking Slot");
    cancelBtn = new QPushButton("Cancel Request");
    
    connect(occupyBtn, &QPushButton::clicked, this, &MainWindow::onOccupyParking);
    connect(releaseBtn, &QPushButton::clicked, this, &MainWindow::onReleaseParking);
    connect(cancelBtn, &QPushButton::clicked, this, &MainWindow::onCancelRequest);
    
    opsLayout->addWidget(vehicleOpsLabel, 0, 0);
    opsLayout->addWidget(vehicleIDForOpsInput, 0, 1);
    opsLayout->addWidget(occupyBtn, 1, 0);
    opsLayout->addWidget(releaseBtn, 1, 1);
    opsLayout->addWidget(cancelBtn, 2, 0, 1, 2);
    
    layout->addWidget(operationsGroup);
    
    // Log Area
    QGroupBox* logGroup = new QGroupBox("Operation Log", requestTab);
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    
    requestLogArea = new QTextEdit();
    requestLogArea->setReadOnly(true);
    requestLogArea->setPlaceholderText("Operations log will appear here...");
    
    logLayout->addWidget(requestLogArea);
    layout->addWidget(logGroup);
    
    layout->addStretch();
}

void MainWindow::setupDashboardTab(QWidget* dashboardTab, QVBoxLayout* layout) {
    
    QGroupBox* statsGroup = new QGroupBox("System Statistics", dashboardTab);
    QGridLayout* statsLayout = new QGridLayout(statsGroup);
    
    // Statistics Labels
    QLabel* totalReqTitleLabel = new QLabel("Total Requests:");
    totalRequestsLabel = new QLabel("0");
    
    QLabel* allocatedTitleLabel = new QLabel("Allocated:");
    allocatedLabel = new QLabel("0");
    
    QLabel* occupiedTitleLabel = new QLabel("Occupied:");
    occupiedLabel = new QLabel("0");
    
    QLabel* releasedTitleLabel = new QLabel("Released:");
    releasedLabel = new QLabel("0");
    
    QLabel* cancelledTitleLabel = new QLabel("Cancelled:");
    cancelledLabel = new QLabel("0");
    
    QLabel* avgDurationTitleLabel = new QLabel("Avg Duration (min):");
    averageDurationLabel = new QLabel("0");
    
    QLabel* utilizationTitleLabel = new QLabel("System Utilization:");
    utilizationLabel = new QLabel("0%");
    
    statsLayout->addWidget(totalReqTitleLabel, 0, 0);
    statsLayout->addWidget(totalRequestsLabel, 0, 1);
    statsLayout->addWidget(allocatedTitleLabel, 0, 2);
    statsLayout->addWidget(allocatedLabel, 0, 3);
    
    statsLayout->addWidget(occupiedTitleLabel, 1, 0);
    statsLayout->addWidget(occupiedLabel, 1, 1);
    statsLayout->addWidget(releasedTitleLabel, 1, 2);
    statsLayout->addWidget(releasedLabel, 1, 3);
    
    statsLayout->addWidget(cancelledTitleLabel, 2, 0);
    statsLayout->addWidget(cancelledLabel, 2, 1);
    statsLayout->addWidget(avgDurationTitleLabel, 2, 2);
    statsLayout->addWidget(averageDurationLabel, 2, 3);
    
    statsLayout->addWidget(utilizationTitleLabel, 3, 0);
    statsLayout->addWidget(utilizationLabel, 3, 1);
    
    layout->addWidget(statsGroup);
    
    // Zone Availability Group
    QGroupBox* zoneAvailGroup = new QGroupBox("Available Slots by Zone/Area", dashboardTab);
    QVBoxLayout* zoneAvailLayout = new QVBoxLayout(zoneAvailGroup);
    
    zoneAvailabilityTable = new QTableWidget();
    zoneAvailabilityTable->setColumnCount(5);
    zoneAvailabilityTable->setHorizontalHeaderLabels(QStringList() << "Zone ID" << "Total Slots" << "Available" << "Occupied" << "Utilization %");
    zoneAvailabilityTable->horizontalHeader()->setStretchLastSection(true);
    zoneAvailabilityTable->setMaximumHeight(200);
    
    zoneAvailLayout->addWidget(zoneAvailabilityTable);
    layout->addWidget(zoneAvailGroup);
    
    refreshDashboardBtn = new QPushButton("Refresh Dashboard");
    connect(refreshDashboardBtn, &QPushButton::clicked, this, &MainWindow::onRefreshDashboard);
    layout->addWidget(refreshDashboardBtn);
    
    layout->addStretch();
}

void MainWindow::setupHistoryTab(QWidget* historyTab, QVBoxLayout* layout) {
    QGroupBox* historyGroup = new QGroupBox("Request History", historyTab);
    QVBoxLayout* historyLayout = new QVBoxLayout(historyGroup);
    
    historyTable = new QTableWidget();
    historyTable->setColumnCount(5);
    historyTable->setHorizontalHeaderLabels(QStringList() << "Request ID" << "Vehicle ID" << "Zone ID" << "Status" << "Timestamp");
    historyTable->horizontalHeader()->setStretchLastSection(true);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    historyLayout->addWidget(historyTable);
    
    showFullHistoryBtn = new QPushButton("Load Full History");
    connect(showFullHistoryBtn, &QPushButton::clicked, this, &MainWindow::onShowFullHistory);
    historyLayout->addWidget(showFullHistoryBtn);
    
    QGroupBox* detailsGroup = new QGroupBox("History Details", historyTab);
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsGroup);
    
    historyDetails = new QTextEdit();
    historyDetails->setReadOnly(true);
    detailsLayout->addWidget(historyDetails);
    
    layout->addWidget(historyGroup, 1);
    layout->addWidget(detailsGroup, 1);
}

void MainWindow::setupAnalyticsTab(QWidget* analyticsTab, QVBoxLayout* layout) {
    QGroupBox* analyticsGroup = new QGroupBox("Zone Analytics", analyticsTab);
    QVBoxLayout* analyticsLayout = new QVBoxLayout(analyticsGroup);
    
    zoneAnalyticsTable = new QTableWidget();
    zoneAnalyticsTable->setColumnCount(4);
    zoneAnalyticsTable->setHorizontalHeaderLabels(QStringList() << "Zone ID" << "Total Slots" << "Occupied" << "Utilization %");
    zoneAnalyticsTable->horizontalHeader()->setStretchLastSection(true);
    
    analyticsLayout->addWidget(zoneAnalyticsTable);
    
    showZoneAnalyticsBtn = new QPushButton("Refresh Zone Analytics");
    connect(showZoneAnalyticsBtn, &QPushButton::clicked, this, &MainWindow::onShowZoneAnalytics);
    analyticsLayout->addWidget(showZoneAnalyticsBtn);
    
    layout->addWidget(analyticsGroup);
    layout->addStretch();
}

void MainWindow::setupRollbackTab(QWidget* rollbackTab, QVBoxLayout* layout) {
    
    QGroupBox* rollbackGroup = new QGroupBox("Rollback Operations", rollbackTab);
    QGridLayout* rollbackLayout = new QGridLayout(rollbackGroup);
    
    QLabel* countLabel = new QLabel("Number of Operations to Undo:");
    rollbackCountSpinBox = new QSpinBox();
    rollbackCountSpinBox->setMinimum(1);
    rollbackCountSpinBox->setMaximum(100);
    rollbackCountSpinBox->setValue(1);
    
    rollbackBtn = new QPushButton("Perform Rollback");
    connect(rollbackBtn, &QPushButton::clicked, this, &MainWindow::onRollbackOperations);
    
    rollbackLayout->addWidget(countLabel, 0, 0);
    rollbackLayout->addWidget(rollbackCountSpinBox, 0, 1);
    rollbackLayout->addWidget(rollbackBtn, 1, 0, 1, 2);
    
    layout->addWidget(rollbackGroup);
    
    QGroupBox* statusGroup = new QGroupBox("Rollback Status", rollbackTab);
    QVBoxLayout* statusLayout = new QVBoxLayout(statusGroup);
    
    rollbackStatusArea = new QTextEdit();
    rollbackStatusArea->setReadOnly(true);
    statusLayout->addWidget(rollbackStatusArea);
    
    showRollbackStatusBtn = new QPushButton("Show Rollback Status");
    connect(showRollbackStatusBtn, &QPushButton::clicked, this, &MainWindow::onShowRollbackStatus);
    statusLayout->addWidget(showRollbackStatusBtn);
    
    layout->addWidget(statusGroup);
    layout->addStretch();
}

void MainWindow::onCreateZone() {
    try {
        int zoneID = newZoneIDSpinBox->value();
        int numSlots = numSlotsSpinBox->value();
        
        if (zoneID <= 0 || numSlots <= 0) {
            QMessageBox::warning(this, "Input Error", "Please enter valid Zone ID and number of slots");
            return;
        }
        
        // Create the zone with the specified ID and allocate slots
        Zone* newZone = new Zone(zoneID);
        ParkingArea* area = new ParkingArea(zoneID * 100);
        
        for (int i = 1; i <= numSlots; i++) {
            area->addSlot(new ParkingSlot(zoneID * 1000 + i, zoneID));
        }
        
        newZone->addParkingArea(area);
        parkingSystem->addZone(newZone);
        
        logMessage(QString("✓ Zone %1 created with %2 parking slots").arg(zoneID).arg(numSlots));
        QMessageBox::information(this, "Success", QString("Zone %1 created successfully with %2 slots.").arg(zoneID).arg(numSlots));
        
        // Refresh UI
        updateZoneList();
        updateDashboard();
    } catch (const std::exception& e) {
        logMessage(QString("EXCEPTION: %1").arg(QString::fromStdString(e.what())));
        QMessageBox::critical(this, "Error", QString("Error: %1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        logMessage("EXCEPTION: Unknown error");
        QMessageBox::critical(this, "Error", "Unknown error");
    }
}

void MainWindow::onCreateRequest() {
    try {
        QString vehicleID = vehicleIDInput->text().trimmed();
        int zoneID = zoneComboBox->currentData().toInt();
        
        if (vehicleID.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter a vehicle ID");
            return;
        }
        
        if (zoneID <= 0) {
            QMessageBox::warning(this, "Zone Error", "Please select a valid zone from the dropdown");
            return;
        }
        
        ParkingRequest* request = parkingSystem->createRequest(vehicleID.toStdString(), zoneID);
        
        if (request) {
            bool allocated = parkingSystem->allocateSlotForRequest(vehicleID.toStdString());
            
            if (allocated) {
                logMessage(QString("✓ Vehicle %1 allocated a slot in Zone %2").arg(vehicleID).arg(zoneID));
                QMessageBox::information(this, "Success", QString("Vehicle %1 allocated a slot in Zone %2.").arg(vehicleID).arg(zoneID));
            } else {
                logMessage(QString("✗ No available slots in Zone %1 for Vehicle %2").arg(zoneID).arg(vehicleID));
                QMessageBox::warning(this, "Allocation Failed", 
                                   QString("No parking slots available in Zone %1.").arg(zoneID));
            }
            
            vehicleIDInput->clear();
            updateActiveRequestsList();
            updateDashboard();
        } else {
            QMessageBox::critical(this, "Error", "Failed to create parking request.");
        }
        
    } catch (const std::exception& e) {
        logMessage(QString("EXCEPTION CAUGHT: %1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        logMessage("EXCEPTION CAUGHT: Unknown error");
    }
}

void MainWindow::onOccupyParking() {
    try {
        QString vehicleID = vehicleIDForOpsInput->text().trimmed();
        
        if (vehicleID.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter a vehicle ID");
            return;
        }
        
        // Check if vehicle has an active request
        ParkingRequest* request = parkingSystem->getRequestByVehicleID(vehicleID.toStdString());
        if (!request) {
            QMessageBox::warning(this, "Vehicle Not Found", 
                QString("Vehicle %1 does not have a parking request in the system.").arg(vehicleID));
            logMessage(QString("✗ Vehicle %1 not found in system").arg(vehicleID));
            return;
        }
        
        // Check if request is in ALLOCATED state
        if (request->getCurrentStatus() != RequestState::ALLOCATED) {
            std::string statusStr = request->statusToString(request->getCurrentStatus());
            QString errorMsg = QString("Vehicle %1 request is not in ALLOCATED state.\n\n"
                                      "Current Status: %2\n\n"
                                      "A vehicle must have an allocated slot before occupying it.")
                                .arg(vehicleID, QString::fromStdString(statusStr));
            QMessageBox::warning(this, "Cannot Occupy Slot", errorMsg);
            logMessage(QString("✗ Cannot occupy for Vehicle %1 - Status: %2")
                      .arg(vehicleID, QString::fromStdString(statusStr)));
            return;
        }
        
        bool success = parkingSystem->occupyRequest(vehicleID.toStdString());
        
        if (success) {
            logMessage(QString("✓ Vehicle %1 occupied parking slot").arg(vehicleID));
            vehicleIDForOpsInput->clear();
            updateDashboard();
        } else {
            logMessage(QString("✗ Failed to occupy parking slot for Vehicle %1").arg(vehicleID));
            QMessageBox::critical(this, "Operation Failed", "Could not occupy parking slot.");
        }
    } catch (const std::exception& e) {
        logMessage(QString("ERROR in occupy: %1").arg(QString::fromStdString(e.what())));
        QMessageBox::critical(this, "Error", QString("Error: %1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        logMessage("ERROR: Unknown exception in occupy");
        QMessageBox::critical(this, "Error", "Unknown error");
    }
}

void MainWindow::onReleaseParking() {
    try {
        QString vehicleID = vehicleIDForOpsInput->text().trimmed();
        
        if (vehicleID.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter a vehicle ID");
            return;
        }
        
        // Check if vehicle exists in the system
        ParkingRequest* request = parkingSystem->getRequestByVehicleID(vehicleID.toStdString());
        if (!request) {
            QMessageBox::warning(this, "Vehicle Not Found", 
                QString("Vehicle %1 does not have an active parking request in the system.").arg(vehicleID));
            logMessage(QString("✗ Vehicle %1 not found in system").arg(vehicleID));
            return;
        }
        
        // Check if vehicle is actually occupying a slot
        if (request->getCurrentStatus() != RequestState::OCCUPIED) {
            std::string statusStr = request->statusToString(request->getCurrentStatus());
            QString errorMsg = QString("Vehicle %1 is not currently occupying a parking slot.\n\n"
                                      "Current Status: %2\n\n"
                                      "Only vehicles with OCCUPIED status can be released.")
                                .arg(vehicleID, QString::fromStdString(statusStr));
            QMessageBox::warning(this, "Cannot Release Vehicle", errorMsg);
            logMessage(QString("✗ Cannot release Vehicle %1 - Status: %2")
                      .arg(vehicleID, QString::fromStdString(statusStr)));
            return;
        }
        
        bool success = parkingSystem->releaseRequest(vehicleID.toStdString());
        
        if (success) {
            logMessage(QString("✓ Vehicle %1 released parking slot").arg(vehicleID));
            vehicleIDForOpsInput->clear();
            updateDashboard();
        } else {
            logMessage(QString("✗ Failed to release parking slot for Vehicle %1").arg(vehicleID));
            QMessageBox::critical(this, "Operation Failed", "Could not release parking slot.");
        }
    } catch (const std::exception& e) {
        logMessage(QString("ERROR in release: %1").arg(QString::fromStdString(e.what())));
        QMessageBox::critical(this, "Error", QString("Error: %1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        logMessage("ERROR: Unknown exception in release");
        QMessageBox::critical(this, "Error", "Unknown error");
    }
}

void MainWindow::onCancelRequest() {
    try {
        QString vehicleID = vehicleIDForOpsInput->text().trimmed();
        
        if (vehicleID.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter a vehicle ID");
            return;
        }
        
        // Check if vehicle exists before canceling
        ParkingRequest* request = parkingSystem->getRequestByVehicleID(vehicleID.toStdString());
        if (!request) {
            QMessageBox::warning(this, "Vehicle Not Found", 
                QString("Vehicle %1 does not have a parking request in the system.").arg(vehicleID));
            logMessage(QString("✗ Vehicle %1 not found in system").arg(vehicleID));
            return;
        }
        
        // Check if already cancelled or released
        RequestState status = request->getCurrentStatus();
        if (status == RequestState::CANCELLED || status == RequestState::RELEASED) {
            std::string statusStr = request->statusToString(status);
            QString errorMsg = QString("Vehicle %1 request has already been %2!\n\n"
                                      "Cancelled vehicles are permanently removed from the system.")
                                .arg(vehicleID, QString::fromStdString(statusStr).toLower());
            QMessageBox::warning(this, "Cannot Cancel", errorMsg);
            logMessage(QString("✗ Vehicle %1 cannot be cancelled - already %2").arg(vehicleID, QString::fromStdString(statusStr).toLower()));
            return;
        }
        
        bool success = parkingSystem->cancelRequest(vehicleID.toStdString());
        
        if (success) {
            logMessage(QString("✓ Request cancelled for Vehicle %1 - Removed from system").arg(vehicleID));
            QMessageBox::information(this, "Request Cancelled", 
                QString("Vehicle %1 request has been cancelled.\n\nThe vehicle has been permanently removed from the system and cannot be used again.").arg(vehicleID));
            vehicleIDForOpsInput->clear();
            updateDashboard();
            updateActiveRequestsList();
        } else {
            logMessage(QString("✗ Failed to cancel request for Vehicle %1").arg(vehicleID));
            QMessageBox::critical(this, "Operation Failed", "Could not cancel request.");
        }
    } catch (const std::exception& e) {
        logMessage(QString("ERROR in cancel: %1").arg(QString::fromStdString(e.what())));
        QMessageBox::critical(this, "Error", QString("Error: %1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        logMessage("ERROR: Unknown exception in cancel");
        QMessageBox::critical(this, "Error", "Unknown error");
    }
}

void MainWindow::onRefreshDashboard() {
    try {
        updateDashboard();
    } catch (const std::exception& e) {
        logMessage(QString("ERROR refreshing dashboard: %1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        logMessage("ERROR: Unknown exception refreshing dashboard");
    }
}

void MainWindow::onDashboardTabChanged(int index) {
    try {
        qDebug() << "Tab changed to index:" << index;
        // Don't auto-update on tab change - let user click refresh button
        // This prevents crashes from rapid tab switching
    } catch (const std::exception& e) {
        qWarning() << "Error in tab change:" << QString::fromStdString(e.what());
    } catch (...) {
        qWarning() << "Unknown error in tab change";
    }
}

void MainWindow::updateDashboard() {
    try {
        if (!parkingSystem) {
            qWarning() << "updateDashboard: parkingSystem is null";
            return;
        }
        
        DashboardStats stats = parkingSystem->getDashboardStats();
        
        if (totalRequestsLabel) totalRequestsLabel->setText(QString::number(stats.totalRequests));
        if (allocatedLabel) allocatedLabel->setText(QString::number(stats.requestsAllocated));
        if (occupiedLabel) occupiedLabel->setText(QString::number(stats.actualOccupiedSlots));
        if (releasedLabel) releasedLabel->setText(QString::number(stats.requestsReleased));
        if (cancelledLabel) cancelledLabel->setText(QString::number(stats.requestsCancelled));
        if (averageDurationLabel) averageDurationLabel->setText(QString::number(stats.averageParkingDuration, 'f', 2));
        
        int utilization = static_cast<int>(stats.systemUtilization);
        if (utilizationLabel) utilizationLabel->setText(QString::number(utilization) + "%");
        
        // Populate zone availability table
        if (zoneAvailabilityTable) {
            zoneAvailabilityTable->setRowCount(0);
            auto zoneNode = stats.zoneStatuses.getHead();
            int row = 0;
            int maxRows = 1000;  // Safety limit to prevent infinite loop
            while (zoneNode != nullptr && row < maxRows) {
                ZoneSlotStatus zoneStatus = zoneNode->data;
                
                zoneAvailabilityTable->insertRow(row);
                zoneAvailabilityTable->setItem(row, 0, new QTableWidgetItem(QString::number(zoneStatus.zoneID)));
                zoneAvailabilityTable->setItem(row, 1, new QTableWidgetItem(QString::number(zoneStatus.totalSlots)));
                zoneAvailabilityTable->setItem(row, 2, new QTableWidgetItem(QString::number(zoneStatus.availableSlots)));
                zoneAvailabilityTable->setItem(row, 3, new QTableWidgetItem(QString::number(zoneStatus.occupiedSlots)));
                zoneAvailabilityTable->setItem(row, 4, new QTableWidgetItem(QString::number(zoneStatus.utilization, 'f', 1) + "%"));
                
                row++;
                zoneNode = zoneNode->next;
            }
            if (row >= maxRows) {
                qWarning() << "Zone table update exceeded max rows - possible infinite loop!";
            }
        }
    } catch (const std::exception& e) {
        qWarning() << "Error updating dashboard:" << QString::fromStdString(e.what());
    } catch (...) {
        qWarning() << "Unknown error updating dashboard";
    }
}

void MainWindow::updateZoneList() {
    try {
        if (!zoneComboBox) return;
        
        zoneComboBox->blockSignals(true);
        zoneComboBox->clear();
        zoneComboBox->addItem("Select Zone...", -1);
        
        // Get zones from parking system
        if (!parkingSystem || !parkingSystem->getEngine()) {
            zoneComboBox->blockSignals(false);
            return;
        }
        
        // Use the same defensive approach as getDashboardStats
        auto& zonesList = parkingSystem->getEngine()->getAllZones();
        
        // Create a safe copy of zone pointers to avoid list corruption
        std::vector<Zone*> zonePtrs;
        auto tempNode = zonesList.getHead();
        int tempCount = 0;
        
        while (tempNode != nullptr && tempCount < 10000) {
            // Validate pointer
            if ((intptr_t)tempNode < 0x1000 || (intptr_t)tempNode > 0x7FFFFFFF0000) {
                break;
            }
            
            if (tempNode->data != nullptr) {
                if ((intptr_t)(tempNode->data) >= 0x1000 && (intptr_t)(tempNode->data) <= 0x7FFFFFFF0000) {
                    zonePtrs.push_back(tempNode->data);
                }
            }
            
            auto nextPtr = tempNode->next;
            tempNode = nextPtr;
            tempCount++;
        }
        
        // Now populate combo box from the safe copy
        for (Zone* zone : zonePtrs) {
            try {
                int id = zone->getZoneID();
                if (id < 1 || id > 999) continue;
                
                int capacity = zone->getTotalCapacity();
                if (capacity <= 0 || capacity > 100000) continue;
                
                zoneComboBox->addItem(QString("Zone %1 (%2 slots)").arg(id).arg(capacity), id);
            } catch (...) {
                // Skip problematic zones
            }
        }
        
        zoneComboBox->blockSignals(false);
    } catch (const std::exception& e) {
        qWarning() << "Error loading zones:" << QString::fromStdString(e.what());
    } catch (...) {
        qWarning() << "Unknown error loading zones";
    }
}

void MainWindow::updateActiveRequestsList() {
    // This would be implemented to show active requests
    logMessage("Active requests updated");
}

void MainWindow::onShowZoneAnalytics() {
    try {
        logMessage("=== onShowZoneAnalytics START ===");
        std::cout.flush();
        
        if (!zoneAnalyticsTable) {
            logMessage("ERROR: zoneAnalyticsTable is null");
            return;
        }
        
        logMessage("Clearing table...");
        zoneAnalyticsTable->setRowCount(0);
        logMessage("Table cleared");
        
        DashboardStats stats = parkingSystem->getDashboardStats();
        
        logMessage(QString("Got %1 zones").arg(stats.zoneStatuses.getSize()));
        
        // Set header if not already set
        if (zoneAnalyticsTable->columnCount() < 5) {
            logMessage("Setting table headers...");
            zoneAnalyticsTable->setColumnCount(5);
            QStringList headers = {"Zone ID", "Total Slots", "Available", "Occupied", "Utilization %"};
            zoneAnalyticsTable->setHorizontalHeaderLabels(headers);
            logMessage("Headers set");
        }
        
        logMessage("Populating analytics table...");
        std::cout.flush();
        // Populate with actual zone data
        auto zoneNode = stats.zoneStatuses.getHead();
        int row = 0;
        int maxRows = 1000;
        
        while (zoneNode != nullptr && row < maxRows) {
            logMessage(QString("Processing row %1...").arg(row));
            ZoneSlotStatus zoneStatus = zoneNode->data;
            
            zoneAnalyticsTable->insertRow(row);
            zoneAnalyticsTable->setItem(row, 0, new QTableWidgetItem(QString::number(zoneStatus.zoneID)));
            zoneAnalyticsTable->setItem(row, 1, new QTableWidgetItem(QString::number(zoneStatus.totalSlots)));
            zoneAnalyticsTable->setItem(row, 2, new QTableWidgetItem(QString::number(zoneStatus.availableSlots)));
            zoneAnalyticsTable->setItem(row, 3, new QTableWidgetItem(QString::number(zoneStatus.occupiedSlots)));
            zoneAnalyticsTable->setItem(row, 4, new QTableWidgetItem(QString::number(zoneStatus.utilization, 'f', 1) + "%"));
            
            row++;
            zoneNode = zoneNode->next;
        }
        
        logMessage(QString("=== Zone analytics complete - %1 zones displayed ===").arg(row));
        std::cout.flush();
        
    } catch (const std::exception& e) {
        logMessage(QString("EXCEPTION in onShowZoneAnalytics: %1").arg(QString::fromStdString(e.what())));
        QMessageBox::critical(this, "Error", QString("Error loading analytics: %1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        logMessage("EXCEPTION in onShowZoneAnalytics: Unknown error");
        QMessageBox::critical(this, "Error", "Unknown error loading analytics");
    }
}

void MainWindow::onShowFullHistory() {
    try {
        if (!historyTable) {
            logMessage("ERROR: historyTable is null");
            return;
        }
        
        historyTable->setRowCount(0);
        
        logMessage("Getting master history...");
        auto& masterHistory = parkingSystem->getMasterHistory();
        
        logMessage("🔍 Checking master history...");
        
        int row = 0;
        int requestID = 1;
        int maxRows = 10000;
        // Iterate through all requests in master history
        auto currentNode = masterHistory.getHead();
        
        if (currentNode == nullptr) {
            logMessage("⚠️ Master history is empty - no requests created yet");
            return;
        }
        
        while (currentNode != nullptr && row < maxRows) {
            if (currentNode->data != nullptr) {
                ParkingRequest* req = currentNode->data;
                
                // Insert row
                historyTable->insertRow(row);
                
                // Column 0: Request ID
                QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(requestID));
                historyTable->setItem(row, 0, idItem);
                
                // Column 1: Vehicle ID
                QTableWidgetItem* vehicleItem = new QTableWidgetItem(QString::fromStdString(req->getVehicleID()));
                historyTable->setItem(row, 1, vehicleItem);
                
                // Column 2: Zone ID
                QTableWidgetItem* zoneItem = new QTableWidgetItem(QString::number(req->getRequestedZoneID()));
                historyTable->setItem(row, 2, zoneItem);
                
                // Column 3: Status
                QTableWidgetItem* statusItem = new QTableWidgetItem(QString::fromStdString(req->statusToString(req->getCurrentStatus())));
                historyTable->setItem(row, 3, statusItem);
                
                // Column 4: Timestamp
                QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                QTableWidgetItem* timeItem = new QTableWidgetItem(timestamp);
                historyTable->setItem(row, 4, timeItem);
                
                logMessage(QString("  Row %1: Vehicle=%2, Zone=%3, Status=%4").arg(row).arg(QString::fromStdString(req->getVehicleID())).arg(req->getRequestedZoneID()).arg(QString::fromStdString(req->statusToString(req->getCurrentStatus()))));
                
                requestID++;
                row++;
            }
            currentNode = currentNode->next;
        }
        
        logMessage(QString("✓ Full history loaded - %1 request(s) displayed").arg(row));
    } catch (const std::exception& e) {
        logMessage(QString("EXCEPTION in onShowFullHistory: %1").arg(QString::fromStdString(e.what())));
        QMessageBox::critical(this, "Error", QString("Error loading history: %1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        logMessage("EXCEPTION in onShowFullHistory: Unknown error");
        QMessageBox::critical(this, "Error", "Unknown error loading history");
    }
}

void MainWindow::onRollbackOperations() {
    int k = rollbackCountSpinBox->value();
    
    bool success = parkingSystem->rollbackOperations(k);
    
    if (success) {
        logMessage(QString("✓ Successfully rolled back %1 operation(s)").arg(k));
        updateActiveRequestsList();  // Refresh the active requests display
        updateDashboard();             // Refresh the dashboard
    } else {
        logMessage(QString("✗ Failed to rollback %1 operation(s)").arg(k));
        QMessageBox::critical(this, "Rollback Failed", "Could not perform rollback operation.");
    }
}

void MainWindow::onShowRollbackStatus() {
    rollbackStatusArea->clear();
    
    // Get rollback manager info
    int totalRollbacks = parkingSystem->getRollbackManager()->getTotalRollbacksPerformed();
    int historySize = parkingSystem->getRollbackManager()->getHistorySize();
    bool hasHistory = parkingSystem->getRollbackManager()->hasHistory();
    
    QString status;
    status += "═══════════════════════════════════════\n";
    status += "        ROLLBACK STATUS\n";
    status += "═══════════════════════════════════════\n\n";
    
    status += QString("Total Rollbacks Performed: %1\n").arg(totalRollbacks);
    status += QString("Available Operations in History: %1\n").arg(historySize);
    status += QString("History Available: %1\n\n").arg(hasHistory ? "Yes ✓" : "No ✗");
    
    // Display active requests that can be rolled back
    auto& activeRequests = parkingSystem->getActiveRequests();
    int activeCount = activeRequests.getSize();
    
    status += QString("Active Requests: %1\n").arg(activeCount);
    status += "─────────────────────────────────────\n";
    
    if (activeCount == 0) {
        status += "No active requests in system\n";
    } else {
        status += "Vehicles in System:\n";
        auto node = activeRequests.getHead();
        int count = 1;
        while (node != nullptr) {
            if (node->data != nullptr) {
                ParkingRequest* req = node->data;
                status += QString("  %1. Vehicle: %2 | Status: %3 | Zone: %4\n")
                    .arg(count)
                    .arg(QString::fromStdString(req->getVehicleID()))
                    .arg(QString::fromStdString(req->statusToString(req->getCurrentStatus())))
                    .arg(req->getRequestedZoneID());
                count++;
            }
            node = node->next;
        }
    }
    
    status += "\n═══════════════════════════════════════\n";
    rollbackStatusArea->setText(status);
    logMessage("Rollback status displayed");
}

void MainWindow::logMessage(const QString &message) {
    if (requestLogArea == nullptr) {
        qWarning() << "Log area not initialized:" << message;
        return;
    }
    try {
        requestLogArea->append("[" + QDateTime::currentDateTime().toString("hh:mm:ss") + "] " + message);
    } catch (...) {
        qWarning() << "Error logging message:" << message;
    }
}
