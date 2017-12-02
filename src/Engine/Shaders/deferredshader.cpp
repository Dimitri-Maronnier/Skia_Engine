#include "deferredshader.h"

void DeferredShader::bindAttributes(){

}

void DeferredShader::getAllUniformLocations(){
    location_lightPosition = ShaderProgram::getUniformLocation("lightPosition");
    location_lightTint = ShaderProgram::getUniformLocation("lightTint");
    location_viewPos = ShaderProgram::getUniformLocation("viewPos");
}

void DeferredShader::loadLight(Light light)
{
    ShaderProgram::loadVector(location_lightPosition,light.getPosition());
    ShaderProgram::loadVector(location_lightTint,light.getColor());
}

void DeferredShader::loadViewPos(glm::vec3 viewPos)
{
    ShaderProgram::loadVector(location_viewPos,viewPos);
}
