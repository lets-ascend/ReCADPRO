import QtQuick
import QtQuick.Controls

/**
 * Sidebar - Left sidebar with drawing tools
 * Optimized for reMarkable Paper Pro touch interface
 */
Rectangle {
    id: sidebar
    
    property alias tools: toolsRef
    property alias draftingTools: draftingToolsRef
    
    Tools { id: toolsRef }
    DraftingTools { id: draftingToolsRef }
    
    color: "#ffffff"
    border.color: "#e0e0e0"
    border.width: 1
    
    ScrollView {
        anchors.fill: parent
        anchors.margins: 16
        
        Column {
            spacing: 24
            
            // Drawing tools section
            Column {
                width: parent.width
                spacing: 8
                
                Text {
                    text: "Drawing Tools"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Grid {
                    columns: 4
                    spacing: 8
                    
                    ToolIcon {
                        iconType: "pen"
                        checked: toolsRef.currentTool === "pen"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: toolsRef.currentTool = "pen"
                        }
                    }
                    
                    ToolIcon {
                        iconType: "pencil"
                        checked: toolsRef.currentTool === "pencil"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: toolsRef.currentTool = "pencil"
                        }
                    }
                    
                    ToolIcon {
                        iconType: "brush"
                        checked: toolsRef.currentTool === "brush"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: toolsRef.currentTool = "brush"
                        }
                    }
                    
                    ToolIcon {
                        iconType: "eraser"
                        checked: toolsRef.currentTool === "eraser"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: toolsRef.currentTool = "eraser"
                        }
                    }
                }
            }
            
            // Shape tools section
            Column {
                width: parent.width
                spacing: 8
                
                Text {
                    text: "Shapes"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Grid {
                    columns: 4
                    spacing: 8
                    
                    ToolIcon {
                        iconType: "line"
                        checked: toolsRef.currentTool === "line"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                toolsRef.currentTool = "line"
                                if (window && window.shapeTools) {
                                    window.shapeTools.currentShape = "line"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "rectangle"
                        checked: toolsRef.currentTool === "rectangle"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                toolsRef.currentTool = "rectangle"
                                if (window && window.shapeTools) {
                                    window.shapeTools.currentShape = "rectangle"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "circle"
                        checked: toolsRef.currentTool === "circle"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                toolsRef.currentTool = "circle"
                                if (window && window.shapeTools) {
                                    window.shapeTools.currentShape = "circle"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "arrow"
                        checked: toolsRef.currentTool === "arrow"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                toolsRef.currentTool = "arrow"
                                if (window && window.shapeTools) {
                                    window.shapeTools.currentShape = "arrow"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "text"
                        checked: toolsRef.currentTool === "text"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: toolsRef.currentTool = "text"
                        }
                    }
                    
                    ToolIcon {
                        iconType: "select"
                        checked: toolsRef.currentTool === "select"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: toolsRef.currentTool = "select"
                        }
                    }
                }
            }
            
            // Shape fill options (when shape tool selected)
            Column {
                width: parent.width
                spacing: 8
                visible: toolsRef.currentTool === "rectangle" || 
                         toolsRef.currentTool === "circle"
                
                Text {
                    text: "Shape Options"
                    font.bold: true
                    font.pixelSize: 14
                }
                
                CheckBox {
                    text: "Fill Shape"
                    checked: window && window.shapeTools ? window.shapeTools.fillEnabled : false
                    onToggled: {
                        if (window && window.shapeTools) {
                            window.shapeTools.fillEnabled = checked
                        }
                    }
                }
                
                // Fill color picker (simplified)
                Rectangle {
                    width: parent.width
                    height: 32
                    color: window && window.shapeTools ? window.shapeTools.fillColor : "#ffffff"
                    border.color: "#e0e0e0"
                    border.width: 1
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: fillColorDialog.open()
                    }
                }
            }
            
            // Advanced Line Tools section (AutoCAD/Fusion 360 style)
            Column {
                width: parent.width
                spacing: 8
                
                Text {
                    text: "Advanced Lines"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Grid {
                    columns: 2
                    spacing: 8
                    
                    ToolIcon {
                        iconType: "line"
                        checked: toolsRef.currentTool === "line" && (!window || !window.advancedLineTools || window.advancedLineTools.currentTool === "line")
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                toolsRef.currentTool = "line"
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "line"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "ray"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "ray"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "ray"
                                    toolsRef.currentTool = "ray"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "construction"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "construction"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "construction"
                                    toolsRef.currentTool = "construction"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "polyline"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "polyline"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "polyline"
                                    toolsRef.currentTool = "polyline"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "spline"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "spline"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "spline"
                                    toolsRef.currentTool = "spline"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "bezier"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "bezier"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "bezier"
                                    toolsRef.currentTool = "bezier"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "tangent"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "tangent"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "tangent"
                                    toolsRef.currentTool = "tangent"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "perpendicular"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "perpendicular"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "perpendicular"
                                    toolsRef.currentTool = "perpendicular"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "parallel"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "parallel"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "parallel"
                                    toolsRef.currentTool = "parallel"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "offset"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "offset"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "offset"
                                    toolsRef.currentTool = "offset"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "fillet"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "fillet"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "fillet"
                                    toolsRef.currentTool = "fillet"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "chamfer"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "chamfer"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "chamfer"
                                    toolsRef.currentTool = "chamfer"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "trim"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "trim"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "trim"
                                    toolsRef.currentTool = "trim"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "extend"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "extend"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "extend"
                                    toolsRef.currentTool = "extend"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "break"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "break"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "break"
                                    toolsRef.currentTool = "break"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "join"
                        checked: window && window.advancedLineTools && window.advancedLineTools.currentTool === "join"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.advancedLineTools) {
                                    window.advancedLineTools.currentTool = "join"
                                    toolsRef.currentTool = "join"
                                }
                            }
                        }
                    }
                }
                
                // Construction mode toggle
                CheckBox {
                    text: "Construction Mode"
                    checked: window && window.advancedLineTools ? window.advancedLineTools.constructionMode : false
                    onToggled: {
                        if (window && window.advancedLineTools) {
                            window.advancedLineTools.constructionMode = checked
                        }
                    }
                }
            }
            
            // Drafting tools section
            Column {
                width: parent.width
                spacing: 8
                
                Text {
                    text: "Drafting Tools"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Column {
                    spacing: 8
                    
                    ToolIcon {
                        iconType: "ruler"
                        checked: toolsRef.currentTool === "ruler"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: toolsRef.currentTool = "ruler"
                        }
                    }
                    
                    ToolIcon {
                        iconType: "protractor"
                        checked: toolsRef.currentTool === "protractor"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: toolsRef.currentTool = "protractor"
                        }
                    }
                }
            }
            
            // Brush Presets section
            Column {
                width: parent.width
                spacing: 8
                visible: toolsRef.currentTool === "pen" || toolsRef.currentTool === "pencil" || toolsRef.currentTool === "brush"
                
                Text {
                    text: "Brush Presets"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                ComboBox {
                    id: brushPresetCombo
                    width: parent.width
                    model: window && window.brushTools ? window.brushTools.getBrushPresetNames() : []
                    onCurrentTextChanged: {
                        if (window && window.brushTools) {
                            window.brushTools.currentBrush = currentText
                        }
                    }
                }
                
                Row {
                    spacing: 8
                    width: parent.width
                    
                    Text {
                        text: "Spacing:"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Slider {
                        id: brushSpacingSlider
                        width: 120
                        from: 0
                        to: 200
                        value: window && window.brushTools ? window.brushTools.brushSpacing * 100 : 10
                        onValueChanged: {
                            if (window && window.brushTools) {
                                window.brushTools.brushSpacing = value / 100.0
                            }
                        }
                    }
                    
                    Text {
                        text: Math.round(brushSpacingSlider.value) + "%"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                
                Row {
                    spacing: 8
                    width: parent.width
                    
                    Text {
                        text: "Opacity:"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Slider {
                        id: brushOpacitySlider
                        width: 120
                        from: 0
                        to: 100
                        value: window && window.brushTools ? window.brushTools.brushOpacity * 100 : 100
                        onValueChanged: {
                            if (window && window.brushTools) {
                                window.brushTools.brushOpacity = value / 100.0
                            }
                        }
                    }
                    
                    Text {
                        text: Math.round(brushOpacitySlider.value) + "%"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            
            // Transform Tools section
            Column {
                width: parent.width
                spacing: 8
                visible: toolsRef.currentTool === "select"
                
                Text {
                    text: "Transform"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Grid {
                    columns: 2
                    spacing: 8
                    
                    ToolIcon {
                        iconType: "free"
                        checked: window && window.transformTools && window.transformTools.currentTransform === "free"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.transformTools) {
                                    window.transformTools.currentTransform = "free"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "skew"
                        checked: window && window.transformTools && window.transformTools.currentTransform === "skew"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.transformTools) {
                                    window.transformTools.currentTransform = "skew"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "perspective"
                        checked: window && window.transformTools && window.transformTools.currentTransform === "perspective"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.transformTools) {
                                    window.transformTools.currentTransform = "perspective"
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "flip-h"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.transformTools && window.selectionManager && window.selectionManager.hasSelection) {
                                    var bounds = window.selectionManager.selectionBounds
                                    var transform = window.transformTools.createFlipHorizontal(bounds)
                                    // Apply transform to selected objects
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "flip-v"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.transformTools && window.selectionManager && window.selectionManager.hasSelection) {
                                    var bounds = window.selectionManager.selectionBounds
                                    var transform = window.transformTools.createFlipVertical(bounds)
                                    // Apply transform to selected objects
                                }
                            }
                        }
                    }
                }
            }
            
            // 3D Tools section
            Column {
                width: parent.width
                spacing: 8
                visible: toolsRef.currentTool === "3d" || toolsRef.currentTool === "isometric"
                
                Text {
                    text: "3D Tools"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Grid {
                    columns: 2
                    spacing: 8
                    
                    ToolIcon {
                        iconType: "sphere"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.drawing3DEnhanced) {
                                    var points = window.drawing3DEnhanced.createSphere(50, Qt.point(100, 100))
                                    // Draw points on canvas
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "cone"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.drawing3DEnhanced) {
                                    var points = window.drawing3DEnhanced.createCone(30, 60, Qt.point(100, 100))
                                    // Draw points on canvas
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "pyramid"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.drawing3DEnhanced) {
                                    var points = window.drawing3DEnhanced.createPyramid(40, 60, Qt.point(100, 100))
                                    // Draw points on canvas
                                }
                            }
                        }
                    }
                    
                    ToolIcon {
                        iconType: "torus"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (window && window.drawing3DEnhanced) {
                                    var points = window.drawing3DEnhanced.createTorus(50, 20, Qt.point(100, 100))
                                    // Draw points on canvas
                                }
                            }
                        }
                    }
                }
                
                Text {
                    text: "Rotation"
                    font.bold: true
                    font.pixelSize: 14
                }
                
                Row {
                    spacing: 8
                    width: parent.width
                    
                    Text {
                        text: "X:"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Slider {
                        id: rotationXSlider
                        width: 100
                        from: -180
                        to: 180
                        value: window && window.drawing3DEnhanced ? window.drawing3DEnhanced.rotationX : 0
                        onValueChanged: {
                            if (window && window.drawing3DEnhanced) {
                                window.drawing3DEnhanced.rotationX = value
                            }
                        }
                    }
                }
                
                Row {
                    spacing: 8
                    width: parent.width
                    
                    Text {
                        text: "Y:"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Slider {
                        id: rotationYSlider
                        width: 100
                        from: -180
                        to: 180
                        value: window && window.drawing3DEnhanced ? window.drawing3DEnhanced.rotationY : 0
                        onValueChanged: {
                            if (window && window.drawing3DEnhanced) {
                                window.drawing3DEnhanced.rotationY = value
                            }
                        }
                    }
                }
                
                Row {
                    spacing: 8
                    width: parent.width
                    
                    Text {
                        text: "Z:"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Slider {
                        id: rotationZSlider
                        width: 100
                        from: -180
                        to: 180
                        value: window && window.drawing3DEnhanced ? window.drawing3DEnhanced.rotationZ : 0
                        onValueChanged: {
                            if (window && window.drawing3DEnhanced) {
                                window.drawing3DEnhanced.rotationZ = value
                            }
                        }
                    }
                }
                
                ComboBox {
                    id: viewCombo
                    width: parent.width
                    model: window && window.drawing3DEnhanced ? window.drawing3DEnhanced.getAvailableViews() : []
                    onCurrentTextChanged: {
                        if (window && window.drawing3DEnhanced) {
                            window.drawing3DEnhanced.currentView = currentText
                        }
                    }
                }
            }
            
            // Properties section
            Column {
                width: parent.width
                spacing: 8
                
                Text {
                    text: "Properties"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Column {
                    spacing: 8
                    
                    Row {
                        spacing: 8
                        
                        Text {
                            text: "Line Width:"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        
                        Slider {
                            id: lineWidthSlider
                            width: 150
                            from: 1
                            to: 50
                            value: toolsRef.lineWidth
                            onValueChanged: toolsRef.lineWidth = value
                        }
                        
                        Text {
                            text: Math.round(lineWidthSlider.value) + "px"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    
                    Row {
                        spacing: 8
                        
                        Text {
                            text: "Opacity:"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        
                        Slider {
                            id: opacitySlider
                            width: 150
                            from: 0
                            to: 100
                            value: 100
                        }
                    }
                }
            }
        }
    }
    
    // Fill color dialog
    Popup {
        id: fillColorDialog
        x: parent.width + 10
        y: parent.height / 2 - height / 2
        width: 200
        height: 300
        
        Column {
            anchors.fill: parent
            spacing: 8
            
            Text {
                text: "Fill Color"
                font.bold: true
            }
            
            Repeater {
                model: ["#ffffff", "#000000", "#333333", "#666666", "#999999", "#cccccc"]
                
                Rectangle {
                    width: parent.width
                    height: 40
                    color: modelData
                    border.color: "#e0e0e0"
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (window && window.shapeTools) {
                                window.shapeTools.fillColor = modelData
                            }
                            fillColorDialog.close()
                        }
                    }
                }
            }
        }
    }
}

