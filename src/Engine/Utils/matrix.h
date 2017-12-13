#ifndef MATRIX_H
#define MATRIX_H
#include "src/Engine/Entities/camerathird.h"
#include "vmath.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Matrix
{
public:
    Matrix();
    static vmath::mat4 rotate(float angle, glm::vec3 axis, vmath::mat4 src);
    static glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::mat4 rotation, glm::vec3 scale);
    static glm::mat4 createViewMatrix(CameraThird camera);
    static glm::mat4 createProjectionMatrix(Camera camera, int w, int h);


};

#endif // MATRIX_H
