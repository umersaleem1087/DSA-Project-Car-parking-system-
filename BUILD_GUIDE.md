# Qt UI Build and Compilation Guide

## Quick Start

This guide helps you build and run the Parking System Qt UI on Windows.

## Prerequisites Check

Before building, ensure you have:

1. **Qt Framework** (5.15+ or 6.x)
   - Download from: https://www.qt.io/download
   - Recommended: Qt Creator IDE included

2. **C++ Compiler**
   - MSVC (Visual Studio 2019+) - Recommended for Windows
   - MinGW (included with Qt)
   - Clang

3. **Build Tools**
   - CMake 3.16+ (or use qmake which comes with Qt)
   - Git (already in use)

## Installation Steps

### Step 1: Install Qt (Windows)

1. Go to https://www.qt.io/download-open-source
2. Download Qt Online Installer
3. Run installer and select:
   - Qt 6.x (latest LTS) or Qt 5.15 LTS
   - MSVC compiler for your Visual Studio version
   - Qt Creator

### Step 2: Verify Installation

Open PowerShell and run:
```powershell
qmake -version
```

Expected output:
```
QMake version 3.x
Using Qt version x.x.x in path\to\Qt\...
```

## Building Methods

### Method 1: Using Qt Creator (Easiest)

1. Open Qt Creator
2. File → Open File or Project
3. Navigate to: `f:\VS Code ALL Files\Dsa Project\DSA-Project-Car-parking-system-\ParkingSystemUI.pro`
4. Select your kit (MSVC or MinGW)
5. Configure the project
6. Click "Build" or press Ctrl+B
7. Click "Run" or press Ctrl+R

### Method 2: Using qmake from Command Line

```powershell
cd "f:\VS Code ALL Files\Dsa Project\DSA-Project-Car-parking-system-"

# Generate Makefile
qmake ParkingSystemUI.pro

# Build the project
nmake          # For MSVC
# or
mingw32-make   # For MinGW
```

### Method 3: Using CMake

```powershell
cd "f:\VS Code ALL Files\Dsa Project\DSA-Project-Car-parking-system-"

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 16 2019"
# or for MinGW:
cmake .. -G "MinGW Makefiles"

# Build
cmake --build . --config Release
```

### Method 4: Using VS Code with CMake Tools

1. Install CMake Tools extension in VS Code
2. Select appropriate Kit (MSVC or MinGW)
3. Run "CMake: Build" from command palette
4. Run "CMake: Run without debugging"

## Troubleshooting

### Issue: "qmake not found"

**Solution**: Add Qt to PATH
```powershell
# Temporarily
$env:Path += ";C:\Qt\6.x\msvc2019_64\bin"

# Permanently: Edit environment variables
```

### Issue: "Qt modules not found"

**Solution**: Ensure correct Qt version is installed
```powershell
# List installed Qt versions
# Check: C:\Qt\
```

### Issue: MSVC compiler not found

**Solution**: Install Visual Studio Build Tools
- Download from: https://visualstudio.microsoft.com/downloads/
- Select "Desktop development with C++"

### Issue: Linker errors during build

**Solution**: Clean and rebuild
```powershell
qmake -r ParkingSystemUI.pro
nmake clean
nmake
```

## Running the Application

After successful build:

### From Command Line
```powershell
# After qmake build
.\ParkingSystemUI.exe

# After CMake build
.\build\Release\ParkingSystemUI.exe
```

### From Qt Creator
- Click the green "Run" button
- Or press Ctrl+R

## Expected Output

When the application launches, you should see:

1. **Main Window**: Titled "DSA Parking System - Management Interface"
2. **Tab Widget** with 5 tabs:
   - Parking Requests
   - Dashboard
   - History
   - Analytics
   - Rollback
3. **Parking Requests Tab** active with input fields for:
   - Vehicle ID
   - Zone selection
   - Create Request button

## Verification Steps

1. **Application Launches**: Window appears without errors
2. **Tabs Load**: All 5 tabs are visible and clickable
3. **Create Request**: Can enter vehicle ID and select zone
4. **Dashboard**: Shows initial statistics (all zero)
5. **Button Clicks**: All buttons respond to clicks

## Build Artifacts

After successful build, you'll have:

```
build/
├── Debug/
│   ├── ParkingSystemUI.exe
│   └── [Qt DLLs and dependencies]
└── Release/
    ├── ParkingSystemUI.exe
    └── [Qt DLLs and dependencies]
```

## Running on Different Machines

For Distribution, copy:
- `ParkingSystemUI.exe`
- All required Qt DLLs (listed in build directory)
- Or use Qt deployment tool: `windeployqt ParkingSystemUI.exe`

## Development Build vs Release Build

**Debug Build** (Development):
```powershell
qmake CONFIG+=debug ParkingSystemUI.pro
nmake debug
```

**Release Build** (Production):
```powershell
qmake CONFIG+=release ParkingSystemUI.pro
nmake release
```

## Next Steps After Build

1. Test basic functionality as described in UI_README.md
2. Create parking requests
3. Monitor dashboard statistics
4. Test rollback operations
5. Verify zone analytics

## Support Resources

- Qt Documentation: https://doc.qt.io/
- Qt Creator Manual: https://doc.qt.io/qtcreator/
- Project Repository: Check main README.md

## Advanced Configuration

### Enable Additional Qt Modules
Edit `ParkingSystemUI.pro`:
```qmake
QT += core gui sql network
```

### Change Compiler Flags
```qmake
CONFIG += debug_and_release
QMAKE_CXXFLAGS += -Wall -O2
```

### Custom Build Paths
```qmake
DESTDIR = ./bin
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
```

## Clean Build

To clean build artifacts:

```powershell
# Using qmake
qmake ParkingSystemUI.pro
nmake distclean

# Using CMake
cd build
cmake --build . --target clean
# or delete entire build folder and reconfigure

# Using VS Code/Qt Creator
# Delete build directory or use Clean Project option
```

---

**Last Updated**: January 2026
**Qt Version Tested**: 5.15 LTS, 6.x LTS
**Compiler**: MSVC 2019+, MinGW
