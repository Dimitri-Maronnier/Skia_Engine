#include "simpleskyboxshader.h"

void SimpleSkyboxShader::bindAttributes(){

}

void SimpleSkyboxShader::getAllUniformLocations(){

    location_environmentMap = ShaderProgram::getUniformLocation("environmentMap");
    location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
    location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
}

void SimpleSkyboxShader::connectTextureUnits(){
    ShaderProgram::loadInt(location_environmentMap,0);
}

void SimpleSkyboxShader::loadProjectionMatrix(glm::mat4 projectionMatrix)
{
    ShaderProgram::loadMatrix(location_projectionMatrix,projectionMatrix);
}

void SimpleSkyboxShader::loadViewMatrix(glm::mat4 viewMatrix)
{
    ShaderProgram::loadMatrix(location_viewMatrix,viewMatrix);
}
