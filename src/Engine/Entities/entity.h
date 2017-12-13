#ifndef ENTITY_H
#define ENTITY_H
#include <QString>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/geometric.hpp>
#include<glm/common.hpp>

class Entity
{
public:
    /**
     * @brief Entity
     */
    Entity();
    /**
     * @brief Entity
     * @param position
     * @param rotation
     * @param scale
     * @param label
     */
    Entity(glm::vec3 position,glm::vec3 rotation, glm::vec3 scale,QString label,unsigned int handle,std::string name,std::string path);

    /**
     * @brief getPostion
     * @return
     */
    glm::vec3 getPostion();
    /**
     * @brief getRotation
     * @return
     */
    glm::vec3 getRotation();
    /**
     * @brief getScale
     * @return
     */
    glm::vec3 getScale();
    /**
     * @brief getLabel
     * @return
     */
    QString getLabel();
    /**
     * @brief getModelMatrix
     * @return
     */
    glm::mat4 getModelMatrix();

    /**
     * @brief setPosition
     * @param position
     */
    void setPosition(glm::vec3 position);
    /**
     * @brief setRotation
     * @param rotation
     */
    void setRotation(glm::vec3 rotation);
    /**
     * @brief setScale
     * @param scale
     */
    void setScale(glm::vec3 scale);
    /**
     * @brief setLabel
     * @param label
     */
    void setLabel(QString label);

    /**
     * @brief setPositionX
     * @param x
     */
    void setPositionX(float x);
    /**
     * @brief setPositionY
     * @param y
     */
    void setPositionY(float y);
    /**
     * @brief setPositionZ
     * @param z
     */
    void setPositionZ(float z);

    /**
     * @brief setRotationX
     * @param x
     */
    void setRotationX(float x);
    /**
     * @brief setRotationY
     * @param y
     */
    void setRotationY(float y);
    /**
     * @brief setRotationZ
     * @param z
     */
    void setRotationZ(float z);

    /**
     * @brief setScaleX
     * @param x
     */
    void setScaleX(float x);
    /**
     * @brief setScaleY
     * @param y
     */
    void setScaleY(float y);
    /**
     * @brief setScaleZ
     * @param z
     */
    void setScaleZ(float z);

    /**
     * @brief computeModelMatrix
     */
    void computeModelMatrix();

    void computeRotationMatrix();
    void computeRotationMatrixQuat(glm::quat quat);

    void transformFromParent(glm::mat4 parentModelMatrix);

    void addChild(Entity* child);
    void removeChild(Entity* child);

    void setParent(Entity* parent);

protected:
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;

    QString _label;
    std::vector<Entity*> _childs;
    Entity* _parent;
    glm::mat4 _modelMatrix;
    glm::mat4 _rotationMatrix;
    unsigned int _handle;
    std::string _name;
    std::string _path;

};

#endif // ENTITY_H
