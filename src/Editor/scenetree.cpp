#include "scenetree.h"
#include <iostream>
#include <QVector3D>
#include <QMatrix4x4>
#include <QDrag>
#include <QMimeData>
SceneTree::SceneTree(QWidget *parent)
{
    this->setDragEnabled(true);
}


SceneTree::~SceneTree()
{

}


void SceneTree::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::LeftButton){
        EntityTreeWidgetItem *item = (EntityTreeWidgetItem*)this->itemAt( mouseEvent->pos() );
        if(item && item->entity){
            emit clicked(item->entity);
        }

    }
}

void SceneTree::dropEvent(QDropEvent *event)
{
    QTreeWidget::dropEvent(event);
    EntityTreeWidgetItem *item = _item;

    if(item){
        EntityTreeWidgetItem* parent = (EntityTreeWidgetItem*)item->parent();
        if(parent)
            parent->entity->removeChild(item->entity);
        parent = (EntityTreeWidgetItem*)this->itemAt( event->pos() );
        if(parent){
            //parent->addChild(item);
            item->entity->setParent(parent->entity);
            parent->entity->addChild(item->entity);
            this->repaint();
        }
    }

}

void SceneTree::addEntity(Entity *entity)
{
    EntityTreeWidgetItem* treeWidgetItem = new EntityTreeWidgetItem();
    treeWidgetItem->entity = entity;
    treeWidgetItem->setText(0,entity->getLabel());
    this->addTopLevelItem(treeWidgetItem);

}
