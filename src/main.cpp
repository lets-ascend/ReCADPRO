#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScreen>
#include <QDir>
#include <QQmlEngine>

#include "drawingengine.h"
#include "tools.h"
#include "layers.h"
#include "draftingtools.h"
#include "stylusinput.h"
#include "clipboard.h"
#include "importer.h"
#include "measurement.h"
#include "drawing3d.h"
#include "drawingobject.h"
#include "objectmanager.h"
#include "layercompositor.h"
#include "layermanager.h"
#include "selectionmanager.h"
#include "shapetools.h"
#include "texttool.h"
#include "exporter.h"
#include "projectfile.h"
#include "advancedlinetools.h"
#include "transformtools.h"
#include "brushtools.h"
#include "advancedmeasurement.h"
#include "drawing3denhanced.h"
#include "snaptools.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("reMarkable Drafting Pro");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("reMarkable");

    QQmlApplicationEngine engine;

    // Register C++ types for QML
    qmlRegisterType<DrawingEngine>("ReCADPro", 1, 0, "DrawingEngine");
    qmlRegisterType<Tools>("ReCADPro", 1, 0, "Tools");
    qmlRegisterType<Layers>("ReCADPro", 1, 0, "Layers");
    qmlRegisterType<DraftingTools>("ReCADPro", 1, 0, "DraftingTools");
    qmlRegisterType<StylusInput>("ReCADPro", 1, 0, "StylusInput");
    qmlRegisterType<Clipboard>("ReCADPro", 1, 0, "Clipboard");
    qmlRegisterType<Importer>("ReCADPro", 1, 0, "Importer");
    qmlRegisterType<Measurement>("ReCADPro", 1, 0, "Measurement");
    // Note: Measurement is already registered above
    qmlRegisterType<Drawing3D>("ReCADPro", 1, 0, "Drawing3D");
    qmlRegisterType<DrawingObject>("ReCADPro", 1, 0, "DrawingObject");
    qmlRegisterType<ObjectManager>("ReCADPro", 1, 0, "ObjectManager");
    qmlRegisterType<LayerCompositor>("ReCADPro", 1, 0, "LayerCompositor");
    qmlRegisterType<LayerManager>("ReCADPro", 1, 0, "LayerManager");
    qmlRegisterType<SelectionManager>("ReCADPro", 1, 0, "SelectionManager");
    qmlRegisterType<ShapeTools>("ReCADPro", 1, 0, "ShapeTools");
    qmlRegisterType<TextTool>("ReCADPro", 1, 0, "TextTool");
    qmlRegisterType<Exporter>("ReCADPro", 1, 0, "Exporter");
    qmlRegisterType<ProjectFile>("ReCADPro", 1, 0, "ProjectFile");
    qmlRegisterType<AdvancedLineTools>("ReCADPro", 1, 0, "AdvancedLineTools");
    qmlRegisterType<TransformTools>("ReCADPro", 1, 0, "TransformTools");
    qmlRegisterType<BrushTools>("ReCADPro", 1, 0, "BrushTools");
    qmlRegisterType<AdvancedMeasurement>("ReCADPro", 1, 0, "AdvancedMeasurement");
    qmlRegisterType<Drawing3DEnhanced>("ReCADPro", 1, 0, "Drawing3DEnhanced");
    qmlRegisterType<SnapTools>("ReCADPro", 1, 0, "SnapTools");

    // Get screen dimensions (reMarkable Paper Pro: 1872x1404)
    QScreen *screen = app.primaryScreen();
    QRect screenGeometry = screen->geometry();
    
    // Expose screen properties to QML
    engine.rootContext()->setContextProperty("screenWidth", screenGeometry.width());
    engine.rootContext()->setContextProperty("screenHeight", screenGeometry.height());

    // Load main QML file
    // For Qt5, use QUrl with qrc resource path
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    
    engine.load(url);

    return app.exec();
}

