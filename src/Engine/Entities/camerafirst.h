#ifndef CAMERAFIRST_H
#define CAMERAFIRST_H
#include "camera.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "src/utils.h"
#include "src/Engine/Utils/matrix.h"

class CameraFirst: public Camera
{
public:
    CameraFirst();
    CameraFirst(float distanceFromPivot);
    void move(float dx, float dy , int dw, int z, int s, int q, int d);

    glm::mat4 getViewMatrix();
private:
    glm::vec3 m_orientation;
    glm::vec3 m_axeVertical;
    glm::vec3 m_deplacementLateral;
    glm::vec3 m_position;
    glm::vec3 m_pointCible;
    glm::mat4 viewMatrix;
    float distance;
};

#endif // CAMERAFIRST_H
