#ifndef NODESAMPLER2D_H
#define NODESAMPLER2D_H
#include "src/qnodeseditor/qneblock.h"
#include <QGraphicsScene>

class NodeSampler2D
{
public:
    NodeSampler2D(QGraphicsScene *scene);
private:
     QNEBlock *m_b;
     QGraphicsScene *m_scene;
     QGraphicsPixmapItem* m_item;
};

#endif // NODESAMPLER2D_H
