import QtQuick
import QtQuick.Controls
import ReCADPro

/**
 * LayersPanel - Right sidebar with layers and templates
 * Optimized for reMarkable Paper Pro
 */
Rectangle {
    id: layersPanel
    
    property alias layers: layersRef
    
    Layers { id: layersRef }
    
    color: "#ffffff"
    border.color: "#e0e0e0"
    border.width: 1
    
    ScrollView {
        anchors.fill: parent
        anchors.margins: 16
        
        Column {
            spacing: 32  // Increased spacing between sections
            
            // Layers section
            Column {
                width: parent.width
                spacing: 8
                
                Row {
                    width: parent.width
                    spacing: 8
                    
                    Text {
                        text: "Layers"
                        font.bold: true
                        font.pixelSize: 18  // Increased for readability
                    }
                    
                    NativeTouchButton {
                        text: "+"
                        width: 48  // Minimum touch target
                        height: 48
                        onClicked: layersRef.addLayer()
                    }
                }
                
                ListView {
                    width: parent.width
                    height: Math.min(400, layersRef.layerNames.length * 100)
                    model: layersRef.layerNames
                    
                    delegate: Rectangle {
                        width: parent.width
                        height: 100
                        color: layersRef.activeLayerIndex === index ? "#e3f2fd" : "#ffffff"
                        border.color: "#e0e0e0"
                        border.width: 1
                        
                        Column {
                            anchors.fill: parent
                            anchors.margins: 4
                            spacing: 4
                            
                            Row {
                                width: parent.width
                                spacing: 8
                                
                                Text {
                                    text: modelData
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 80
                                    elide: Text.ElideRight
                                }
                                
                                Item { width: 1; height: 1 } // Spacer
                                
                                NativeTouchButton {
                                    text: layersRef.isLayerVisible(index) ? "👁" : "🚫"
                                    width: 48  // Minimum touch target
                                    height: 48
                                    onClicked: layersRef.setLayerVisible(index, !layersRef.isLayerVisible(index))
                                }
                                
                                NativeTouchButton {
                                    text: "×"
                                    width: 48  // Minimum touch target
                                    height: 48
                                    onClicked: layersRef.removeLayer(index)
                                }
                            }
                            
                            // Opacity slider
                            Row {
                                width: parent.width
                                spacing: 4
                                
                                Text {
                                    text: "Opacity:"
                                    font.pixelSize: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 50
                                }
                                
                                Slider {
                                    width: 100
                                    height: 20
                                    from: 0
                                    to: 100
                                    value: layersRef.getLayerOpacity(index) * 100
                                    onValueChanged: layersRef.setLayerOpacity(index, value / 100.0)
                                }
                                
                                Text {
                                    text: Math.round(layersRef.getLayerOpacity(index) * 100) + "%"
                                    font.pixelSize: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 30
                                }
                            }
                            
                            // Blend mode dropdown
                            Row {
                                width: parent.width
                                spacing: 4
                                
                                Text {
                                    text: "Blend:"
                                    font.pixelSize: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 50
                                }
                                
                                ComboBox {
                                    width: 120
                                    height: 24
                                    model: ["normal", "multiply", "screen", "overlay", "darken", "lighten"]
                                    currentIndex: {
                                        var mode = layersRef.getLayerBlendMode(index);
                                        var modes = ["normal", "multiply", "screen", "overlay", "darken", "lighten"];
                                        return modes.indexOf(mode);
                                    }
                                    onCurrentTextChanged: layersRef.setLayerBlendMode(index, currentText)
                                }
                            }
                        }
                        
                        MouseArea {
                            anchors.fill: parent
                            onClicked: layersRef.setActiveLayerIndex(index)
                        }
                    }
                }
            }
            
            // Templates section
            Column {
                width: parent.width
                spacing: 8
                
                Text {
                    text: "Templates"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Grid {
                    columns: 2
                    spacing: 8
                    
                    Button {
                        width: 120
                        height: 80
                        text: "Grid"
                        onClicked: {
                            // Apply grid template
                        }
                    }
                    
                    Button {
                        width: 120
                        height: 80
                        text: "Ruled"
                        onClicked: {
                            // Apply ruled template
                        }
                    }
                    
                    Button {
                        width: 120
                        height: 80
                        text: "Isometric"
                        onClicked: {
                            // Apply isometric template
                        }
                    }
                    
                    Button {
                        width: 120
                        height: 80
                        text: "Dot Grid"
                        onClicked: {
                            // Apply dot grid template
                        }
                    }
                }
            }
            
            // Advanced Measurements section
            Column {
                width: parent.width
                spacing: 8
                
                Text {
                    text: "Measurements"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Column {
                    spacing: 8
                    
                    CheckBox {
                        text: "Show Dimension Lines"
                        checked: window && window.advancedMeasurement ? window.advancedMeasurement.showDimensionLines : true
                        onToggled: {
                            if (window && window.advancedMeasurement) {
                                window.advancedMeasurement.showDimensionLines = checked
                            }
                        }
                    }
                    
                    CheckBox {
                        text: "Show Area Fill"
                        checked: window && window.advancedMeasurement ? window.advancedMeasurement.showAreaFill : true
                        onToggled: {
                            if (window && window.advancedMeasurement) {
                                window.advancedMeasurement.showAreaFill = checked
                            }
                        }
                    }
                    
                    Button {
                        width: parent.width
                        text: "Export Measurements"
                        onClicked: {
                            if (window && window.advancedMeasurement) {
                                var text = window.advancedMeasurement.exportMeasurementsToText()
                                // Show export dialog or save to file
                            }
                        }
                    }
                    
                    Button {
                        width: parent.width
                        text: "Clear All"
                        onClicked: {
                            if (window && window.advancedMeasurement) {
                                window.advancedMeasurement.clearAllMeasurements()
                            }
                        }
                    }
                }
            }
            
            // Export section
            Column {
                width: parent.width
                spacing: 8
                
                Text {
                    text: "Export"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Column {
                    spacing: 8
                    
                    Button {
                        width: parent.width
                        text: "PNG"
                        onClicked: {
                            if (window && window.exporter && window.drawingEngine) {
                                var image = window.drawingEngine.getImage();
                                var fileUrl = Qt.resolvedUrl("file:///home/root/export.png");
                                window.exporter.exportPNG(image, fileUrl);
                            }
                        }
                    }
                    
                    NativeTouchButton {
                        width: parent.width
                        height: 48
                        text: "SVG"
                        onClicked: {
                            if (window && window.exporter && window.drawingEngine) {
                                var image = window.drawingEngine.getImage();
                                var fileUrl = Qt.resolvedUrl("file:///home/root/export.svg");
                                window.exporter.exportSVG(image, fileUrl, Qt.size(1872, 1404));
                            }
                        }
                    }
                    
                    NativeTouchButton {
                        width: parent.width
                        height: 48
                        text: "PDF"
                        onClicked: {
                            if (window && window.exporter && window.drawingEngine) {
                                var image = window.drawingEngine.getImage();
                                var fileUrl = Qt.resolvedUrl("file:///home/root/export.pdf");
                                window.exporter.exportPDF(image, fileUrl, Qt.size(1872, 1404));
                            }
                        }
                    }
                    
                    NativeTouchButton {
                        width: parent.width
                        height: 48
                        text: "Export All..."
                        onClicked: {
                            if (window && window.exporter) {
                                exportDialog.exporter = window.exporter
                                exportDialog.image = window.drawingEngine ? window.drawingEngine.getImage() : null
                                exportDialog.open()
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Export dialog
    ExportDialog {
        id: exportDialog
        anchors.centerIn: parent.parent
    }
}

