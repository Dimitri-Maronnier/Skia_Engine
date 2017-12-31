#ifndef IRRADIANCECONVOLUTIONSHADER_H
#define IRRADIANCECONVOLUTIONSHADER_H

#include <glm/mat4x4.hpp>
#include "src/Engine/Shaders/shaderprogram.h"

class IrradianceConvolutionShader :public ShaderProgram
{
public:
    IrradianceConvolutionShader():ShaderProgram()
    {

    }

    void connectTextureUnits();

    void loadProjection(glm::mat4 projection);
    void loadView(glm::mat4 view);
    void loadDelta(float delta);

protected:
    void bindAttributes();
    void getAllUniformLocations();

private:
    int location_environmentMap;
    int location_projection;
    int location_view;
    int location_delta;
};


#endif // IRRADIANCECONVOLUTIONSHADER_H
