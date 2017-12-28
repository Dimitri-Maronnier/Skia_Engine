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
        if(item!=nullptr && item->entity){
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
        if(parent!=nullptr)
            parent->entity->removeChild(item->entity);
        parent = (EntityTreeWidgetItem*)this->itemAt( event->pos() );
        if(parent!=nullptr){
            item->entity->setParent(parent->entity);
            parent->entity->addChild(item->entity);
            this->repaint();
        }else{
            item->entity->setParent(nullptr);
        }
    }

}

void SceneTree::cleanUp()
{
    this->clear();
}

void SceneTree::addEntity(Entity *entity)
{
    EntityTreeWidgetItem* treeWidgetItem = new EntityTreeWidgetItem();
    treeWidgetItem->entity = entity;
    treeWidgetItem->setText(0,entity->getLabel());
    if(entity->getParent() == nullptr)
        this->addTopLevelItem(treeWidgetItem);
    else{
        Entity* parent = entity->getParent();
        QList<QTreeWidgetItem*> l = this->findItems(parent->getLabel(),Qt::MatchContains | Qt::MatchRecursive);
        l.at(0)->addChild(treeWidgetItem);
    }

}
