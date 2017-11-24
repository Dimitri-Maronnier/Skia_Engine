#ifndef EQUIRECTANGULARTOCUBEMAPSHADER_H
#define EQUIRECTANGULARTOCUBEMAPSHADER_H
#include "src/Engine/Shaders/shaderprogram.h"
#include <glm/mat4x4.hpp>

class EquirectangularToCubemapShader:public ShaderProgram
{
public:
    EquirectangularToCubemapShader():ShaderProgram()
    {

    }

    void connectTextureUnits();

    void loadProjection(glm::mat4 projection);
    void loadView(glm::mat4 view);

protected:
    void bindAttributes();
    void getAllUniformLocations();

private:
    int location_equirectangularMap;
    int location_projection;
    int location_view;
};

#endif // EQUIRECTANGULARTOCUBEMAPSHADER_H
