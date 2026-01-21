cmake_minimum_required(VERSION 3.16)
project(ParkingSystemUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 or Qt5
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
endif()

# Core system source files
set(CORE_SOURCES
    src/AllocationEngine.cpp
    src/ParkingArea.cpp
    src/ParkingRequest.cpp
    src/ParkingSlot.cpp
    src/ParkingSystem.cpp
    src/RollbackManager.cpp
    src/Vehicle.cpp
    src/zone.cpp
)

# UI source files
set(UI_SOURCES
    src/main-qt.cpp
    src/MainWindow.cpp
)

# Headers
set(HEADERS
    include/MainWindow.h
    include/ParkingSystem.h
    include/AllocationEngine.h
    include/Common.h
    include/LinkedList.h
    include/Node.h
    include/ParkingArea.h
    include/ParkingRequest.h
    include/ParkingSlot.h
    include/RollbackManager.h
    include/Stack.h
    include/Vehicle.h
    include/Zone.h
)

# Create executable
add_executable(${PROJECT_NAME}
    ${CORE_SOURCES}
    ${UI_SOURCES}
    ${HEADERS}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include/)

# Link Qt libraries
if(Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)
else()
    target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)
endif()
