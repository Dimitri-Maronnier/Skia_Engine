#include "entity.h"
#include "src/Engine/Utils/matrix.h"

Entity::Entity()
{
    m_position = glm::vec3(0);
    m_rotation = glm::vec3(0);
    m_scale = glm::vec3(1);
    m_label = "UndefinedLabel";
    computeModelMatrix();
}

Entity::Entity(glm::vec3 position,glm::vec3 rotation, glm::vec3 scale,std::string label){
    m_position = position;
    m_rotation = rotation;
    m_scale = scale;
    m_label = label;
    computeModelMatrix();
}

glm::vec3 Entity::getPostion(){
    return m_position;
}

glm::vec3 Entity::getRotation(){
    return m_rotation;
}

glm::vec3 Entity::getScale(){
    return m_scale;
}

std::string Entity::getLabel(){
    return m_label;
}

glm::mat4 Entity::getModelMatrix()
{
    return m_modelMatrix;
}

void Entity::setPosition(glm::vec3 position){
    m_position = position;
    computeModelMatrix();
}

void Entity::setRotation(glm::vec3 rotation){
    m_rotation = rotation;
    computeModelMatrix();
}

void Entity::setScale(glm::vec3 scale){
    m_scale = scale;
    computeModelMatrix();
}

void Entity::setLabel(std::string label){
    m_label = label;
}

void Entity::setPositionX(float x)
{
    m_position.x = x;
    computeModelMatrix();
}

void Entity::setPositionY(float y)
{
    m_position.y = y;
    computeModelMatrix();
}

void Entity::setPositionZ(float z)
{
    m_position.z = z;
    computeModelMatrix();
}

void Entity::setRotationX(float x)
{
    m_rotation.x = x;
    computeModelMatrix();
}

void Entity::setRotationY(float y)
{
    m_rotation.y = y;
    computeModelMatrix();
}

void Entity::setRotationZ(float z)
{
    m_rotation.z = z;
    computeModelMatrix();
}

void Entity::setScaleX(float x)
{
    m_scale.x = x;
    computeModelMatrix();
}

void Entity::setScaleY(float y)
{
    m_scale.y = y;
    computeModelMatrix();
}

void Entity::setScaleZ(float z)
{
    m_scale.z = z;
    computeModelMatrix();
}

void Entity::computeModelMatrix()
{
    m_modelMatrix = Matrix::createTransformationMatrix(m_position,m_rotation,m_scale);
}
