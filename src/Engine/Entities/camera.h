#ifndef CAMERA_H
#define CAMERA_H
#include "glm/mat4x4.hpp"

class Camera
{
public:
    Camera();
    glm::vec3 getPosition();
    float getPitch();
    float getYaw();
    float getRoll();
    void invertRoll();
    float getFOV();
    float getNEAR_PLANE();
    float getFAR_PLANE();
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    glm::mat4 getProjectionMatrix();

protected:
    glm::vec3 position;
    glm::vec3 lookAt;
    float pitch;
    float roll;
    float yaw;
    float FOV;
    float NEAR_PLANE;
    float FAR_PLANE;

private:




    glm::mat4 projectionMatrix;
};

#endif // CAMERA_H
