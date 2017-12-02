#include "entity.h"
#include "src/Engine/Utils/matrix.h"

Entity::Entity()
{
    _position = glm::vec3(0);
    _rotation = glm::vec3(0);
    _scale = glm::vec3(1);
    _label = "UndefinedLabel";
    computeModelMatrix();
}

Entity::Entity(glm::vec3 position,glm::vec3 rotation, glm::vec3 scale,std::string label){
    _position = position;
    _rotation = rotation;
    _scale = scale;
    _label = label;
    computeModelMatrix();
}

glm::vec3 Entity::getPostion(){
    return _position;
}

glm::vec3 Entity::getRotation(){
    return _rotation;
}

glm::vec3 Entity::getScale(){
    return _scale;
}

std::string Entity::getLabel(){
    return _label;
}

glm::mat4 Entity::getModelMatrix()
{
    return _modelMatrix;
}

void Entity::setPosition(glm::vec3 position){
    _position = position;
    computeModelMatrix();
}

void Entity::setRotation(glm::vec3 rotation){
    _rotation = rotation;
    computeModelMatrix();
}

void Entity::setScale(glm::vec3 scale){
    _scale = scale;
    computeModelMatrix();
}

void Entity::setLabel(std::string label){
    _label = label;
}

void Entity::setPositionX(float x)
{
    _position.x = x;
    computeModelMatrix();
}

void Entity::setPositionY(float y)
{
    _position.y = y;
    computeModelMatrix();
}

void Entity::setPositionZ(float z)
{
    _position.z = z;
    computeModelMatrix();
}

void Entity::setRotationX(float x)
{
    _rotation.x = x;
    computeModelMatrix();
}

void Entity::setRotationY(float y)
{
    _rotation.y = y;
    computeModelMatrix();
}

void Entity::setRotationZ(float z)
{
    _rotation.z = z;
    computeModelMatrix();
}

void Entity::setScaleX(float x)
{
    _scale.x = x;
    computeModelMatrix();
}

void Entity::setScaleY(float y)
{
    _scale.y = y;
    computeModelMatrix();
}

void Entity::setScaleZ(float z)
{
    _scale.z = z;
    computeModelMatrix();
}

void Entity::computeModelMatrix()
{
    _modelMatrix = Matrix::createTransformationMatrix(_position,_rotation,_scale);
}
