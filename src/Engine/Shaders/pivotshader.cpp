#include "pivotshader.h"

void PivotShader::bindAttributes(){
}

void PivotShader::getAllUniformLocations(){

    location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
    location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
    location_modelMatrix = ShaderProgram::getUniformLocation("modelMatrix");
    location_aPoint = ShaderProgram::getUniformLocation("a");
    location_bPoint = ShaderProgram::getUniformLocation("b");
    location_viewPos = ShaderProgram::getUniformLocation("viewPos");
    location_color = ShaderProgram::getUniformLocation("color");
}

void PivotShader::loadProjectionMatrix(glm::mat4 matrix){
    ShaderProgram::loadMatrix(location_projectionMatrix, matrix);
}

void PivotShader::loadModelMatrix(glm::mat4 matrix)
{
    ShaderProgram::loadMatrix(location_modelMatrix,matrix);
}

void PivotShader::loadViewMatrix(glm::vec3 viewPos, glm::mat4 matrix){
    ShaderProgram::loadMatrix(location_viewMatrix, matrix);
    ShaderProgram::loadVector(location_viewPos, viewPos);
}

void PivotShader::loadPoint(glm::vec3 a, glm::vec3 b)
{
    ShaderProgram::loadVector(location_aPoint,a);
    ShaderProgram::loadVector(location_bPoint,b);
}
