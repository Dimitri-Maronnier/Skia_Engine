#ifndef MATERIALGRAPHWIGET_H
#define MATERIALGRAPHWIGET_H

#include <QGraphicsView>
#include <QWheelEvent>
#include "src/qnodeseditor/qnodeseditor.h"
#include "src/Editor/Materials/Nodes/nodebase.h"

#include <QDebug>

class QNodesEditor;

class MaterialGraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    MaterialGraphWidget( QWidget *parent = 0);
    ~MaterialGraphWidget();
    QNodesEditor* nodesEditor;

    NodeBase* getNodeBase();

    virtual  void dragEnterEvent(QDragEnterEvent *event);

    virtual  void dragLeaveEvent(QDragLeaveEvent *event){

    }
    virtual void dragMoveEvent ( QDragMoveEvent * event ){

    }

    virtual void dropEvent(QDropEvent *event);



    void fitView(QPoint pos);
    std::vector<NodeMaterial*> getAllNode();
    void resetNodeEditor(NodeBase *nodeBase);
    void connectBlock(QNEBlock* b);

    void setMaterialPath(QString materialPath);

public slots:
    void zoomIn();
    void zoomOut();
    void loadMaterial();
    void saveMaterial();

protected:
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) override;
#endif
    void scaleView(qreal scaleFactor);

private:
    NodeBase *m_nodeBase;
    QGraphicsScene *m_scene;
    std::vector<NodeMaterial*> m_nodeMats;
    QString m_pathMateral;
};

#endif // MATERIALGRAPHWIGET_H
