#ifndef SIMPLESKYBOXSHADER_H
#define SIMPLESKYBOXSHADER_H
#include "src/Engine/Shaders/shaderprogram.h"
#include <glm/mat4x4.hpp>

class SimpleSkyboxShader : public ShaderProgram
{
public:
    SimpleSkyboxShader():ShaderProgram()
    {

    }

    void connectTextureUnits();

    void loadProjectionMatrix(glm::mat4 projectionMatrix);
    void loadViewMatrix(glm::mat4 viewMatrix);

protected:
    void bindAttributes();
    void getAllUniformLocations();

private:
    int location_environmentMap;
    int location_projectionMatrix;
    int location_viewMatrix;
};

#endif // SIMPLESKYBOXSHADER_H
