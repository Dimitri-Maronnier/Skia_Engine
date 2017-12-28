#ifndef SPRINGCAMERA_H
#define SPRINGCAMERA_H

#include "src/Engine/Entities/camera.h"

class SpringCamera:public Camera
{
public:
    SpringCamera();
    glm::mat4 getViewMatrix();
};

#endif // SPRINGCAMERA_H
