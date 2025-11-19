cmake_minimum_required(VERSION 3.16)
project(ReCADPro VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use Qt5 for reMarkable SDK compatibility
# Try multiple methods to find Qt5

# Method 1: Use QTDIR if set by SDK
if(DEFINED ENV{QTDIR})
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};$ENV{QTDIR}")
    message(STATUS "Using QTDIR: $ENV{QTDIR}")
endif()

# Method 2: Use OECORE_TARGET_SYSROOT (common in Yocto SDKs)
if(DEFINED ENV{OECORE_TARGET_SYSROOT})
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};$ENV{OECORE_TARGET_SYSROOT}/usr")
    message(STATUS "Using OECORE_TARGET_SYSROOT: $ENV{OECORE_TARGET_SYSROOT}/usr")
endif()

# Method 3: Try common SDK paths
set(COMMON_SDK_PATHS
    "/opt/codex/ferrari/5.2.96-dirty/sysroots/cortexa53-crypto-remarkable-linux/usr"
    "/opt/codex/ferrari/5.2.96-dirty/sysroots/aarch64-remarkable-linux/usr"
    "$ENV{HOME}/remarkable-sdk/sysroots/cortexa53-crypto-remarkable-linux/usr"
)

foreach(SDK_PATH ${COMMON_SDK_PATHS})
    if(EXISTS "${SDK_PATH}/lib/cmake/Qt5/Qt5Config.cmake")
        set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${SDK_PATH}")
        message(STATUS "Found Qt5 at: ${SDK_PATH}")
        break()
    endif()
endforeach()

# Set CMAKE_PREFIX_PATH explicitly if provided via command line
if(CMAKE_PREFIX_PATH)
    message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
endif()

find_package(Qt5 REQUIRED COMPONENTS Quick Core Gui Qml QuickControls2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Source files
set(SOURCES
    src/main.cpp
    src/drawingengine.cpp
    src/tools.cpp
    src/layers.cpp
    src/draftingtools.cpp
    src/stylusinput.cpp
    src/clipboard.cpp
    src/importer.cpp
    src/measurement.cpp
    src/drawing3d.cpp
    src/drawingobject.cpp
    src/objectmanager.cpp
    src/layercompositor.cpp
    src/layermanager.cpp
    src/selectionmanager.cpp
    src/shapetools.cpp
    src/texttool.cpp
    src/exporter.cpp
    src/projectfile.cpp
    src/advancedlinetools.cpp
    src/transformtools.cpp
    src/brushtools.cpp
    src/advancedmeasurement.cpp
    src/drawing3denhanced.cpp
    src/snaptools.cpp
)

# Header files
set(HEADERS
    src/drawingengine.h
    src/tools.h
    src/layers.h
    src/draftingtools.h
    src/stylusinput.h
    src/clipboard.h
    src/importer.h
    src/measurement.h
    src/drawing3d.h
    src/drawingobject.h
    src/objectmanager.h
    src/layercompositor.h
    src/layermanager.h
    src/selectionmanager.h
    src/shapetools.h
    src/texttool.h
    src/exporter.h
    src/projectfile.h
    src/advancedlinetools.h
    src/transformtools.h
    src/brushtools.h
    src/advancedmeasurement.h
    src/drawing3denhanced.h
    src/snaptools.h
)

# QML files
        set(QML_FILES
            qml/main.qml
            qml/DrawingCanvas.qml
            qml/Toolbar.qml
            qml/Sidebar.qml
            qml/LayersPanel.qml
            qml/GridOverlay.qml
            qml/SelectionOverlay.qml
            qml/TextInputDialog.qml
            qml/ExportDialog.qml
            qml/RulerOverlay.qml
            qml/ProtractorOverlay.qml
            qml/DimensionLineOverlay.qml
            qml/AreaMeasurementOverlay.qml
            qml/ToolIcon.qml
            qml/ObjectPanel.qml
            qml/SnapIndicatorOverlay.qml
            qml/NativeTouchButton.qml
            qml/ZoomPanHandler.qml
            qml/ToastMessage.qml
            qml/SwipeArea.qml
            qml/ZoomControls.qml
        )

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# QML resources (embedded in binary)
# Note: Qt5 uses .qrc files, we'll create one
file(WRITE ${CMAKE_BINARY_DIR}/qml.qrc
"<RCC>
    <qresource prefix=\"/\">
")
foreach(QML_FILE ${QML_FILES})
    get_filename_component(QML_NAME ${QML_FILE} NAME)
    file(APPEND ${CMAKE_BINARY_DIR}/qml.qrc "        <file alias=\"${QML_NAME}\">${CMAKE_SOURCE_DIR}/${QML_FILE}</file>\n")
endforeach()
file(APPEND ${CMAKE_BINARY_DIR}/qml.qrc
"    </qresource>
</RCC>
")

qt5_add_resources(QML_RESOURCES ${CMAKE_BINARY_DIR}/qml.qrc)

# Icon resources
qt5_add_resources(ICON_RESOURCES
    PREFIX "/resources/icons"
    FILES
        resources/icons/pen.svg
        resources/icons/pencil.svg
        resources/icons/brush.svg
        resources/icons/eraser.svg
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt5::Quick 
    Qt5::Core 
    Qt5::Gui 
    Qt5::Qml
    Qt5::QuickControls2
    ${QML_RESOURCES}
    ${ICON_RESOURCES}
)

# Set QML import path
set_target_properties(${PROJECT_NAME} PROPERTIES
    QT_QML_MODULE_VERSION "1.0"
    QT_QML_MODULE_URI "ReCADPro"
)

# Installation rules for reMarkable Paper Pro
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION /opt/recadpro
)

# Set output name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "recadpro"
)

