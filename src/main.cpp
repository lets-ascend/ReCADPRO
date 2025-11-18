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

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("reMarkable Drafting Pro");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("reMarkable");

    QQmlApplicationEngine engine;

    // Register C++ types for QML
    qmlRegisterType<DrawingEngine>("RemarkableDraftingPro", 1, 0, "DrawingEngine");
    qmlRegisterType<Tools>("RemarkableDraftingPro", 1, 0, "Tools");
    qmlRegisterType<Layers>("RemarkableDraftingPro", 1, 0, "Layers");
    qmlRegisterType<DraftingTools>("RemarkableDraftingPro", 1, 0, "DraftingTools");
    qmlRegisterType<StylusInput>("RemarkableDraftingPro", 1, 0, "StylusInput");
    qmlRegisterType<Clipboard>("RemarkableDraftingPro", 1, 0, "Clipboard");
    qmlRegisterType<Importer>("RemarkableDraftingPro", 1, 0, "Importer");
    qmlRegisterType<Measurement>("RemarkableDraftingPro", 1, 0, "Measurement");
    // Note: Measurement is already registered above
    qmlRegisterType<Drawing3D>("RemarkableDraftingPro", 1, 0, "Drawing3D");
    qmlRegisterType<DrawingObject>("RemarkableDraftingPro", 1, 0, "DrawingObject");
    qmlRegisterType<ObjectManager>("RemarkableDraftingPro", 1, 0, "ObjectManager");
    qmlRegisterType<LayerCompositor>("RemarkableDraftingPro", 1, 0, "LayerCompositor");
    qmlRegisterType<LayerManager>("RemarkableDraftingPro", 1, 0, "LayerManager");
    qmlRegisterType<SelectionManager>("RemarkableDraftingPro", 1, 0, "SelectionManager");
    qmlRegisterType<ShapeTools>("RemarkableDraftingPro", 1, 0, "ShapeTools");
    qmlRegisterType<TextTool>("RemarkableDraftingPro", 1, 0, "TextTool");
    qmlRegisterType<Exporter>("RemarkableDraftingPro", 1, 0, "Exporter");
    qmlRegisterType<ProjectFile>("RemarkableDraftingPro", 1, 0, "ProjectFile");
    qmlRegisterType<AdvancedLineTools>("RemarkableDraftingPro", 1, 0, "AdvancedLineTools");
    qmlRegisterType<TransformTools>("RemarkableDraftingPro", 1, 0, "TransformTools");
    qmlRegisterType<BrushTools>("RemarkableDraftingPro", 1, 0, "BrushTools");
    qmlRegisterType<AdvancedMeasurement>("RemarkableDraftingPro", 1, 0, "AdvancedMeasurement");
    qmlRegisterType<Drawing3DEnhanced>("RemarkableDraftingPro", 1, 0, "Drawing3DEnhanced");

    // Get screen dimensions (reMarkable Paper Pro: 1872x1404)
    QScreen *screen = app.primaryScreen();
    QRect screenGeometry = screen->geometry();
    
    // Expose screen properties to QML
    engine.rootContext()->setContextProperty("screenWidth", screenGeometry.width());
    engine.rootContext()->setContextProperty("screenHeight", screenGeometry.height());

    // Load main QML file
    const QUrl url(QStringLiteral("qrc:/RemarkableDraftingPro/main.qml"));
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    
    engine.loadFromModule("RemarkableDraftingPro", "Main");

    return app.exec();
}

