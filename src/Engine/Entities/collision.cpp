#include "collision.h"

Collision::Collision()
{
}

Collision::Collision(QString label, glm::vec3 point, glm::vec3 normale, int tag)
{
    this->colliderLabel = label;
    this->collisionPoint = point;
    this->normalAtCollision = normale;
    this->tag = tag;
    this->valid = true;
}

Collision Collision::reciproc(Entity e)
{
    return Collision(e.getLabel(), collisionPoint, normalAtCollision * -1.0f, e.getTag());
}

