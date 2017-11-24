#include "shader.h"

void Shader::bindAttributes(){
    ShaderProgram::bindAttribute(0,"position");
    ShaderProgram::bindAttribute(1,"normal");
    ShaderProgram::bindAttribute(2,"textureCoord");
    ShaderProgram::bindAttribute(3,"tangent");
}

void Shader::getAllUniformLocations(){

    location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
    location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
    location_modelMatrix = ShaderProgram::getUniformLocation("modelMatrix");
    location_lightPosition = ShaderProgram::getUniformLocation("lightPosition");
    location_lightTint = ShaderProgram::getUniformLocation("lightTint");
    location_viewPos = ShaderProgram::getUniformLocation("viewPos");
    location_time = ShaderProgram::getUniformLocation("time");

    m_isInit = true;
}

void Shader::connectTextureUnits(){
    //ShaderProgram::loadInt(location_diffuseMap,0);
}

void Shader::loadProjectionMatrix(glm::mat4 matrix){
    ShaderProgram::loadMatrix(location_projectionMatrix, matrix);
}

void Shader::loadModelMatrix(glm::mat4 matrix)
{
    ShaderProgram::loadMatrix(location_modelMatrix,matrix);
}

void Shader::loadViewMatrix(glm::vec3 viewPos,glm::mat4 matrix){
    ShaderProgram::loadMatrix(location_viewMatrix, matrix);
    ShaderProgram::loadVector(location_viewPos,viewPos);
}

void Shader::loadLightPosition(glm::vec3 position){
    ShaderProgram::loadVector(location_lightPosition,position);
}

void Shader::loadLightTint(glm::vec3 tint){
    ShaderProgram::loadVector(location_lightTint,tint);
}

void Shader::loadTime(float time)
{
    ShaderProgram::loadFloat(location_time,time);
}

bool Shader::isInit()
{
    return m_isInit;
}
