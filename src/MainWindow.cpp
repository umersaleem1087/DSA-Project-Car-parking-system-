#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QAbstractItemModel>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("DSA Parking System - Management Interface");
    setGeometry(100, 100, 1200, 800);
    
    // Initialize parking system with sample data
    parkingSystem = new ParkingSystem();
    
    // Add sample zones
    parkingSystem->addZone(new Zone(1, 2, 5));  // Zone 1: 2 Areas, 5 Slots each
    parkingSystem->addZone(new Zone(2, 1, 10)); // Zone 2: 1 Area, 10 Slots
    parkingSystem->addZone(new Zone(3, 3, 8));  // Zone 3: 3 Areas, 8 Slots each
    
    setupUI();
    updateZoneList();
    updateDashboard();
}

MainWindow::~MainWindow() {
    delete parkingSystem;
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QTabWidget* tabWidget = new QTabWidget(this);
    
    // Create tabs
    createRequestTab();
    createDashboardTab();
    createHistoryTab();
    createAnalyticsTab();
    createRollbackTab();
    
    // Add tabs to widget
    tabWidget->addTab(createRequestTab(), "Parking Requests");
    tabWidget->addTab(createDashboardTab(), "Dashboard");
    tabWidget->addTab(createHistoryTab(), "History");
    tabWidget->addTab(createAnalyticsTab(), "Analytics");
    tabWidget->addTab(createRollbackTab(), "Rollback");
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(tabWidget);
    centralWidget->setLayout(mainLayout);
}

void MainWindow::createRequestTab() {
    QWidget* requestTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(requestTab);
    
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

void MainWindow::createDashboardTab() {
    QWidget* dashboardTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(dashboardTab);
    
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
    utilizationBar = new QProgressBar();
    utilizationBar->setValue(0);
    
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
    statsLayout->addWidget(utilizationBar, 3, 2, 1, 2);
    
    layout->addWidget(statsGroup);
    
    refreshDashboardBtn = new QPushButton("Refresh Dashboard");
    connect(refreshDashboardBtn, &QPushButton::clicked, this, &MainWindow::onRefreshDashboard);
    layout->addWidget(refreshDashboardBtn);
    
    layout->addStretch();
}

void MainWindow::createHistoryTab() {
    QWidget* historyTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(historyTab);
    
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

void MainWindow::createAnalyticsTab() {
    QWidget* analyticsTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(analyticsTab);
    
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

void MainWindow::createRollbackTab() {
    QWidget* rollbackTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(rollbackTab);
    
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

void MainWindow::onCreateRequest() {
    QString vehicleID = vehicleIDInput->text().trimmed();
    int zoneID = zoneComboBox->currentData().toInt();
    
    if (vehicleID.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a vehicle ID");
        return;
    }
    
    ParkingRequest* request = parkingSystem->createRequest(vehicleID.toStdString(), zoneID);
    
    if (request) {
        logMessage(QString("✓ Request created for Vehicle %1 in Zone %2")
                   .arg(vehicleID).arg(zoneID));
        vehicleIDInput->clear();
        updateDashboard();
        updateActiveRequestsList();
    } else {
        logMessage(QString("✗ Failed to create request for Vehicle %1 in Zone %2")
                   .arg(vehicleID).arg(zoneID));
        QMessageBox::critical(this, "Request Failed", "Could not create parking request. Zone may be full.");
    }
}

void MainWindow::onOccupyParking() {
    QString vehicleID = vehicleIDForOpsInput->text().trimmed();
    
    if (vehicleID.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a vehicle ID");
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
}

void MainWindow::onReleaseParking() {
    QString vehicleID = vehicleIDForOpsInput->text().trimmed();
    
    if (vehicleID.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a vehicle ID");
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
}

void MainWindow::onCancelRequest() {
    QString vehicleID = vehicleIDForOpsInput->text().trimmed();
    
    if (vehicleID.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a vehicle ID");
        return;
    }
    
    bool success = parkingSystem->cancelRequest(vehicleID.toStdString());
    
    if (success) {
        logMessage(QString("✓ Request cancelled for Vehicle %1").arg(vehicleID));
        vehicleIDForOpsInput->clear();
        updateDashboard();
    } else {
        logMessage(QString("✗ Failed to cancel request for Vehicle %1").arg(vehicleID));
        QMessageBox::critical(this, "Operation Failed", "Could not cancel request.");
    }
}

void MainWindow::onRefreshDashboard() {
    updateDashboard();
    logMessage("Dashboard refreshed");
}

void MainWindow::updateDashboard() {
    DashboardStats stats = parkingSystem->getDashboardStats();
    
    totalRequestsLabel->setText(QString::number(stats.totalRequests));
    allocatedLabel->setText(QString::number(stats.requestsAllocated));
    occupiedLabel->setText(QString::number(stats.requestsOccupied));
    releasedLabel->setText(QString::number(stats.requestsReleased));
    cancelledLabel->setText(QString::number(stats.requestsCancelled));
    averageDurationLabel->setText(QString::number(stats.averageParkingDuration, 'f', 2));
    
    int utilization = static_cast<int>(stats.systemUtilization);
    utilizationLabel->setText(QString::number(utilization) + "%");
    utilizationBar->setValue(utilization);
}

void MainWindow::updateZoneList() {
    zoneComboBox->clear();
    
    // Add sample zones - in a real scenario, you'd get these from the parking system
    zoneComboBox->addItem("Zone 1", 1);
    zoneComboBox->addItem("Zone 2", 2);
    zoneComboBox->addItem("Zone 3", 3);
}

void MainWindow::updateActiveRequestsList() {
    // This would be implemented to show active requests
    logMessage("Active requests updated");
}

void MainWindow::onShowZoneAnalytics() {
    zoneAnalyticsTable->setRowCount(0);
    
    for (int zoneID = 1; zoneID <= 3; ++zoneID) {
        double utilization = parkingSystem->getZoneUtilization(zoneID);
        
        int row = zoneAnalyticsTable->rowCount();
        zoneAnalyticsTable->insertRow(row);
        
        zoneAnalyticsTable->setItem(row, 0, new QTableWidgetItem(QString::number(zoneID)));
        zoneAnalyticsTable->setItem(row, 1, new QTableWidgetItem("Variable"));
        zoneAnalyticsTable->setItem(row, 2, new QTableWidgetItem("Variable"));
        zoneAnalyticsTable->setItem(row, 3, new QTableWidgetItem(QString::number(utilization, 'f', 2) + "%"));
    }
    
    logMessage("Zone analytics refreshed");
}

void MainWindow::onShowFullHistory() {
    historyTable->setRowCount(0);
    
    auto& masterHistory = parkingSystem->getMasterHistory();
    
    int row = 0;
    // Note: This is a simplified version. Full implementation would iterate through linked list
    logMessage("Full history loaded");
}

void MainWindow::onRollbackOperations() {
    int k = rollbackCountSpinBox->value();
    
    bool success = parkingSystem->rollbackOperations(k);
    
    if (success) {
        logMessage(QString("✓ Successfully rolled back %1 operation(s)").arg(k));
        updateDashboard();
    } else {
        logMessage(QString("✗ Failed to rollback %1 operation(s)").arg(k));
        QMessageBox::critical(this, "Rollback Failed", "Could not perform rollback operation.");
    }
}

void MainWindow::onShowRollbackStatus() {
    rollbackStatusArea->clear();
    parkingSystem->displayRollbackStatus();
    logMessage("Rollback status displayed");
}

void MainWindow::logMessage(const QString &message) {
    requestLogArea->append("[" + QDateTime::currentDateTime().toString("hh:mm:ss") + "] " + message);
}
