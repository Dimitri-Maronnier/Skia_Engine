#ifndef SHADER_H
#define SHADER_H
#include "shaderprogram.h"
#include "src/Engine/Entities/camerathird.h"
#include "src/Engine/Utils/matrix.h"

class Shader: virtual public ShaderProgram
{
private:
    char* vertexFile;
    char* fragmentFile;
    int location_projectionMatrix;
    int location_modelMatrix;
    int location_viewMatrix;
    int location_lightPosition;
    int location_lightTint;
    int location_diffuseMap;
    int location_viewPos;
    int location_time;

    bool m_isInit;

public:
    Shader():ShaderProgram()
    {
        m_isInit = false;
    }

    void connectTextureUnits();

    void loadProjectionMatrix(glm::mat4 matrix);
    void loadModelMatrix(glm::mat4 matrix);
    void loadViewMatrix(glm::vec3 viewPos, glm::mat4 matrix);
    void loadLightPosition(glm::vec3 position);
    void loadLightTint(glm::vec3 tint);
    void loadTime(float time);

    bool isInit();

protected:
    void bindAttributes();
    void getAllUniformLocations();

};

#endif // SHADER_H
