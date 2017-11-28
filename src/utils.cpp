#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include "utils.h"

Utils::Utils()
{

}

float Utils::toRadians(float angle){
    return angle * M_PI / 180.0 ;
}

std::vector<std::string> Utils::split(std::string str, char delimiter) {
  std::vector<std::string> internal;
  std::stringstream ss(str); // Turn the string into a stream.
  std::string tok;

  while(std::getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}

std::string Utils::stringBetween(std::string str, char start,char stop){
    unsigned first = str.find(start);
    str.at(first) = 0;
    unsigned last = str.find(stop);

    std::string strNew = str.substr (first+1,last-first-1);
    return strNew;
}

QString Utils::removeExtension(QString fileName)
{
    for(int i=fileName.size();i>0;i--){
        if(fileName.at(i) == '.'){
            fileName.remove(i,fileName.size()-i);
            break;
        }
    }
    return fileName;
}

std::string Utils::getExtension(std::string fileName)
{
    size_t pos = fileName.find('.');
    std::string ext = fileName.substr(pos);
    return ext;
}

void Utils::createProcess(std::string cmd){

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    WCHAR wcmd[MAX_PATH];
    int i;
    for(i=0;i<cmd.size();i++)
        wcmd[i] = cmd.at(i);
    wcmd[i] = 0;

    if( !CreateProcess( NULL,   // No module name (use command line)
        wcmd,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NO_WINDOW,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    )
    {
        std::cerr << "CreateProcess failed " << GetLastError() << std::endl;
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

void Utils::shellExecute(std::string arg)
{


    WCHAR wcmd[MAX_PATH];
    int i;
    for(i=0;i<arg.size();i++)
        wcmd[i] = arg.at(i);
    wcmd[i] = 0;
    ShellExecute( NULL,  L"open",
        L"command",
        wcmd,
        L"c:\\",
        SW_SHOWNORMAL
    );

}

TypeVarType Utils::stringToTypeVar(QString str)
{
    if(str.compare("mat4") == 0){
        return mat4;
    }else if(str.compare("mat3") == 0){
        return mat3;
    }else if(str.compare("mat2") == 0){
        return mat2;
    }else if(str.compare("vec4") == 0){
        return vec4;
    }else if(str.compare("vec3") == 0){
        return vec3;
    }else if(str.compare("vec2") == 0){
        return vec2;
    }else if(str.compare("float") == 0){
        return Float;
    }else if(str.compare("bool") == 0){
        return Bool;
    }else{
        return Default;
    }
}

QString Utils::typeVarToString(TypeVarType type)
{
    switch(type){
        case mat4:
            return "mat4";
        case mat3:
            return "mat3";
        case mat2:
            return "mat2";
        case vec4:
            return "vec4";
        case vec3:
            return "vec3";
        case vec2:
            return "vec2";
        case Float:
            return "float";
        case Bool:
            return "bool";
        default:
            return "";
    }
}

TypeVarType Utils::extToTypeVar(QString ext)
{
    std::vector<std::string> split = Utils::split(ext.toStdString(),'.');

    if(split.at(1).size()==1){
        return Float;
    }else if(split.at(1).size()==2){
        return vec2;
    }else if(split.at(1).size()==3){
        return vec3;
    }
}

TypeVarType Utils::above(TypeVarType type)
{
    switch(type){
        case vec4:
            return vec4;
        case vec3:
            return vec4;
        case vec2:
            return vec3;
        case Float:
            return vec2;
        case Bool:
            return Bool;
        default:
            return Float;
    }
}

TypeVarType Utils::addTypeVar(TypeVarType type1, TypeVarType type2)
{
    switch(type1){
        case vec3:
            switch(type2){
                case Float:
                    return vec4;
                default:
                    return Float;
            }
        case vec2:
            switch(type2){

                case vec2:
                    return vec4;
                case Float:
                    return vec3;
                default:
                    return Float;
            }
        case Float:
            switch(type2){

                case vec3:
                    return vec4;
                case vec2:
                    return vec3;
                case Float:
                    return vec2;
                default:
                    return Float;
            }
        default:
            return Float;
    }
}

GLuint Utils::equirectangularToCubeMap(GLuint hdrTexture)
{
    GLuint *cubeVAO;
    GLuint *cubeVBO;

    cubeVAO = new GLuint(1);
    cubeVBO = new GLuint(1);

    generateCube(cubeVAO,cubeVBO);

    unsigned int captureFBO;
    glGenFramebuffers(1, &captureFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    EquirectangularToCubemapShader equirectangularToCubemapShader;
    equirectangularToCubemapShader.init("cubemapVertex.glsl","fromEquirectangularToCubemapFragment.glsl");
    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    GLuint envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    equirectangularToCubemapShader.start();
    equirectangularToCubemapShader.connectTextureUnits();
    equirectangularToCubemapShader.loadProjection(captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectangularToCubemapShader.loadView(captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube(*cubeVAO);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    equirectangularToCubemapShader.stop();
    equirectangularToCubemapShader.cleanUp();

    SAFE_DELETE(cubeVAO);
    SAFE_DELETE(cubeVBO);

    return envCubemap;
}

GLuint Utils::irradianceConvolution(GLuint envCubemap)
{
    GLuint *cubeVAO;
    GLuint *cubeVBO;

    cubeVAO = new GLuint(1);
    cubeVBO = new GLuint(1);

    generateCube(cubeVAO,cubeVBO);

    unsigned int captureFBO;
    glGenFramebuffers(1, &captureFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

    IrradianceConvolutionShader irradianceShader;
    irradianceShader.init("cubemapVertex.glsl","PBR_Pipeline/irradiance_convolution.glsl");
    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
    GLuint irradianceMap;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    irradianceShader.start();
    irradianceShader.connectTextureUnits();
    irradianceShader.loadProjection(captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        irradianceShader.loadView( captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube(*cubeVAO);
    }
    irradianceShader.stop();
    irradianceShader.cleanUp();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    SAFE_DELETE(cubeVAO);
    SAFE_DELETE(cubeVBO);

    return irradianceMap;
}

GLuint Utils::prefilterCubeMap(GLuint envCubemap)
{
    GLuint *cubeVAO;
    GLuint *cubeVBO;
    cubeVAO = new GLuint(1);
    cubeVBO = new GLuint(1);
    generateCube(cubeVAO,cubeVBO);

    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    PreFilterShader prefilterShader;
    prefilterShader.init("cubemapVertex.glsl","PBR_Pipeline/prefilter.glsl");

    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };


    GLuint prefilterMap;
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minifcation filter to mip_linear
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    // ----------------------------------------------------------------------------------------------------
    prefilterShader.start();
    prefilterShader.connectTextureUnits();
    prefilterShader.loadProjection(captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader.loadRoughness(roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader.loadView(captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube(*cubeVAO);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    prefilterShader.stop();
    prefilterShader.cleanUp();

    SAFE_DELETE(cubeVAO);
    SAFE_DELETE(cubeVBO);

    return prefilterMap;
}

GLuint Utils::generate2DLut()
{

    GLuint *quadVAO;
    GLuint *quadVBO;

    quadVAO = new GLuint(1);
    quadVBO = new GLuint(1);

    generateQuad(quadVAO,quadVBO);

    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    GLuint brdfLUTTexture;
    glGenTextures(1, &brdfLUTTexture);

    BRDFShader brdfShader;
    brdfShader.init("PBR_Pipeline/brdfVertex.glsl","PBR_Pipeline/brdfFragment.glsl");

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

    glViewport(0, 0, 512, 512);
    brdfShader.start();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderQuad(*quadVAO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    brdfShader.stop();
    brdfShader.cleanUp();

    SAFE_DELETE(quadVAO);
    SAFE_DELETE(quadVBO);

    return brdfLUTTexture;
}

void Utils::generateCube(GLuint *VAO,GLuint *VBO)
{
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
    };
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // link vertex attributes
    glBindVertexArray(*VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Utils::generateQuad(GLuint *VAO, GLuint *VBO)
{

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

}



void Utils::renderCube(GLuint VAO)
{
    // render Cube
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}

void Utils::renderQuad(GLuint VAO)
{


    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

}

void Utils::setupDeferredShading(const unsigned int width,const unsigned int height, GLuint *gBuffer, GLuint *position, GLuint *normal, GLuint *albedoRoughness, GLuint *metalOcclusion)
{
    glGenFramebuffers(1, gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *gBuffer);

    // position color buffer
    glGenTextures(1, position);
    glBindTexture(GL_TEXTURE_2D, *position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *position, 0);
    // normal color buffer
    glGenTextures(1, normal);
    glBindTexture(GL_TEXTURE_2D, *normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, *normal, 0);
    // color + roughness color buffer
    glGenTextures(1, albedoRoughness);
    glBindTexture(GL_TEXTURE_2D, *albedoRoughness);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, *albedoRoughness, 0);
    // metal + occlussion ambient color buffer
    glGenTextures(1, metalOcclusion);
    glBindTexture(GL_TEXTURE_2D, *metalOcclusion);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, *metalOcclusion, 0);



    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT4 };
    glDrawBuffers(4, attachments);
    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
