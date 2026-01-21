# DSA Parking System - Qt UI

This branch contains the Qt-based graphical user interface for the DSA Parking System.

## Features

The Qt UI provides the following functionality:

### 1. Parking Requests Management
- **Create Request**: Add a new parking request for a vehicle specifying preferred zone
- **Occupy Parking Slot**: Mark a vehicle as occupying an allocated slot
- **Release Parking Slot**: Mark a vehicle as departing the parking system
- **Cancel Request**: Cancel an existing parking request

### 2. Dashboard
- Real-time system statistics including:
  - Total requests processed
  - Requests allocated
  - Slots currently occupied
  - Requests released
  - Requests cancelled
  - Average parking duration
  - System utilization percentage
- Visual utilization bar

### 3. History Management
- View all parking requests history
- Track request lifecycle from creation to completion
- Detailed request information display

### 4. Zone Analytics
- View statistics for each parking zone
- Monitor zone utilization rates
- Track occupied vs available slots per zone
- Real-time analytics refresh

### 5. Rollback Operations
- Undo multiple operations at once
- Configurable rollback depth (1-100 operations)
- View rollback operation status
- Maintain system consistency

## System Architecture

### Zones Configuration (Default)
- **Zone 1**: 2 Areas with 5 slots each (10 total slots)
- **Zone 2**: 1 Area with 10 slots
- **Zone 3**: 3 Areas with 8 slots each (24 total slots)

**Total System Capacity**: 44 parking slots

### Core Components

1. **ParkingSystem**: Main controller managing the parking system
2. **AllocationEngine**: Handles parking space allocation algorithms
3. **RollbackManager**: Manages undo/rollback operations using stack-based approach
4. **Vehicle**: Represents a vehicle entity
5. **ParkingArea**: Represents a parking area within a zone
6. **ParkingSlot**: Individual parking slots
7. **Zone**: Groups areas and slots geographically
8. **ParkingRequest**: Tracks individual parking requests

### Data Structures
- **LinkedList**: Used for maintaining request history (doubly-linked)
- **Stack**: Used for rollback operations

## Building the Project

### Prerequisites
- Qt 5.15+ or Qt 6.x
- C++17 compatible compiler
- CMake 3.16+ or Qt Creator

### Option 1: Using Qt Creator
1. Open `ParkingSystemUI.pro` in Qt Creator
2. Configure the project
3. Build and run

### Option 2: Using CMake
```bash
mkdir build
cd build
cmake ..
make
./ParkingSystemUI
```

### Option 3: Using qmake
```bash
qmake ParkingSystemUI.pro
make
```

## Usage Guide

### Creating a Parking Request
1. Navigate to "Parking Requests" tab
2. Enter Vehicle ID (e.g., V001, CAR123)
3. Select preferred Zone from dropdown
4. Click "Create Request"
5. Check operation log for confirmation

### Occupying a Parking Slot
1. Enter the Vehicle ID
2. Click "Occupy Parking Slot"
3. System will verify allocation and mark as occupied

### Releasing a Parking Slot
1. Enter the Vehicle ID currently parked
2. Click "Release Parking Slot"
3. Vehicle departure is recorded and statistics updated

### Cancelling a Request
1. Enter the Vehicle ID
2. Click "Cancel Request"
3. Request is removed from active list

### Monitoring Dashboard
1. Navigate to "Dashboard" tab
2. View real-time statistics
3. Click "Refresh Dashboard" to update metrics

### Viewing History
1. Navigate to "History" tab
2. Click "Load Full History" to populate table
3. Select entries to view detailed information

### Checking Zone Analytics
1. Navigate to "Analytics" tab
2. Click "Refresh Zone Analytics"
3. View utilization rates and slot information per zone

### Performing Rollback
1. Navigate to "Rollback" tab
2. Specify number of operations to undo (1-100)
3. Click "Perform Rollback"
4. View status and confirmation in log area

## File Structure

```
├── include/
│   ├── MainWindow.h              # Qt main window header
│   ├── ParkingSystem.h           # Core system
│   ├── AllocationEngine.h        # Allocation logic
│   ├── Common.h                  # Common definitions
│   ├── LinkedList.h              # Generic linked list template
│   ├── Node.h                    # Linked list node
│   ├── ParkingArea.h             # Area abstraction
│   ├── ParkingRequest.h          # Request entity
│   ├── ParkingSlot.h             # Individual slot
│   ├── RollbackManager.h         # Rollback operations
│   ├── Stack.h                   # Stack template
│   ├── Vehicle.h                 # Vehicle entity
│   └── Zone.h                    # Zone entity
├── src/
│   ├── main-qt.cpp               # Qt application entry point
│   ├── MainWindow.cpp            # Qt UI implementation
│   ├── ParkingSystem.cpp         # Core system implementation
│   ├── AllocationEngine.cpp      # Allocation implementation
│   └── [other implementations]
├── ParkingSystemUI.pro           # Qt project file
├── CMakeLists.txt                # CMake build configuration
└── UI_README.md                  # This file
```

## Statistics Calculation

### System Utilization
Calculated as: (Total Occupied Slots / Total Available Slots) × 100%

### Zone Utilization
Calculated per zone as: (Occupied Slots in Zone / Total Slots in Zone) × 100%

### Average Parking Duration
Average time vehicles remain in the system (calculated from released requests)

## Error Handling

- **Invalid Vehicle ID**: User is prompted to enter a valid ID
- **Full Zone**: Request is rejected if no slots available
- **Invalid Operation**: System validates request state before operations
- **Rollback Limits**: Maximum 100 operations can be rolled back

## Threading Considerations

The current implementation is single-threaded. For production use with high traffic:
- Implement thread-safe parking system
- Use mutex locks for concurrent access
- Consider async operations for database integration

## Future Enhancements

- [ ] Multi-threaded request processing
- [ ] Database integration for persistence
- [ ] Advanced analytics and reporting
- [ ] Vehicle type-based slot assignment
- [ ] Time-based pricing integration
- [ ] Real-time notifications
- [ ] Export reports to PDF/Excel
- [ ] Mobile app companion

## Testing

To test basic functionality:
1. Create multiple parking requests with different vehicles and zones
2. Occupy and release slots
3. Monitor dashboard statistics
4. Perform rollback operations
5. Verify zone analytics accuracy

## Notes

- All timestamps are in local system time
- Vehicle IDs should be unique
- Zone IDs range from 1 to 3 in default configuration
- Operation log maintains history for current session

## Support

For issues or feature requests, please refer to the main project documentation.
