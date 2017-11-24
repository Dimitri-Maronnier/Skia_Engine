#include "irradianceconvolutionshader.h"

void IrradianceConvolutionShader::bindAttributes(){

}

void IrradianceConvolutionShader::getAllUniformLocations(){

    location_environmentMap = ShaderProgram::getUniformLocation("environmentMap");
    location_view = ShaderProgram::getUniformLocation("view");
    location_projection = ShaderProgram::getUniformLocation("projection");
}

void IrradianceConvolutionShader::connectTextureUnits(){
    ShaderProgram::loadInt(location_environmentMap,0);
}

void IrradianceConvolutionShader::loadProjection(glm::mat4 projection)
{
    ShaderProgram::loadMatrix(location_projection,projection);
}

void IrradianceConvolutionShader::loadView(glm::mat4 view)
{
    ShaderProgram::loadMatrix(location_view,view);
}
