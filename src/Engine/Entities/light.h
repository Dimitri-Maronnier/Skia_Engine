#ifndef LIGHT_H
#define LIGHT_H
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Light
{
public:
    /**
     * @brief Light
     */
    Light();
    /**
     * @brief Light
     * @param position
     * @param color
     */
    Light(glm::vec3 position,glm::vec3 color);
    /**
     * @brief getPosition
     * @return
     */
    glm::vec3 getPosition();
    /**
     * @brief getColor
     * @return
     */
    glm::vec3 getColor();
    /**
     * @brief getLightViewMatrix
     * @return
     */
    glm::mat4 getLightViewMatrix();

    /**
     * @brief setPosition
     * @param position
     */
    void setPosition(glm::vec3 position);
    /**
     * @brief setColor
     * @param color
     */
    void setColor(glm::vec3 color);
private:
    glm::vec3 _position;
    glm::vec3 _color;

};

#endif // LIGHT_H
