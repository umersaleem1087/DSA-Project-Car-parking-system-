QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = ParkingSystemUI
TEMPLATE = app

# Minimal core system sources (only what's needed for Qt to compile)
SOURCES += \
    src/ParkingSlot.cpp \
    src/ParkingArea.cpp \
    src/ParkingRequest.cpp \
    src/AllocationEngine.cpp \
    src/Vehicle.cpp \
    src/Zone.cpp \
    src/RollbackManager.cpp \
    src/ParkingSystem.cpp

# UI specific sources
SOURCES += \
    src/main-qt.cpp \
    src/MainWindow.cpp

# Headers
HEADERS += \
    include/MainWindow.h \
    include/AllocationEngine.h \
    include/Common.h \
    include/LinkedList.h \
    include/Node.h \
    include/ParkingArea.h \
    include/ParkingRequest.h \
    include/ParkingSlot.h \
    include/RollbackManager.h \
    include/Stack.h \
    include/Vehicle.h \
    include/Zone.h \
    include/ParkingSystem.h

INCLUDEPATH += include/

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Disable creation of .a files
CONFIG -= static

# Don't include the Windows GUI subsystem flag
win32 {
    QMAKE_LFLAGS_WINDOWS = 
}

