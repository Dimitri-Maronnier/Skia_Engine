#include "light.h"

Light::Light()
{
    _position = glm::vec3(0);
    _color = glm::vec3(1);
}

Light::Light(glm::vec3 position, glm::vec3 color){
    _position = position;
    _color = color;
}

glm::vec3 Light::getPosition(){
    return _position;
}

glm::vec3 Light::getColor(){
    return _color;
}

glm::mat4 Light::getLightViewMatrix()
{
    return glm::lookAt(_position,glm::vec3(0,0,0),glm::vec3(0,1,0));
}

void Light::setPosition(glm::vec3 position){
    _position = position;
}

void Light::setColor(glm::vec3 color){
    _color = color;
}
