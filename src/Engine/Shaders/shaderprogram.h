#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <iostream>
#include <GL/glew.h>
#include <QGLWidget>
#include <fstream>
#include "glm/mat4x4.hpp"


class ShaderProgram
{
public:
    ShaderProgram();
    void init(QString vertexStr,QString fragmentStr);
    void init(QString vertexStr,QString geoStr,QString fragmentStr);
    void initWithSource(QString vertexStr,QString fragmentStr);
    void start();
    void stop();
    void cleanUp();

protected:
    virtual void getAllUniformLocations();
    virtual void bindAttributes();
    void bindAttribute(int attribute, const char *variableName);
    void loadBoolean(int location, bool value);

    void loadMatrix(int location,glm::mat4  matrix );

    void loadInt(int location, int value);
    void loadFloat(int location, float value);

    void loadVector(int location, glm::vec3 vector);
    int getUniformLocation(QString uniformName);

private:
    int programID;
    int vertexID;
    int fragmentID;
    int geoID;

    int loadShader(QString fileName,int type);
    int loadShaderWithSource(QString source ,int type);
};

#endif // SHADERPROGRAM_H
