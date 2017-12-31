#ifndef PREFILTERSHADER_H
#define PREFILTERSHADER_H
#include <glm/mat4x4.hpp>
#include "src/Engine/Shaders/shaderprogram.h"

class PreFilterShader:public ShaderProgram
{
public:
    PreFilterShader():ShaderProgram()
    {

    }

    void connectTextureUnits();

    void loadProjection(glm::mat4 projection);
    void loadView(glm::mat4 view);
    void loadRoughness(float roughness);
    void loadNbSample(int n);
    void loadResolution(float resolution);

protected:
    void bindAttributes();
    void getAllUniformLocations();

private:
    int location_environmentMap;
    int location_projection;
    int location_view;
    int location_roughness;
    int location_nbSample;
    int location_resolution;
};

#endif // PREFILTERSHADER_H
