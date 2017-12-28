#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include "src/Engine/scene.h"
#include <stdlib.h>
#include <GL/glew.h>
#include "shadowpassshader.h"

class ShadowMap
{
public:
    ShadowMap();
    /**
     * @brief cleanUp
     */
    void cleanUp();
    /**
     * @brief initTexture
     */
    void initTexture();
    /**
     * @brief initFrameBuffer
     */
    void initFrameBuffer();
    /**
     * @brief shadowPass
     */
    void shadowPass(Scene &scene);
private:
    GLuint _depthFBO;
    GLuint _shadowMap;
    const unsigned int _WIDTH = 1024,_HEIGHT = 1024;
    ShadowPassShader _shader;

};

#endif // SHADOWMAP_H
