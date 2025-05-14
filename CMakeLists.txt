cmake_minimum_required(VERSION 3.16)
project(Cashatak VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Automatically run moc, rcc, and uic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set the build output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Define the executable
add_executable(Cashatak)

# List of source files
set(PROJECT_SOURCES
    src/main.cpp
    src/ui/windows/CashatakMainWindow.cpp
    src/core/Account.cpp
    src/core/Admin.cpp
    src/core/Database.cpp
    src/core/Event.cpp
    src/core/FileManager.cpp
    src/core/Notification.cpp
    src/core/NotificationManager.cpp
    src/core/PaymentRequest.cpp
    src/core/Transaction.cpp
    src/core/User.cpp
    src/stdafx.cpp
)

# List of header files (especially those needing MOC)
set(PROJECT_HEADERS
    src/ui/windows/CashatakMainWindow.h
    src/core/Account.h
    src/core/Admin.h
    src/core/Database.h
    src/core/Event.h
    src/core/FileManager.h
    src/core/Notification.h
    src/core/NotificationManager.h
    src/core/PaymentRequest.h
    src/core/Transaction.h
    src/core/User.h
    src/stdafx.h
)

# List of UI and QRC files
set(PROJECT_UI_FILES
    src/ui/windows/CashatakMainWindow.ui
)

set(PROJECT_QRC_FILES
    src/resources/Cashatak.qrc
)

# Add sources to the target
target_sources(Cashatak PRIVATE
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS} # For IDEs and AUTOMOC to see them
    ${PROJECT_UI_FILES}
    ${PROJECT_QRC_FILES}
)

# Add precompiled header
target_precompile_headers(Cashatak PRIVATE src/stdafx.h)

# Link Qt libraries
target_link_libraries(Cashatak PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# Include directories
# This allows you to use #include "MyClass.h" if MyClass.h is in src/
# or #include "core/MyCoreClass.h" if it's in src/core/
target_include_directories(Cashatak PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/windows
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
)

# Optional: If you want to install the application
# install(TARGETS Cashatak DESTINATION bin) 