#include "entity.h"
#include "src/Engine/Utils/matrix.h"
#include <iostream>
#include "../Utils/smath.h"

std::map<QString,int> Entity::labels;

Entity::Entity()
{
    _speed = glm::vec3(0);
    _position = glm::vec3(0);
    _rotation = glm::vec3(0);
    _scale = glm::vec3(1);
    _label = checkoutLabel("UndefinedLabel");
    _parent = nullptr;
    computeModelMatrix();
}

Entity::~Entity(){}

Entity::Entity(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, QString label,unsigned int handle,std::string name,std::string path){
    _speed = glm::vec3(0);
    _position = position;
    _rotation = rotation;
    _scale = scale;
    _label = checkoutLabel(label);
    _handle = handle;
    _name = name;
    _path = path;
    _parent = nullptr;
    computeModelMatrix();
}

glm::vec3 Entity::getPosition(){
    return _position;
}

glm::vec3 Entity::getRotation(){
    return _rotation;
}

void Entity::setSpeed(glm::vec3 newSpeed){
    _speed = newSpeed;
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
    for(Entity*entity:_childs){
        entity->transformFromParent(_modelMatrix);
    }
    _rotation = rotation;
    computeRotationMatrix();
    computeModelMatrix();
}

void Entity::setScale(glm::vec3 scale){
    _scale = scale;
    computeModelMatrix();
}

void Entity::setLabel(QString label){
    _label = checkoutLabel(label);
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
    _forwardVector = glm::normalize(glm::vec3(_modelMatrix[2][0],_modelMatrix[2][1],_modelMatrix[2][2])*(-1.0f));
    _rightVector = glm::normalize(glm::vec3(_modelMatrix[0][0],_modelMatrix[0][1],_modelMatrix[0][2]));
    /*for(unsigned int i=0;i<8;i++){
        _boundingBox[i] *= _modelMatrix;
    }*/
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

std::vector<Entity *> Entity::getChilds()
{
    return _childs;
}

void Entity::setParent(Entity *parent)
{
    _parent = parent;
}

Entity* Entity::getParent()
{
    return _parent;
}

void Entity::serialization(QDataStream& dataStream)
{
    dataStream << _childs.size();
    dataStream << _label;
    dataStream << _position.x;
    dataStream << _position.y;
    dataStream << _position.z;
    dataStream << _rotation.x;
    dataStream << _rotation.y;
    dataStream << _rotation.z;
    dataStream << _scale.x;
    dataStream << _scale.y;
    dataStream << _scale.z;
    dataStream << FolderGestion::removeProjectPath(FolderGestion::checkoutReferences(QString(_path.c_str()))).toStdString().c_str();

    foreach (Entity* entity, _childs) {
        entity->serialization(dataStream);
    }
}

glm::vec3 Entity::getForwardVector()
{
    return _forwardVector;
}

glm::vec3 Entity::getRightVector()
{
    return _rightVector;
}

glm::vec3 Entity::getSpeed()
{
    return _speed;
}

int Entity::getTag()
{
    return _tag;
}

float Entity::getRadius()
{
    return _sphericRadius;
}

glm::vec3 Entity::getCenter()
{
    return _sphereColliderCenter;
}

bool Entity::isDynamic()
{
    return _dynamic;
}

glm::vec3 *Entity::getBoundingBox()
{
    return _boundingBox;
}

void Entity::setTag(int t)
{
    _tag = t;
}

void Entity::setRadius(float r)
{
    _sphericRadius = r;
}

bool Entity::sphereCollide(Entity that, Collision &c){
    c = sphereCollide(that);
    return c.valid;
}
/*
bool Entity::sphereToBBoxCollide(Entity that, Collision &c){
    c = sphereToBBoxCollide(Entity that);
    return c.valid;
}

Collision Entity::sphereToBBoxCollide(Entity that){
    glm::vec3 BBoxCenter = that.getBoundingBox()[0];
    for(int i=1;i<8;i++)
        BBoxcenter += that.getBoundingBox()[i];
    BBoxCenter /= 8;
    float BsphereRadius = glm::length(BBox_center - that.getBoundingBox()[0]);
    BBoxCenter += that._position;
    glm::vec3 thisSphCenter = this->_sphereColliderCenter + this->_position;
    // test si dans la sphere englobante
    if(glm::length(BBoxCenter - thisSphCenter)< this->_sphericRadius + BsphereRadius){
        glm::vec3 closest = that.getBoundingBox()[0]* that.getModelMatrix();
        float closestDist = glm::length(closest - thisSphCenter);
        int closestID =0;
        for(int i=1;i<8; i++){
            glm::vec3 current = that.getBoundingBox()[i]* that.getModelMatrix();
            float currentDist = glm::length(current - thisSphCenter);
            if(currentdist < closestDist){
                closestDist = currentDist;
                closest = current;
                closestID =i;
            }
            int planeBuddy[3];
            planeBuddy[0] = i ^ 1;
            planeBuddy[1] = i ^ 2;
            planeBuddy[2] = i ^ 4;

            // first plane:
            // origine is closest
            glm::vec3 unitVec1 = that.getBoundingBox()[planeBuddy[0]]* that.getModelMatrix() - closest ;
            glm::vec3 unitVec2 = that.getBoundingBox()[planeBuddy[1]]* that.getModelMatrix() - closest;
            glm::vec3 normalVec = glm::cross(unitVec1, unitVec2);
            glm::vec3 diffVector = thisSphCenter - closest;
            float dot = glm::dot(diffVector,normalVec);
            glm::vec3 projPoint = thisSphCenter - dot* normal;
            float distProj = glm::length(projPoint-thisSphCenter);
            if(distProj< this->_sphericRadius)

        }

    }
    Collision c;
    c.valid = false;
    return c;
}
*/
bool Entity::isVisible()
{
    return _visible;
}

void Entity::setVisible(bool visible)
{
    _visible = visible;
}

Collision Entity::sphereCollide(Entity that){
    glm::vec3 thisSphereCenter = this->_sphereColliderCenter + this->_position;
    glm::vec3 thatSphereCenter = that._sphereColliderCenter + that._position;

    glm::vec3 vecDiff = thatSphereCenter - thisSphereCenter ;

    if(glm::length(vecDiff) < this->_sphericRadius + that._sphericRadius){
        glm::vec3 normale= glm::normalize(vecDiff);
        glm::vec3 point = normale * that._sphericRadius;
        return Collision(that._label, normale, point, that.getTag());
    }
    Collision c;
    c.valid= false;
    return c;
}



QString Entity::checkoutLabel(const QString &label)
{
    labels[label]++;
    QString computeLabel;
    if(labels[label]==1)
        computeLabel = label;
    else
        computeLabel = label + "_" + QString::number(labels[label]);
    return computeLabel;
}

