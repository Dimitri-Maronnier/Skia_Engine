#include "light.h"

Light::Light()
{
    position = glm::vec3(0);
    tint = glm::vec3(1);
}

Light::Light(glm::vec3 position,glm::vec3 tint){
    this->position = position;
    this->tint = tint;
}

glm::vec3 Light::getPosition(){
    return position;
}

glm::vec3 Light::getTint(){
    return tint;
}

void Light::setPosition(glm::vec3 position){
    this->position = position;
}

void Light::setTint(glm::vec3 tint){
    this->tint = tint;
}
