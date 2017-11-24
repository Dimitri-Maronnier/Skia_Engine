#ifndef LIGHT_H
#define LIGHT_H
#include "glm/vec3.hpp"

class Light
{
public:
    Light();
    Light(glm::vec3 position,glm::vec3 tint);
    glm::vec3 getPosition();
    glm::vec3 getTint();

    void setPosition(glm::vec3 position);
    void setTint(glm::vec3 tint);
private:
    glm::vec3 position;
    glm::vec3 tint;

};

#endif // LIGHT_H
