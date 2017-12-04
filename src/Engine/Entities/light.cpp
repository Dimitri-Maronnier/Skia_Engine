#include "light.h"

Light::Light()
{
    _position = glm::vec3(0);
    _color = glm::vec3(1);
    _nearPlane = 1.0f;
    _farPlane = 5.0f;
}

Light::Light(glm::vec3 position, glm::vec3 color,float nearPlane,float farPlane){
    _position = position;
    _color = color;
    _nearPlane = nearPlane;
    _farPlane = farPlane;
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

glm::mat4 Light::getLightProjectionMatrix()
{
    return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, _nearPlane, _farPlane);
}

void Light::setPosition(glm::vec3 position){
    _position = position;
}

void Light::setColor(glm::vec3 color){
    _color = color;
}

glm::mat4 Light::computeLightProjectionViewMatrix()
{
    return getLightProjectionMatrix() * getLightViewMatrix();
}
