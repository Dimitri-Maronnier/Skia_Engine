#ifndef CAMERATHIRD_H
#define CAMERATHIRD_H
#include "src/utils.h"
#include "glm/mat4x4.hpp"
#include "camera.h"
#include "entity.h"

class CameraThird: public Camera
{
public:
    CameraThird(float distanceFromPivot);
    CameraThird();
    void move(float dx, float dy , int dw, int z, int s, int q, int d);
    void move(float dx, float dy , int dw, Entity *e);
    glm::mat4 getViewMatrix();
    void setDistanceFromPivot(float d);

private:
    glm::vec3 lookAt;

    float distanceFromPivot;
    float angleAroundPivot;


    glm::mat4 viewMatrix;
    void calculateCameraPosition(float hD, float vD);
    float calculateHorizontal();
    float calculateVertical();
};

#endif // CAMERATHIRD_H
