#include "entity.h"
#include "src/Engine/Utils/matrix.h"
#include <iostream>
#include "../Utils/smath.h"

Entity::Entity()
{
    _position = glm::vec3(0);
    _rotation = glm::vec3(0);
    _scale = glm::vec3(1);
    _label = "UndefinedLabel";
    _parent = nullptr;
    computeModelMatrix();
}

Entity::Entity(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, QString label,unsigned int handle,std::string name,std::string path){
    _position = position;
    _rotation = rotation;
    _scale = scale;
    _label = label;
    _handle = handle;
    _name = name;
    _path = path;
    _parent = nullptr;
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

QString Entity::getLabel(){
    return _label;
}

glm::mat4 Entity::getModelMatrix()
{
    return _modelMatrix;
}

void Entity::setPosition(glm::vec3 position){
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }
    _position = position;
    computeModelMatrix();
}

void Entity::setRotation(glm::vec3 rotation){
    _rotation = rotation;
    computeRotationMatrix();
    computeModelMatrix();
}

void Entity::setScale(glm::vec3 scale){
    _scale = scale;
    computeModelMatrix();
}

void Entity::setLabel(QString label){
    _label = label;
}

void Entity::setPositionX(float x)
{
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }
    _position.x = x;
    computeModelMatrix();
}

void Entity::setPositionY(float y)
{
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }
    _position.y = y;
    computeModelMatrix();
}

void Entity::setPositionZ(float z)
{
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }
    _position.z = z;
    computeModelMatrix();
}

void Entity::setRotationX(float x)
{
    _rotation.x = x;
    computeRotationMatrix();
    computeModelMatrix();
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }

}

void Entity::setRotationY(float y)
{
    _rotation.y = y;
    computeRotationMatrix();
    computeModelMatrix();
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }


}

void Entity::setRotationZ(float z)
{
    _rotation.z = z;
    computeRotationMatrix();
    computeModelMatrix();
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }
}

void Entity::setScaleX(float x)
{
    _scale.x = x;
    computeRotationMatrix();
    computeModelMatrix();
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }
}

void Entity::setScaleY(float y)
{
    _scale.y = y;
    computeRotationMatrix();
    computeModelMatrix();
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }
}

void Entity::setScaleZ(float z)
{
    _scale.z = z;
    computeRotationMatrix();
    computeModelMatrix();
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }
}

void Entity::computeModelMatrix()
{
    if(_parent!=nullptr)
        _modelMatrix = _parent->getModelMatrix() * Matrix::createTransformationMatrix(_position,_rotationMatrix,_scale);
    else
        _modelMatrix = Matrix::createTransformationMatrix(_position,_rotationMatrix,_scale);
}

void Entity::computeRotationMatrix()
{
    _rotationMatrix = glm::eulerAngleYXZ(Utils::toRadians(_rotation.y), Utils::toRadians(_rotation.x), Utils::toRadians(_rotation.z));
}

void Entity::computeRotationMatrixQuat(glm::quat quat)
{
    _rotationMatrix = glm::mat4(quat);
}

void Entity::transformFromParent( glm::mat4 parentModelMatrix)
{

    computeRotationMatrix();
    computeModelMatrix();
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }

}

void Entity::addChild(Entity *child)
{
    _childs.push_back(child);
}

void Entity::removeChild(Entity *child)
{
     std::vector<Entity*>::iterator it;
     it = find(_childs.begin(),_childs.end(),child);
     if(it != _childs.end())
         _childs.erase(it);
}

void Entity::setParent(Entity *parent)
{
    _parent = parent;
}
