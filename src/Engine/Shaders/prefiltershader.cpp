#include "prefiltershader.h"

void PreFilterShader::bindAttributes(){

}

void PreFilterShader::getAllUniformLocations(){

    location_environmentMap = ShaderProgram::getUniformLocation("environmentMap");
    location_view = ShaderProgram::getUniformLocation("view");
    location_projection = ShaderProgram::getUniformLocation("projection");
    location_roughness = ShaderProgram::getUniformLocation("roughness");
}

void PreFilterShader::connectTextureUnits(){
    ShaderProgram::loadInt(location_environmentMap,0);
}

void PreFilterShader::loadProjection(glm::mat4 projection)
{
    ShaderProgram::loadMatrix(location_projection,projection);
}

void PreFilterShader::loadView(glm::mat4 view)
{
    ShaderProgram::loadMatrix(location_view,view);
}

void PreFilterShader::loadRoughness(float roughness)
{
    ShaderProgram::loadFloat(location_roughness,roughness);
}
