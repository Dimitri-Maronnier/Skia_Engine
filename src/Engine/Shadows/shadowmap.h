#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <stdlib.h>
#include <GL/glew.h>

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
    void shadowPass();
private:
    GLuint _depthFBO;
    GLuint _shadowMap;
    const unsigned int _WIDTH = 1024,_HEIGHT = 1024;
};

#endif // SHADOWMAP_H
