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
    Cashatak/main.cpp
    Cashatak/Cashatak.cpp
    Cashatak/Account.cpp
    Cashatak/Admin.cpp
    Cashatak/Database.cpp
    Cashatak/Event.cpp
    Cashatak/FileManager.cpp
    Cashatak/Notification.cpp
    Cashatak/NotificationManager.cpp
    Cashatak/PaymentRequest.cpp
    Cashatak/Transaction.cpp
    Cashatak/User.cpp
    Cashatak/stdafx.cpp
)

# List of header files (especially those needing MOC)
set(PROJECT_HEADERS
    Cashatak/Cashatak.h
    Cashatak/Account.h
    Cashatak/Admin.h
    Cashatak/Database.h
    Cashatak/Event.h
    Cashatak/FileManager.h
    Cashatak/Notification.h
    Cashatak/NotificationManager.h
    Cashatak/PaymentRequest.h
    Cashatak/Transaction.h
    Cashatak/User.h
    Cashatak/stdafx.h
)

# List of UI and QRC files
set(PROJECT_UI_FILES
    Cashatak/Cashatak.ui
)

set(PROJECT_QRC_FILES
    Cashatak/Cashatak.qrc
)

# Add sources to the target
target_sources(Cashatak PRIVATE
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS} # For IDEs and AUTOMOC to see them
    ${PROJECT_UI_FILES}
    ${PROJECT_QRC_FILES}
)

# Add precompiled header
target_precompile_headers(Cashatak PRIVATE Cashatak/stdafx.h)

# Link Qt libraries
target_link_libraries(Cashatak PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# Include directories (if necessary, e.g., if stdafx.h is not found)
target_include_directories(Cashatak PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Cashatak)

# Optional: If you want to install the application
# install(TARGETS Cashatak DESTINATION bin) 