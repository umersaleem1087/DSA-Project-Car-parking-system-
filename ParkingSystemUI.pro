QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = ParkingSystemUI
TEMPLATE = app

SOURCES += \
    src/main-qt.cpp \
    src/MainWindow.cpp \
    src/ParkingSystem.cpp \
    src/AllocationEngine.cpp \
    src/ParkingArea.cpp \
    src/ParkingRequest.cpp \
    src/ParkingSlot.cpp \
    src/RollbackManager.cpp \
    src/Vehicle.cpp \
    src/zone.cpp

HEADERS += \
    include/MainWindow.h \
    include/ParkingSystem.h \
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
    include/Zone.h

INCLUDEPATH += include/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Disable creation of .a files
CONFIG -= static
