import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

/**
 * Toolbar - Top toolbar for reMarkable Paper Pro
 * Optimized for touch interaction
 */
Rectangle {
    id: toolbar
    
    property alias tools: toolsRef
    property alias draftingTools: draftingToolsRef
    property alias layers: layersRef
    
    signal undo()
    signal redo()
    signal clear()
    
    color: "#ffffff"
    border.color: "#e0e0e0"
    border.width: 1
    
    Tools { id: toolsRef }
    DraftingTools { id: draftingToolsRef }
    Layers { id: layersRef }
    
    Row {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 12
        
        // File operations
        Row {
            spacing: 8
            
            ToolButton {
                text: "File"
                onClicked: fileMenu.open()
            }
            
            ToolButton {
                text: "Undo"
                onClicked: toolbar.undo()
            }
            
            ToolButton {
                text: "Redo"
                onClicked: toolbar.redo()
            }
        }
        
        // Tool selection
        ComboBox {
            id: toolComboBox
            width: 150
            model: ["Pen", "Pencil", "Brush", "Eraser", "Line", "Rectangle", "Circle", "Arrow", "Text", "Select", "Ruler", "Protractor", "3D"]
            onCurrentTextChanged: {
                var toolName = currentText.toLowerCase()
                if (toolName === "3d") {
                    toolName = "isometric"
                }
                toolsRef.currentTool = toolName
            }
        }
        
        // Line width
        Row {
            spacing: 8
            
            Text {
                text: "Size:"
                anchors.verticalCenter: parent.verticalCenter
            }
            
            Slider {
                id: sizeSlider
                width: 100
                from: 1
                to: 50
                value: 3
                onValueChanged: toolsRef.lineWidth = value
            }
            
            Text {
                text: Math.round(sizeSlider.value) + "px"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        
        // Color picker (grayscale for e-paper)
        Rectangle {
            width: 40
            height: 32
            color: toolsRef.color
            border.color: "#e0e0e0"
            border.width: 1
            
            MouseArea {
                anchors.fill: parent
                onClicked: colorDialog.open()
            }
        }
        
        // Grid toggle
        ToolButton {
            text: draftingToolsRef.gridEnabled ? "Grid ON" : "Grid OFF"
            checkable: true
            checked: draftingToolsRef.gridEnabled
            onToggled: draftingToolsRef.gridEnabled = checked
        }
        
        // Snap to grid
        ToolButton {
            text: draftingToolsRef.snapToGrid ? "Snap ON" : "Snap OFF"
            checkable: true
            checked: draftingToolsRef.snapToGrid
            onToggled: draftingToolsRef.snapToGrid = checked
        }
    }
    
    // Color dialog (simplified for grayscale)
    Popup {
        id: colorDialog
        x: parent.width / 2 - width / 2
        y: parent.height + 10
        width: 200
        height: 300
        
        Column {
            anchors.fill: parent
            spacing: 8
            
            Text {
                text: "Select Color (Grayscale)"
                font.bold: true
            }
            
            Repeater {
                model: ["#000000", "#333333", "#666666", "#999999", "#cccccc"]
                
                Rectangle {
                    width: parent.width
                    height: 40
                    color: modelData
                    border.color: "#e0e0e0"
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            toolsRef.color = modelData
                            colorDialog.close()
                        }
                    }
                }
            }
        }
    }
    
    // File menu
    Popup {
        id: fileMenu
        x: 10
        y: parent.height + 10
        width: 150
        
        Column {
            anchors.fill: parent
            
            Button {
                width: parent.width
                text: "New"
                onClicked: {
                    toolbar.clear()
                    fileMenu.close()
                }
            }
            
            Button {
                width: parent.width
                text: "Save"
                onClicked: {
                    saveProjectDialog.open()
                    fileMenu.close()
                }
            }
            
            Button {
                width: parent.width
                text: "Load"
                onClicked: {
                    loadProjectDialog.open()
                    fileMenu.close()
                }
            }
            
            Button {
                width: parent.width
                text: "Export..."
                onClicked: {
                    exportDialog.open()
                    fileMenu.close()
                }
            }
        }
    }
    
    // Save project dialog
    FileDialog {
        id: saveProjectDialog
        title: "Save Project"
        fileMode: FileDialog.SaveFile
        nameFilters: ["JSON files (*.json)"]
        onAccepted: {
            if (window && window.projectFile) {
                var projectData = window.projectFile.createProjectData();
                window.projectFile.saveProject(file, projectData);
            }
        }
    }
    
    // Load project dialog
    FileDialog {
        id: loadProjectDialog
        title: "Load Project"
        fileMode: FileDialog.OpenFile
        nameFilters: ["JSON files (*.json)"]
        onAccepted: {
            if (window && window.projectFile) {
                window.projectFile.loadProject(file);
            }
        }
    }
    
    // Export dialog
    ExportDialog {
        id: exportDialog
        anchors.centerIn: parent.parent
        exporter: window ? window.exporter : null
        image: window && window.drawingEngine ? window.drawingEngine.getImage() : null
    }
}

