import QtQuick 2.15
import QtQuick.Controls 2.15

/**
 * ExportDialog - Dialog for exporting drawings
 * Supports PNG, SVG, PDF formats
 */
Dialog {
    id: exportDialog
    
    property Exporter exporter: null
    property var image: null
    
    title: "Export Drawing"
    modal: true
    width: 400
    height: 300
    
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    
    Column {
        anchors.fill: parent
        spacing: 16
        
        Text {
            text: "Select export format:"
            font.pixelSize: 16
        }
        
        ComboBox {
            id: formatCombo
            width: parent.width
            model: ["PNG", "SVG", "PDF"]
        }
        
        TextField {
            id: fileNameField
            width: parent.width
            placeholderText: "Enter filename..."
        }
        
        Row {
            spacing: 8
            anchors.horizontalCenter: parent.horizontalCenter
            
            Button {
                text: "Cancel"
                onClicked: exportDialog.close()
            }
            
            Button {
                text: "Export"
                onClicked: {
                    if (!exporter || !image) {
                        return;
                    }
                    
                    var format = formatCombo.currentText;
                    var fileName = fileNameField.text || "export";
                    var fileUrl = Qt.resolvedUrl("file:///" + fileName);
                    
                    var success = false;
                    if (format === "PNG") {
                        success = exporter.exportPNG(image, fileUrl);
                    } else if (format === "SVG") {
                        success = exporter.exportSVG(image, fileUrl, Qt.size(1872, 1404));
                    } else if (format === "PDF") {
                        success = exporter.exportPDF(image, fileUrl, Qt.size(1872, 1404));
                    }
                    
                    if (success) {
                        exportDialog.close();
                    }
                }
            }
        }
    }
}

