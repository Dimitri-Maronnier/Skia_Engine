#include "shadowpassshader.h"

void ShadowPassShader::bindAttributes()
{

}

void ShadowPassShader::getAllUniformLocations()
{
    location_lightMatrix = ShaderProgram::getUniformLocation("lightMatrix");
    location_modelMatrix = ShaderProgram::getUniformLocation("modelMatrix");
}


void ShadowPassShader::loadLightMatrix(glm::mat4 matrix)
{
    ShaderProgram::loadMatrix(location_lightMatrix,matrix);
}

void ShadowPassShader::loadModelMatrix(glm::mat4 matrix)
{
    ShaderProgram::loadMatrix(location_modelMatrix,matrix);
}

