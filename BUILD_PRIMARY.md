# Parking System UI - Build Configuration

## Primary Executable Location
**`build_qt/release/ParkingSystemUI.exe`**

This is the only location where the application is built and should be used.

## Quick Launch
Run the application using one of these methods:

1. **Using batch script (recommended):**
   ```
   run.bat
   ```

2. **Direct path:**
   ```
   build_qt\release\ParkingSystemUI.exe
   ```

## Build Process
To rebuild the application:

```bash
cd build_qt
mingw32-make clean
mingw32-make
```

The executable will be generated in: `build_qt/release/ParkingSystemUI.exe`

## Important Notes
- The old `release/` directory has been removed
- All future builds will output to `build_qt/release/`
- Qt projects are configured to use this directory by default
- No other executable locations are maintained

---
Last Updated: 2026-01-24
