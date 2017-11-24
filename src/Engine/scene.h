#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include "src/Engine/Entities/object3dstatic.h"
#include "src/Engine/Entities/entity.h"
#include <QObject>

class Scene: public QObject
{
   Q_OBJECT
public:
    Scene();
    void addObject3DStatic(std::string completePath, std::string objectname);
    void addObject3DStatic(Object3DStatic* object);
    void cleanUp();
    std::vector<Object3DStatic*> StaticObjects;
    Object3DStatic* selectedEntity;
signals:
    void oneEntityHaveBeenSelected(Object3DStatic* entity);
};


#endif // SCENE_H
