#ifndef PIVOTSHADER_H
#define PIVOTSHADER_H
#include "shaderprogram.h"
#include "src/Engine/Utils/matrix.h"

class PivotShader: public ShaderProgram
{
private:
    int location_projectionMatrix;
    int location_modelMatrix;
    int location_viewMatrix;
    int location_viewPos;
    int location_aPoint;
    int location_bPoint;
    int location_color;

public:
    PivotShader():ShaderProgram()
    {

    }

    void loadProjectionMatrix(glm::mat4 matrix);
    void loadModelMatrix(glm::mat4 matrix);
    void loadViewMatrix(glm::vec3 viewPos, glm::mat4 matrix);
    void loadPoint(glm::vec3 a,glm::vec3 b);

protected:
    void bindAttributes();
    void getAllUniformLocations();
};

#endif // PIVOTSHADER_H
