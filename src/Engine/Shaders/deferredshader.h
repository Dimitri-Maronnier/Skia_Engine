#ifndef DEFERREDSHADER_H
#define DEFERREDSHADER_H

#include "src/Engine/Shaders/shaderprogram.h"
#include "src/Engine/Entities/light.h"
#include <glm/vec3.hpp>

class DeferredShader: public ShaderProgram
{
public:
    DeferredShader():ShaderProgram(){

    }
    void bindAttributes();
    void getAllUniformLocations();

    void loadLight(Light light);
    void loadViewPos(glm::vec3 viewPos);

private:
    int location_lightPosition;
    int location_lightTint;
    int location_viewPos;
};

#endif // DEFERREDSHADER_H
