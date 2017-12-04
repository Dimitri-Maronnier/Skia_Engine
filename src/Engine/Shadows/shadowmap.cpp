#include "shadowmap.h"

ShadowMap::ShadowMap()
{

}

void ShadowMap::cleanUp()
{
    glDeleteTextures(1,&_shadowMap);
    glDeleteFramebuffers(1,&_depthFBO);
}

void ShadowMap::initTexture()
{

   glGenTextures(1,&_shadowMap);
   glBindTexture(GL_TEXTURE_2D,_shadowMap);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _WIDTH, _HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void ShadowMap::initFrameBuffer()
{
    glGenFramebuffers(1,&_depthFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::shadowPass(Scene &scene)
{
    glViewport(0, 0, _WIDTH, _HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, _depthFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //Configure Shadow Matrix
        //render scene
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
