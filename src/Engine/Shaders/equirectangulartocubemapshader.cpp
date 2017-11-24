#include "equirectangulartocubemapshader.h"


void EquirectangularToCubemapShader::bindAttributes(){

}

void EquirectangularToCubemapShader::getAllUniformLocations(){

    location_equirectangularMap = ShaderProgram::getUniformLocation("equirectangularMap");
    location_view = ShaderProgram::getUniformLocation("view");
    location_projection = ShaderProgram::getUniformLocation("projection");
}

void EquirectangularToCubemapShader::connectTextureUnits(){
    ShaderProgram::loadInt(location_equirectangularMap,0);
}

void EquirectangularToCubemapShader::loadProjection(glm::mat4 projection)
{
    ShaderProgram::loadMatrix(location_projection,projection);
}

void EquirectangularToCubemapShader::loadView(glm::mat4 view)
{
    ShaderProgram::loadMatrix(location_view,view);
}
