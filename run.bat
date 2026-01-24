@echo off
REM Primary launch script for Parking System UI
REM Executable location: build_qt\release\ParkingSystemUI.exe

cd /d "%~dp0"
start "" "build_qt\release\ParkingSystemUI.exe"
exit /b 0
