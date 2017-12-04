#include "scene.h"
#include <Windows.h>
#include <boost/filesystem.hpp>
#include "src/utils.h"
#include <iostream>
#include <stdlib.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <iostream>

CameraThird Scene::camera = CameraThird(50);

Scene::Scene()
{

}

void Scene::init(){
    camera.move(0,0,0,0,0,0,0);

    cubeVAO = new GLuint();
    cubeVBO = new GLuint();
    RenderTools::generateCube(cubeVAO,cubeVBO);

    hdr = Loader::loadHdr((char*)"reflexion.hdr");
    m_skyboxHdr = RenderTools::equirectangularToCubeMap(hdr);
    m_irradianceMap = RenderTools::irradianceConvolution(m_skyboxHdr);
    m_prefilterMap = RenderTools::prefilterCubeMap(m_skyboxHdr);
    m_brdfMap = RenderTools::generate2DLut();

    m_skyShader.init("simpleSyboxVertex.glsl","simpleSkyboxFragment.glsl");
    m_skyShader.start();
    m_skyShader.connectTextureUnits();
    m_skyShader.loadProjection(camera.getProjectionMatrix());
    m_skyShader.stop();

    shader.init("vs.glsl","fs.glsl");


    light.setColor(glm::vec3(1,1,1));
    light.setPosition(glm::vec3(-200,200,0));

    shader.start();
    shader.loadProjectionMatrix(camera.getProjectionMatrix());
    shader.stop();
}

void Scene::addObject3DStatic(Object3DStatic *object)
{
    selectedEntity = object;
    emit oneEntityHaveBeenSelected(selectedEntity);
    StaticObjects.push_back(object);
}

void Scene::cleanUp()
{
    shader.cleanUp();
    SAFE_DELETE(cubeVAO);
    SAFE_DELETE(cubeVBO);
}

void Scene::render()
{

    foreach(Object3DStatic *object,StaticObjects){
        shader.start();
        shader.loadModelMatrix(object->getModelMatrix());
        shader.stop();
        foreach(Model* model,object->getModels()){

            glBindVertexArray(model->getMesh().getVaoID());
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);

            if(model->getMaterial() && model->getMaterial()->isShaderInit()){
                model->getMaterial()->getShader().start();
                model->getMaterial()->getShader().loadModelMatrix(object->getModelMatrix());
                model->getMaterial()->getShader().loadProjectionMatrix(camera.getProjectionMatrix());
                model->getMaterial()->getShader().loadViewMatrix(camera.getPosition(), camera.getViewMatrix());
                model->getMaterial()->getShader().loadLightPosition(light.getPosition());
                model->getMaterial()->getShader().loadLightColor(light.getColor());
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, m_brdfMap);
                for(unsigned int i=0;i<model->getMaterial()->getTexturesPath().size();i++){

                    glActiveTexture(GL_TEXTURE0+i+3);
                    Texture *t = AssetsCollections::TexturesCollection.GetElementByPath(model->getMaterial()->getTexturesPath().at(i));
                    if (t)
                        glBindTexture(GL_TEXTURE_2D, t->getTextureID() );
                }
                glDrawElements(GL_TRIANGLES,model->getMesh().getVertexCount(),GL_UNSIGNED_INT, 0);
                model->getMaterial()->getShader().stop();
            }else{
                shader.start();
                shader.loadViewMatrix(camera.getPosition(), camera.getViewMatrix());
                shader.loadLightPosition(light.getPosition());
                shader.loadLightColor(light.getColor());
                glDrawElements(GL_TRIANGLES,model->getMesh().getVertexCount(),GL_UNSIGNED_INT, 0);
                shader.stop();
            }


            glDisableVertexAttribArray(3);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        }
    }

    m_skyShader.start();
    m_skyShader.loadView( camera.getViewMatrix());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxHdr);
    RenderTools::renderCube(*cubeVAO);
    m_skyShader.stop();

}

void Scene::resize(glm::mat4 matrix)
{
    camera.setProjectionMatrix(matrix);
}
