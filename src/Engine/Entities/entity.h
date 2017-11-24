#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Entity
{
public:
    Entity();
    Entity(glm::vec3 position,glm::vec3 rotation, glm::vec3 scale,std::string label);

    glm::vec3 getPostion();
    glm::vec3 getRotation();
    glm::vec3 getScale();
    std::string getLabel();
    glm::mat4 getModelMatrix();

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);
    void setLabel(std::string label);

    void setPositionX(float x);
    void setPositionY(float y);
    void setPositionZ(float z);

    void setRotationX(float x);
    void setRotationY(float y);
    void setRotationZ(float z);

    void setScaleX(float x);
    void setScaleY(float y);
    void setScaleZ(float z);

    void computeModelMatrix();
private:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    std::string m_label;

    glm::mat4 m_modelMatrix;

};

#endif // ENTITY_H
