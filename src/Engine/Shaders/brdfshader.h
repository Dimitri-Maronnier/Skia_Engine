#ifndef BRDFSHADER_H
#define BRDFSHADER_H
#include "src/Engine/Shaders/shaderprogram.h"

class BRDFShader:public ShaderProgram
{
public:
    BRDFShader():ShaderProgram(){

    }
    void bindAttributes();
    void getAllUniformLocations();
};

#endif // BRDFSHADER_H
