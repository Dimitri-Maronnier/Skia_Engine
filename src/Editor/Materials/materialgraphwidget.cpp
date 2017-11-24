#include "src/Editor/Materials/materialgraphwidget.h"
#include "src/Editor/Materials/materialeditorwindow.h"
#include "src/qnodeseditor/qneblock.h"


#include <QGraphicsScene>
#include <QFileDialog>
#include <QDir>
#define _USE_MATH_DEFINES
#include <math.h>
#include "src/qnodeseditor/qneport.h"
#include "src/Editor/Materials/Nodes/nodematerial.h"
#include <QPixmap>
#include <iostream>
#include <QMimeData>
#include <QTreeWidget>
#include "src/Editor/Materials/glsltreewidgetitem.h"
#include <QDataStream>
#include "src/define.h"


MaterialGraphWidget::MaterialGraphWidget(QWidget *parent)
    : QGraphicsView(parent)
{

    m_scene = new QGraphicsScene();
    nodesEditor = new QNodesEditor(this);
    nodesEditor->install(m_scene);

    m_nodeBase = new NodeBase(QPoint(0,0),m_scene);

    this->setScene(m_scene);

    this->setRenderHint(QPainter::Antialiasing, true);
    this->setAcceptDrops(true);
    this->setDragMode(QGraphicsView::RubberBandDrag);



}

MaterialGraphWidget::~MaterialGraphWidget()
{

    SAFE_DELETE(m_nodeBase);
    SAFE_DELETE(nodesEditor);
    SAFE_DELETE(m_scene);

}

NodeBase *MaterialGraphWidget::getNodeBase()
{
    return m_nodeBase;
}

void MaterialGraphWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MaterialGraphWidget::dropEvent(QDropEvent *event)
{
    QPointF relativeOrigin = mapToScene(event->pos());
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QTreeWidget *tree = dynamic_cast<QTreeWidget *>(event->source());

        QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
        QDataStream stream(&itemData, QIODevice::ReadOnly);

        int r, c;
        QMap<int, QVariant> v;
        stream >> r >> c >> v;

        GlslTreeWidgetItem *item = (GlslTreeWidgetItem *)tree->currentItem();
        NodeMaterial* n = new NodeMaterial(relativeOrigin.toPoint(),m_scene,item,m_nodeBase);

        connectBlock(n->getQNEBlock());
        fitView(relativeOrigin.toPoint());

    }

}


void MaterialGraphWidget::fitView(QPoint pos) {

    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

std::vector<NodeMaterial *> MaterialGraphWidget::getAllNode()
{
    return m_nodeMats;
}

void MaterialGraphWidget::resetNodeEditor( NodeBase *nodeBase)
{
    MaterialEditorWindow* win = (MaterialEditorWindow*)this->parent()->parent();
    win->getGlPreview()->setNodeBase(nodeBase);
    connect(nodeBase,SIGNAL(onFragmentSourceChanged(QString)),win->getDebugBrowser(),SLOT(setText(QString)));
    SAFE_DELETE(m_nodeBase);
    m_nodeBase = nodeBase;
    m_nodeBase->emitt();

}

void MaterialGraphWidget::connectBlock(QNEBlock *b)
{
    connect(b,SIGNAL(selected(NodeMaterial*)),(MaterialEditorWindow*)this->parent()->parent(),SLOT(nodeSelected(NodeMaterial*)));

}

void MaterialGraphWidget::setMaterialPath(QString materialPath)
{
    SAFE_DELETE(m_nodeBase);
    SAFE_DELETE(nodesEditor);
    SAFE_DELETE(m_scene);

    m_scene = new QGraphicsScene();
    nodesEditor = new QNodesEditor(this);
    nodesEditor->install(m_scene);

    m_nodeBase = new NodeBase(QPoint(0,0),m_scene);

    this->setScene(m_scene);

    this->setRenderHint(QPainter::Antialiasing, true);
    this->setAcceptDrops(true);
    this->setDragMode(QGraphicsView::RubberBandDrag);

    MaterialEditorWindow* win = (MaterialEditorWindow*)this->parent()->parent();
    win->getGlPreview()->setNodeBase(m_nodeBase);

    m_pathMateral = materialPath;
    this->loadMaterial();
}

void MaterialGraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void MaterialGraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void MaterialGraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, event->delta() / 240.0));
}

void MaterialGraphWidget::scaleView(qreal scaleFactor)
{

    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);

}


void MaterialGraphWidget::loadMaterial()
{

    QFile f(m_pathMateral);
    f.open(QFile::ReadOnly);
    QDataStream ds(&f);
    bool isInit;
    QString vertexSource;
    QString fragmentSource;
    ds >> isInit;
    ds >> vertexSource;
    ds >> fragmentSource;
    size_t size;
    ds >> size;
    std::cout << vertexSource.toStdString() << std::endl;
    for(int i=0;i<size;i++){
        QString path;
        ds >> path;

    }

    if(isInit)
        nodesEditor->load(this,ds);


    f.close();
}

void MaterialGraphWidget::saveMaterial()
{

    QFile f(m_pathMateral);
    f.open(QFile::WriteOnly);
    QDataStream ds(&f);
    ds << true;
    ds << m_nodeBase->getVertexSource();
    ds << m_nodeBase->getFragmentSource();

    ds << m_nodeBase->getTexturesPath().size();
    foreach(QString path,m_nodeBase->getTexturesPath())
        ds << path;

    nodesEditor->save(ds);
    f.close();
}

