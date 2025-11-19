import QtQuick
import QtQuick.Controls
import ReCADPro

/**
 * Main QML file for reMarkable Paper Pro
 * Follows reMarkable development guidelines:
 * - Uses Screen dimensions (1872x1404 for Paper Pro)
 * - Pure Qt Quick (no Widgets)
 * - Optimized for e-paper display
 */
ApplicationWindow {
    id: window
    
    // Use full screen for reMarkable Paper Pro
    width: Screen.width   // 1872px
    height: Screen.height // 1404px
    visible: true
    title: "reMarkable Drafting Pro"
    
    // Optimize for e-paper
    color: "white"
    
    // Main application controller - exposed to QML
    property alias drawingEngine: drawingCanvas
    property alias tools: toolsController
    property alias layers: layersController
    property alias draftingTools: draftingController
    property alias selectionManager: selectionController
    property alias shapeTools: shapeToolsController
    property alias textTool: textToolController
    property alias exporter: exporterController
    property alias projectFile: projectFileController
    property alias advancedLineTools: advancedLineToolsController
    property alias measurement: measurementController
    property alias transformTools: transformToolsController
    property alias brushTools: brushToolsController
    property alias advancedMeasurement: advancedMeasurementController
    property alias drawing3DEnhanced: drawing3DEnhancedController
    property alias clipboard: clipboardController
    property alias importer: importerController
    property alias objectManager: objectManagerController
    property alias layerCompositor: layerCompositorController
    property alias layerManager: layerManagerController
    property alias snapTools: snapToolsController
    
    // Tools controller
    Tools {
        id: toolsController
        currentTool: "pen"
        lineWidth: 3.0
        color: "#000000"
    }
    
    // Layers controller
    Layers {
        id: layersController
    }
    
    // Drafting tools controller
    DraftingTools {
        id: draftingController
        gridEnabled: false
        snapToGrid: false
        gridType: "lines"
        gridSize: 20.0
    }
    
    // Selection manager
    SelectionManager {
        id: selectionController
    }
    
    // Shape tools controller
    ShapeTools {
        id: shapeToolsController
        currentShape: "rectangle"
        fillEnabled: false
        fillColor: "#ffffff"
    }
    
    // Text tool controller
    TextTool {
        id: textToolController
        textColor: "#000000"
    }
    
    // Exporter controller
    Exporter {
        id: exporterController
    }
    
    // Project file controller
    ProjectFile {
        id: projectFileController
    }
    
    // Advanced line tools controller
    AdvancedLineTools {
        id: advancedLineToolsController
        lineWidth: toolsController.lineWidth
        lineColor: toolsController.color
    }
    
    // Measurement controller
    Measurement {
        id: measurementController
        unit: "mm"
        scaleFactor: 1.0
        pixelsPerUnit: 3.779527559 // 1mm = 3.78 pixels at 96 DPI
    }
    
    // Transform tools controller
    TransformTools {
        id: transformToolsController
    }
    
    // Brush tools controller
    BrushTools {
        id: brushToolsController
    }
    
    // Advanced measurement controller
    AdvancedMeasurement {
        id: advancedMeasurementController
    }
    
    // 3D enhanced controller
    Drawing3DEnhanced {
        id: drawing3DEnhancedController
    }
    
    // Clipboard controller
    Clipboard {
        id: clipboardController
    }
    
    // Importer controller
    Importer {
        id: importerController
    }
    
    // Object manager controller
    ObjectManager {
        id: objectManagerController
    }
    
    // Layer compositor controller
    LayerCompositor {
        id: layerCompositorController
    }
    
    // Layer manager controller
    LayerManager {
        id: layerManagerController
        layers: layersController
    }
    
    // Snap tools controller
    SnapTools {
        id: snapToolsController
        Component.onCompleted: {
            snapToolsController.setDrawingEngine(drawingCanvas);
            snapToolsController.setShapeTools(shapeToolsController);
        }
    }
    
    // Main layout
    Column {
        anchors.fill: parent
        spacing: 0
        
        // Top toolbar (optimized height for Paper Pro)
        Toolbar {
            id: toolbar
            width: parent.width
            height: 96  // Increased for better touch targets (12px * 8 = 96px)
            tools: toolsController
            draftingTools: draftingController
            layers: layersController
            onUndo: drawingEngine.undo()
            onRedo: drawingEngine.redo()
            onClear: drawingEngine.clear()
        }
        
        // Main content area
        Row {
            width: parent.width
            height: parent.height - toolbar.height
            spacing: 0
            
            // Left sidebar (optimized width for Paper Pro)
            Sidebar {
                id: sidebar
                width: 320  // Increased from 280 for better touch targets
                height: parent.height
                tools: toolsController
                draftingTools: draftingController
            }
            
            // Drawing canvas area
            Rectangle {
                width: parent.width - sidebar.width
                height: parent.height
                color: "#f5f5f5"
                
                // Grid overlay (if enabled)
                GridOverlay {
                    id: gridOverlay
                    anchors.fill: parent
                    visible: draftingController.gridEnabled
                    gridType: draftingController.gridType
                    gridSize: draftingController.gridSize
                }
                
                // Main drawing canvas
                DrawingCanvas {
                    id: drawingCanvas
                    anchors.fill: parent
                    anchors.margins: 20
                    
                    // Bind to tools
                    lineWidth: toolsController.lineWidth
                    color: toolsController.color
                    pressureSensitive: true
                    
                    // Bind to controllers
                    selectionManager: selectionController
                    shapeTools: shapeToolsController
                    textTool: textToolController
                    draftingTools: draftingController
                    tools: toolsController
                    advancedLineTools: advancedLineToolsController
                    measurement: measurementController
                    transformTools: transformToolsController
                    brushTools: brushToolsController
                    advancedMeasurement: advancedMeasurementController
                    drawing3DEnhanced: drawing3DEnhancedController
                    snapTools: snapToolsController
                }
                
                // Connect SelectionManager to DrawingEngine and Clipboard
                Component.onCompleted: {
                    selectionController.setDrawingEngine(drawingCanvas);
                    selectionController.setClipboard(clipboardController);
                }
                
                // Connect DrawingEngine stroke completion to LayerManager
                Connections {
                    target: drawingCanvas
                    function onStrokeCompleted(strokeIndex) {
                        // Update layer manager when stroke is completed
                        if (layerManagerController && drawingCanvas) {
                            // Register current canvas image to active layer
                            var activeLayerIndex = layersController.activeLayerIndex;
                            var canvasImage = drawingCanvas.getImage();
                            layerManagerController.registerLayerCanvas(activeLayerIndex, canvasImage);
                            
                            // Trigger layer update (layer manager will composite layers)
                            layerManagerController.updatePreview();
                        }
                    }
                }
            }
            
            // Right sidebar (layers and templates)
            LayersPanel {
                width: 320  // Increased from 280 for better touch targets
                height: parent.height
                layers: layersController
            }
        }
    }
    
    // Handle keyboard shortcuts
    Keys.onPressed: (event) => {
            if ((event.modifiers & Qt.ControlModifier) || (event.modifiers & Qt.MetaModifier)) {
            if (event.key === Qt.Key_Z) {
                if (event.modifiers & Qt.ShiftModifier) {
                    drawingEngine.redo();
                    toastMessage.show("Redo", "success", 1000);
                } else {
                    drawingEngine.undo();
                    toastMessage.show("Undo", "success", 1000);
                }
                event.accepted = true;
            } else if (event.key === Qt.Key_S) {
                // Save functionality
                saveProject();
                toastMessage.show("Project saved", "success", 2000);
                event.accepted = true;
            } else if (event.key === Qt.Key_N) {
                drawingEngine.clear();
                event.accepted = true;
            } else if (event.key === Qt.Key_C) {
                // Copy selected
                if (selectionController.hasSelection) {
                    selectionController.copySelected();
                    toastMessage.show("Copied", "success", 1000);
                } else {
                    toastMessage.show("Nothing selected", "warning", 1500);
                }
                event.accepted = true;
            } else if (event.key === Qt.Key_V) {
                // Paste
                if (clipboardController.hasContent) {
                    var pastePos = Qt.point(width/2, height/2);
                    var pastedStrokes = clipboardController.paste(pastePos);
                    
                    // Convert ClipboardStroke to DrawingEngine::StrokeData
                    var strokeData = [];
                    for (var i = 0; i < pastedStrokes.length; i++) {
                        var clipStroke = pastedStrokes[i];
                        strokeData.push({
                            points: clipStroke.points,
                            color: clipStroke.color,
                            opacity: clipStroke.opacity,
                            lineWidth: clipStroke.lineWidth
                        });
                    }
                    
                    // Add strokes to drawing engine
                    if (strokeData.length > 0) {
                        drawingCanvas.addStrokes(strokeData);
                        toastMessage.show("Pasted", "success", 1000);
                    }
                } else {
                    toastMessage.show("Clipboard empty", "warning", 1500);
                }
                event.accepted = true;
            } else if (event.key === Qt.Key_X) {
                // Cut selected
                if (selectionController.hasSelection) {
                    selectionController.copySelected();
                    selectionController.deleteSelected();
                }
                event.accepted = true;
            } else if (event.key === Qt.Key_Delete || event.key === Qt.Key_Backspace) {
                // Delete selected
                if (selectionController.hasSelection) {
                    selectionController.deleteSelected();
                }
                event.accepted = true;
            }
        } else if (event.key === Qt.Key_G) {
            draftingController.gridEnabled = !draftingController.gridEnabled;
            event.accepted = true;
        }
    }
    
    // Save project function
    function saveProject() {
        var projectData = projectFileController.createProjectData();
        // Add actual project data here
        var fileUrl = Qt.resolvedUrl("file:///home/root/recadpro-project.json");
        projectFileController.saveProject(fileUrl, projectData);
    }
    
    // Load project function
    function loadProject(fileUrl) {
        var projectData = projectFileController.loadProject(fileUrl);
        if (!projectData.isEmpty()) {
            // Load project data into drawing engine
        }
    }
}

