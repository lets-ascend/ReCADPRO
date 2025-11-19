import QtQuick
import QtQuick.Controls
import ReCADPro

/**
 * ObjectPanel - Panel for managing drawing objects
 * Shows list of objects with lock/unlock, group/ungroup controls
 */
Rectangle {
    id: objectPanel
    
    property ObjectManager objectManager: null
    
    color: "#ffffff"
    border.color: "#e0e0e0"
    border.width: 1
    
    ScrollView {
        anchors.fill: parent
        anchors.margins: 16
        
        Column {
            spacing: 16
            
            // Header
            Row {
                width: parent.width
                spacing: 8
                
                Text {
                    text: "Objects"
                    font.bold: true
                    font.pixelSize: 16
                }
                
                Text {
                    text: objectManager ? "(" + objectManager.objectCount + ")" : "(0)"
                    font.pixelSize: 14
                    color: "#666666"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            
            // Object list
            ListView {
                width: parent.width
                height: Math.min(300, (objectManager ? objectManager.objectCount : 0) * 60)
                model: objectManager ? objectManager.objects : []
                
                delegate: Rectangle {
                    width: parent.width
                    height: 60
                    color: modelData.selected ? "#e3f2fd" : "#ffffff"
                    border.color: "#e0e0e0"
                    border.width: 1
                    
                    Column {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 4
                        
                        // Object name and lock status
                        Row {
                            width: parent.width
                            spacing: 8
                            
                            Text {
                                text: modelData.name || "Object " + index
                                font.bold: modelData.selected
                                anchors.verticalCenter: parent.verticalCenter
                                width: 120
                                elide: Text.ElideRight
                            }
                            
                            Item { width: 1; height: 1 } // Spacer
                            
                            Button {
                                text: modelData.locked ? "🔒" : "🔓"
                                width: 40
                                height: 24
                                onClicked: {
                                    if (modelData.locked) {
                                        objectManager.unlockObject(modelData.id);
                                    } else {
                                        objectManager.lockObject(modelData.id);
                                    }
                                }
                            }
                            
                            Button {
                                text: modelData.visible ? "👁" : "🚫"
                                width: 40
                                height: 24
                                onClicked: {
                                    modelData.visible = !modelData.visible;
                                }
                            }
                        }
                        
                        // Object controls
                        Row {
                            width: parent.width
                            spacing: 4
                            
                            Button {
                                text: "Select"
                                width: 60
                                height: 20
                                font.pixelSize: 10
                                onClicked: {
                                    objectManager.selectObject(modelData.id);
                                }
                            }
                            
                            Button {
                                text: "Delete"
                                width: 60
                                height: 20
                                font.pixelSize: 10
                                enabled: !modelData.locked
                                onClicked: {
                                    objectManager.deleteObject(modelData.id);
                                }
                            }
                        }
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            objectManager.selectObject(modelData.id);
                        }
                    }
                }
            }
            
            // Object operations
            Column {
                width: parent.width
                spacing: 8
                
                Text {
                    text: "Operations"
                    font.bold: true
                    font.pixelSize: 14
                }
                
                Button {
                    width: parent.width
                    text: "Select All"
                    onClicked: {
                        if (objectManager) {
                            objectManager.selectAll();
                        }
                    }
                }
                
                Button {
                    width: parent.width
                    text: "Deselect All"
                    onClicked: {
                        if (objectManager) {
                            objectManager.deselectAll();
                        }
                    }
                }
                
                Button {
                    width: parent.width
                    text: "Lock All"
                    onClicked: {
                        if (objectManager) {
                            objectManager.lockAll();
                        }
                    }
                }
                
                Button {
                    width: parent.width
                    text: "Unlock All"
                    onClicked: {
                        if (objectManager) {
                            objectManager.unlockAll();
                        }
                    }
                }
                
                Button {
                    width: parent.width
                    text: "Group Selected"
                    enabled: objectManager && objectManager.getSelectedObjects().length > 1
                    onClicked: {
                        if (objectManager) {
                            var selected = objectManager.getSelectedObjects();
                            if (selected.length > 1) {
                                // Group objects (would need group method)
                                // objectManager.groupObjects(selected);
                            }
                        }
                    }
                }
            }
        }
    }
}

