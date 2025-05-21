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

# Define source file groups
file(GLOB CORE_SOURCES       LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/core/*.cpp")
file(GLOB UI_WINDOWS_SOURCES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/windows/*.cpp")
file(GLOB UI_VIEWS_SOURCES   LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/views/*.cpp")
file(GLOB UI_DIALOGS_SOURCES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/dialogs/*.cpp")
# According to README.md, you might have sources in src/app/ and its subdirectories:
# file(GLOB APP_CORE_SOURCES      LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/app/*.cpp")
# file(GLOB APP_SERVICES_SOURCES  LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/app/services/*.cpp")
# file(GLOB APP_UTILS_SOURCES     LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/app/utils/*.cpp")
# And for UI widgets, if they have .cpp files:
# file(GLOB UI_WIDGETS_SOURCES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/widgets/*.cpp")

# List of source files
set(PROJECT_SOURCES
    src/main.cpp
    src/stdafx.cpp
    ${CORE_SOURCES}
    ${UI_WINDOWS_SOURCES}
    ${UI_VIEWS_SOURCES}
    ${UI_DIALOGS_SOURCES}
    # ${APP_CORE_SOURCES}
    # ${APP_SERVICES_SOURCES}
    # ${APP_UTILS_SOURCES}
    # ${UI_WIDGETS_SOURCES}
)

# Define header file groups
file(GLOB CORE_HEADERS       LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/core/*.h")
file(GLOB UI_WINDOWS_HEADERS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/windows/*.h")
file(GLOB UI_VIEWS_HEADERS   LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/views/*.h")
file(GLOB UI_DIALOGS_HEADERS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/dialogs/*.h")
# According to README.md, you might have headers in src/app/ and its subdirectories:
# file(GLOB APP_CORE_HEADERS      LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/app/*.h")
# file(GLOB APP_SERVICES_HEADERS  LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/app/services/*.h")
# file(GLOB APP_UTILS_HEADERS     LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/app/utils/*.h")
# And for other UI directories:
# file(GLOB UI_WIDGETS_HEADERS   LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/widgets/*.h")
# file(GLOB UI_MODELS_HEADERS    LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/models/*.h")
# file(GLOB UI_DELEGATES_HEADERS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/delegates/*.h")

# List of header files (especially those needing MOC)
set(PROJECT_HEADERS
    src/stdafx.h
    ${CORE_HEADERS}
    ${UI_WINDOWS_HEADERS}
    ${UI_VIEWS_HEADERS}
    ${UI_DIALOGS_HEADERS}
    # ${APP_CORE_HEADERS}
    # ${APP_SERVICES_HEADERS}
    # ${APP_UTILS_HEADERS}
    # ${UI_WIDGETS_HEADERS}
    # ${UI_MODELS_HEADERS}
    # ${UI_DELEGATES_HEADERS}
)

# Define UI file groups
file(GLOB UI_WINDOWS_UI_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/windows/*.ui")
file(GLOB UI_VIEWS_UI_FILES   LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/views/*.ui")
# According to README.md, you might have UI files in src/ui/widgets:
# file(GLOB UI_WIDGETS_UI_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ui/widgets/*.ui")

# List of UI files
set(PROJECT_UI_FILES
    ${UI_WINDOWS_UI_FILES}
    ${UI_VIEWS_UI_FILES}
    # ${UI_WIDGETS_UI_FILES}
)

# List of QRC files
file(GLOB PROJECT_QRC_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/resources/*.qrc")

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
target_include_directories(Cashatak PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/windows
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/views
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dialogs
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
)

# Optional: If you want to install the application
# install(TARGETS Cashatak DESTINATION bin)