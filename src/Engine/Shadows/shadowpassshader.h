#ifndef SHADOWPASSSHADER_H
#define SHADOWPASSSHADER_H
#include "src/Engine/Shaders/shaderprogram.h"

class ShadowPassShader: virtual public ShaderProgram
{
private:
    char* vertexFile;
    char* fragmentFile;
    int location_lightMatrix;
    int location_modelMatrix;

public:
    ShadowPassShader():ShaderProgram()
    {
    }


    void loadLightMatrix(glm::mat4 matrix);
    void loadModelMatrix(glm::mat4 matrix);

protected:
    void bindAttributes();
    void getAllUniformLocations();
};

#endif // SHADOWPASSSHADER_H
