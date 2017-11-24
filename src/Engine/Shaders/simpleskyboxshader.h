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

    void loadProjection(glm::mat4 projection);
    void loadView(glm::mat4 view);

protected:
    void bindAttributes();
    void getAllUniformLocations();

private:
    int location_environmentMap;
    int location_projection;
    int location_view;
};

#endif // SIMPLESKYBOXSHADER_H
