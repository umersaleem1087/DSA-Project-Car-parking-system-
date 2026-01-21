#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QTabWidget>
#include <QTableWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

// Forward declarations to avoid MOC issues
class ParkingSystem;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Parking Request Slots
    void onCreateRequest();
    void onOccupyParking();
    void onReleaseParking();
    void onCancelRequest();
    
    // Analytics Slots
    void onRefreshDashboard();
    void onShowZoneAnalytics();
    void onShowFullHistory();
    
    // Rollback Slots
    void onRollbackOperations();
    void onShowRollbackStatus();

private:
    // Initialize UI
    void setupUI();
    void setupRequestTab(QWidget* requestTab, QVBoxLayout* layout);
    void setupDashboardTab(QWidget* dashboardTab, QVBoxLayout* layout);
    void setupHistoryTab(QWidget* historyTab, QVBoxLayout* layout);
    void setupAnalyticsTab(QWidget* analyticsTab, QVBoxLayout* layout);
    void setupRollbackTab(QWidget* rollbackTab, QVBoxLayout* layout);
    
    // Update UI
    void updateDashboard();
    void updateZoneList();
    void updateActiveRequestsList();
    void logMessage(const QString &message);
    
    // Parking System
    ParkingSystem* parkingSystem;
    
    // =====================================================
    // REQUEST MANAGEMENT TAB WIDGETS
    // =====================================================
    QLineEdit* vehicleIDInput;
    QComboBox* zoneComboBox;
    QPushButton* createRequestBtn;
    QPushButton* occupyBtn;
    QPushButton* releaseBtn;
    QPushButton* cancelBtn;
    QLineEdit* vehicleIDForOpsInput;
    QTextEdit* requestLogArea;
    
    // =====================================================
    // DASHBOARD TAB WIDGETS
    // =====================================================
    QLabel* totalRequestsLabel;
    QLabel* allocatedLabel;
    QLabel* occupiedLabel;
    QLabel* releasedLabel;
    QLabel* cancelledLabel;
    QLabel* averageDurationLabel;
    QLabel* utilizationLabel;
    QProgressBar* utilizationBar;
    QPushButton* refreshDashboardBtn;
    
    // =====================================================
    // HISTORY TAB WIDGETS
    // =====================================================
    QTableWidget* historyTable;
    QTextEdit* historyDetails;
    QPushButton* showFullHistoryBtn;
    
    // =====================================================
    // ANALYTICS TAB WIDGETS
    // =====================================================
    QTableWidget* zoneAnalyticsTable;
    QPushButton* showZoneAnalyticsBtn;
    
    // =====================================================
    // ROLLBACK TAB WIDGETS
    // =====================================================
    QSpinBox* rollbackCountSpinBox;
    QPushButton* rollbackBtn;
    QTextEdit* rollbackStatusArea;
    QPushButton* showRollbackStatusBtn;
};

#endif // MAINWINDOW_H
