#include "camera.h"

Camera::Camera()
{

}

glm::vec3 Camera::getPosition(){
    return position;
}

float Camera::getPitch(){
    return pitch;
}

float Camera::getYaw(){
    return yaw;
}

float Camera::getRoll(){
    return roll;
}

void Camera::invertRoll(){
    roll = -roll;
}

float Camera::getFOV(){
  return FOV;
}

float Camera::getNEAR_PLANE(){
   return NEAR_PLANE;
}

float Camera::getFAR_PLANE(){
  return FAR_PLANE;
}

void Camera::setProjectionMatrix(glm::mat4 projectionMatrix){
    this->projectionMatrix = projectionMatrix;
}

glm::mat4 Camera::getProjectionMatrix(){
    return projectionMatrix;
}
