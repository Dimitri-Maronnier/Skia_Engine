#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>
#include <sstream>
#include "Editor/Materials/Nodes/nodematerial.h"
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Shaders/equirectangulartocubemapshader.h"
#include "Engine/Shaders/irradianceconvolutionshader.h"
#include "Engine/Shaders/prefiltershader.h"
#include "Engine/Shaders/brdfshader.h"

class Utils
{
public:
    Utils();
    static float toRadians(float angle);
    static std::vector<std::string> split(std::string str, char delimiter);
    static std::string stringBetween(std::string str, char start,char stop);
    static QString removeExtension(QString fileName);
    static std::string getExtension(std::string fileName);
    static void createProcess(std::string cmd);
    static void shellExecute(std::string arg);
    static TypeVarType stringToTypeVar(QString str);
    static QString typeVarToString(TypeVarType type);
    static TypeVarType extToTypeVar(QString ext);
    static TypeVarType above(TypeVarType type);
    static TypeVarType addTypeVar(TypeVarType type1,TypeVarType type2);
    static GLuint equirectangularToCubeMap(GLuint hdrTexture);
    static GLuint irradianceConvolution(GLuint envCubemap);
    static GLuint prefilterCubeMap(GLuint envCubemap);
    static GLuint generate2DLut();
    static void generateCube(GLuint *VAO,GLuint *VBO);
    static void generateQuad(GLuint *VAO,GLuint *VBO);
    static void renderCube(GLuint VAO);
    static void renderQuad(GLuint VAO);
    static void setupDeferredShading(const unsigned int width, const unsigned int height, GLuint *gBuffer, GLuint *position, GLuint *normal, GLuint *albedoRoughness, GLuint *metalOcclusion);

private:

};

#endif // UTILS_H
