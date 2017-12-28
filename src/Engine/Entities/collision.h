#ifndef COLLISION_H
#define COLLISION_H
#include "src/Engine/Entities/entity.h"


class Entity;
class Collision
{
public:
    Collision();
    Collision(QString label, glm::vec3 point, glm::vec3 normale, int tag);

    QString colliderLabel;
    glm::vec3 collisionPoint;
    glm::vec3 normalAtCollision;
    int tag;
    bool valid;

    Collision reciproc(Entity e);
};

#endif // COLLISION_H
