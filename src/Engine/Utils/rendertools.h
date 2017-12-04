#ifndef RENDERTOOLS_H
#define RENDERTOOLS_H
#include "../Shaders/equirectangulartocubemapshader.h"
#include "../Shaders/PBR/irradianceconvolutionshader.h"
#include "../Shaders/PBR/prefiltershader.h"
#include "../Shaders/PBR/brdfshader.h"
#include "src/define.h"
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderTools
{
public:
    RenderTools();

    /**
     * @brief equirectangularToCubeMap
     * @param hdrTexture
     * @return
     */
    static GLuint equirectangularToCubeMap(GLuint hdrTexture);
    /**
     * @brief irradianceConvolution
     * @param envCubemap
     * @return
     */
    static GLuint irradianceConvolution(GLuint envCubemap);
    /**
     * @brief prefilterCubeMap
     * @param envCubemap
     * @return
     */
    static GLuint prefilterCubeMap(GLuint envCubemap);
    /**
     * @brief generate2DLut
     * @return
     */
    static GLuint generate2DLut();
    /**
     * @brief generateCube
     * @param VAO
     * @param VBO
     */
    static void generateCube(GLuint *VAO,GLuint *VBO);
    /**
     * @brief generateQuad
     * @param VAO
     * @param VBO
     */
    static void generateQuad(GLuint *VAO,GLuint *VBO);
    /**
     * @brief renderCube
     * @param VAO
     */
    static void renderCube(GLuint VAO);
    /**
     * @brief renderQuad
     * @param VAO
     */
    static void renderQuad(GLuint VAO);
    /**
     * @brief setupDeferredShading
     * @param width
     * @param height
     * @param gBuffer
     * @param position
     * @param normal
     * @param albedoRoughness
     * @param metalOcclusion
     */
    static void setupDeferredShading(const unsigned int width, const unsigned int height, GLuint *gBuffer, GLuint *position, GLuint *normal, GLuint *albedoRoughness, GLuint *metalOcclusion);
};

#endif // RENDERTOOLS_H
