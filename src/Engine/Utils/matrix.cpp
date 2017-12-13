#include "matrix.h"
#include <iostream>
#include <math.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>



Matrix::Matrix()
{

}

glm::mat4 Matrix::createViewMatrix(CameraThird camera){

    glm::mat4 viewMatrix = glm::mat4(1.0);

    viewMatrix = glm::rotate(viewMatrix,Utils::toRadians(camera.getPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix,Utils::toRadians(camera.getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix,Utils::toRadians(camera.getRoll()), glm::vec3(0.0f, 0.0f, 1.0f));
    viewMatrix = glm::translate(viewMatrix, glm::vec3(-camera.getPosition().x,-camera.getPosition().y,-camera.getPosition().z));
   /* viewMatrix = rotate(Utils::toRadians(camera.getPitch()), Vector(1.0f, 0.0f, 0.0f),viewMatrix);
    viewMatrix = rotate(Utils::toRadians(camera.getYaw()), Vector(0.0f, 1.0f, 0.0f),viewMatrix);
    viewMatrix = rotate(Utils::toRadians(camera.getRoll()), Vector(0.0f, 0.0f, 1.0f),viewMatrix);
    /*vmath::vec3 negativeCameraPos(-camera.getPosition().x,-camera.getPosition().y,-camera.getPosition().z);
    viewMatrix = vmath::translate(negativeCameraPos);*/

    return viewMatrix;

}

glm::mat4 Matrix::createTransformationMatrix(glm::vec3 translation, glm::mat4 rotation, glm::vec3 scale){
    glm::mat4 matrix = glm::mat4(1.0);
    matrix = glm::translate(matrix,translation);

    /*matrix = glm::rotate(matrix,Utils::toRadians(rotation.x), glm::vec3(1.0,0.0,0.0));
    matrix = glm::rotate(matrix,Utils::toRadians(rotation.y), glm::vec3(0.0,1.0,0.0));
    matrix = glm::rotate(matrix,Utils::toRadians(rotation.z), glm::vec3(0.0,0.0,1.0));*/

    glm::mat4 matrixScale = glm::scale(glm::mat4(),scale);
    return matrix*rotation*matrixScale;
}

glm::mat4 Matrix::createProjectionMatrix(Camera camera, int w, int h) {
    glm::mat4 projectionMatrix = glm::mat4();
    float aspectRatio = (float) w / (float) h;
    float y_scale = (float)((1.0f / tan(Utils::toRadians(camera.getFOV() / 2.0f))) * aspectRatio);
    float x_scale = y_scale / aspectRatio;
    float frustum_length = camera.getFAR_PLANE() - camera.getNEAR_PLANE();

    projectionMatrix[0][0] = x_scale;
    projectionMatrix[1][1] = y_scale;
    projectionMatrix[2][2] = -((camera.getFAR_PLANE() + camera.getNEAR_PLANE()) / frustum_length);
    projectionMatrix[2][3] = -1;
    projectionMatrix[3][2] = -((2 * camera.getNEAR_PLANE() * camera.getFAR_PLANE()) / frustum_length);
    projectionMatrix[3][3] = 0;

    return projectionMatrix;
}

vmath::mat4 Matrix::rotate(float angle, glm::vec3 axis, vmath::mat4 src) {

    vmath::mat4 dest;
    float c = (float) cos(angle);
    float s = (float) sin(angle);
    float oneminusc = 1.0f - c;
    float xy = axis.x*axis.y;
    float yz = axis.y*axis.z;
    float xz = axis.x*axis.z;
    float xs = axis.x*s;
    float ys = axis.y*s;
    float zs = axis.z*s;

    float f00 = axis.x*axis.x*oneminusc+c;
    float f01 = xy*oneminusc+zs;
    float f02 = xz*oneminusc-ys;
    // n[3] not used
    float f10 = xy*oneminusc-zs;
    float f11 = axis.y*axis.y*oneminusc+c;
    float f12 = yz*oneminusc+xs;
    // n[7] not used
    float f20 = xz*oneminusc+ys;
    float f21 = yz*oneminusc-xs;
    float f22 = axis.z*axis.z*oneminusc+c;

    float t00 = src[0][0] * f00 + src[1][0] * f01 + src[2][0] * f02;
    float t01 = src[0][1] * f00 + src[1][1] * f01 + src[2][1] * f02;
    float t02 = src[0][2] * f00 + src[1][2] * f01 + src[2][2] * f02;
    float t03 = src[0][3] * f00 + src[1][3] * f01 + src[2][3] * f02;
    float t10 = src[0][0] * f10 + src[1][0] * f11 + src[2][0] * f12;
    float t11 = src[0][1] * f10 + src[1][1] * f11 + src[2][1] * f12;
    float t12 = src[0][2] * f10 + src[1][2] * f11 + src[2][2] * f12;
    float t13 = src[0][3] * f10 + src[1][3] * f11 + src[2][3] * f12;
    dest[2][0] = src[0][0] * f20 + src[1][0] * f21 + src[2][0] * f22;
    dest[2][1] = src[0][1] * f20 + src[1][1] * f21 + src[2][1] * f22;
    dest[2][2] = src[0][2] * f20 + src[1][2] * f21 + src[2][2] * f22;
    dest[2][3] = src[0][3] * f20 + src[1][3] * f21 + src[2][3] * f22;
    dest[0][0] = t00;
    dest[0][1] = t01;
    dest[0][2] = t02;
    dest[0][3] = t03;
    dest[1][0] = t10;
    dest[1][1] = t11;
    dest[1][2] = t12;
    dest[1][3] = t13;
    return dest;
}

