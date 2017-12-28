#ifndef SCENETREE_H
#define SCENETREE_H
#include <QTreeWidget>
#include "src/Engine/Entities/entity.h"
#include <QMouseEvent>
#include <QObject>


class EntityTreeWidgetItem:public QTreeWidgetItem
{
public:
    explicit EntityTreeWidgetItem( QWidget *parent = 0){

    }

    Entity* entity;
};

class SceneTree:public QTreeWidget
{
    Q_OBJECT
public:
    explicit SceneTree( QWidget *parent = 0);
    void mouseReleaseEvent(QMouseEvent* mouseEvent);
    virtual void dropEvent(QDropEvent* event);
    virtual  void dragEnterEvent(QDragEnterEvent *event){
        event->accept();
        _item = (EntityTreeWidgetItem*)this->currentItem();
    }
    void cleanUp();
    ~SceneTree();
public slots:
    void addEntity(Entity *entity);
signals:
    void clicked(Entity* entity);
private:
    EntityTreeWidgetItem *_item;//Store cliked item to hack
};

#endif // SCENETREE_H
