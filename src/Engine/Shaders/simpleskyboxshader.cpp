#include "simpleskyboxshader.h"

void SimpleSkyboxShader::bindAttributes(){

}

void SimpleSkyboxShader::getAllUniformLocations(){

    location_environmentMap = ShaderProgram::getUniformLocation("environmentMap");
    location_view = ShaderProgram::getUniformLocation("view");
    location_projection = ShaderProgram::getUniformLocation("projection");
}

void SimpleSkyboxShader::connectTextureUnits(){
    ShaderProgram::loadInt(location_environmentMap,0);
}

void SimpleSkyboxShader::loadProjection(glm::mat4 projection)
{
    ShaderProgram::loadMatrix(location_projection,projection);
}

void SimpleSkyboxShader::loadView(glm::mat4 view)
{
    ShaderProgram::loadMatrix(location_view,view);
}
