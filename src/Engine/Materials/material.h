#ifndef MATERIAL_H
#define MATERIAL_H
#include "GL/glew.h"
#include <QString>
#include "src/Assets/asset.h"
#include "src/Assets/assetscollections.h"
#include "src/Engine/Shaders/shader.h"

class Material: public Asset
{
public:
    Material();
    Material(unsigned int handle,const std::string name,std::string path);
    ~Material();
    static void newEmptyMat(QString path);
    bool isShaderInit();
    Shader getShader();

    std::vector<std::string> getTexturesPath();

    static QString DefaultSourceFragmentShader;
    static QString DefaultSourceVertexShader;
private:
    std::string m_fragmentShader;
    std::string m_vertexShader;

    std::vector<std::string> m_texturespath;

    std::vector<std::string> m_uniforms;

    Shader m_shader;
    bool m_shaderInit;

};

#endif // MATERIAL_H
