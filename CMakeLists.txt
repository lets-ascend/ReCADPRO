cmake_minimum_required(VERSION 3.16)
project(RemarkableDraftingPro VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 (or Qt5 from SDK)
find_package(Qt6 REQUIRED COMPONENTS Quick Core Gui Qml)

# Qt standard project setup
qt_standard_project_setup(REQUIRES 6.5)

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
)

# Create executable
qt_add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Add QML module
qt_add_qml_module(${PROJECT_NAME}
    URI RemarkableDraftingPro
    VERSION 1.0
    QML_FILES ${QML_FILES}
    RESOURCES
        resources/icons/pen.svg
        resources/icons/pencil.svg
        resources/icons/brush.svg
        resources/icons/eraser.svg
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Gui
    Qt6::Qml
)

# Enable automatic MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Installation rules for reMarkable Paper Pro
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION /opt/drafting-pro
)

# Set output name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "drafting-pro"
)

