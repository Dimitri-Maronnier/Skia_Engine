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
    _entitySelected = false;
}

void Scene::init(SceneTree* scenetree){
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
    pivotShader.init("Utils/pivotVertex.glsl","Utils/pivotArrowGeo.glsl","Utils/pivotArrowFragment.glsl");

    light.setColor(glm::vec3(1,1,1));
    light.setPosition(glm::vec3(-200,200,0));

    shader.start();
    shader.loadProjectionMatrix(camera.getProjectionMatrix());
    shader.stop();

    pivotShader.start();
    pivotShader.loadProjectionMatrix(camera.getProjectionMatrix());
    pivotShader.stop();

    #if IN_EDITOR
       connect(this,SIGNAL(oneEntityHaveBeenAdded(Entity*)),scenetree,SLOT(addEntity(Entity*)));
    #endif
}

void Scene::addObject3DStatic(Object3DStatic *object)
{
    std::vector<Object3DStatic*>::iterator it;
    it = find(StaticObjects.begin(),StaticObjects.end(),object);
    _entitySelected = true;
    if(it != StaticObjects.end()){
        Object3DStatic* copy = new Object3DStatic(*object);
        selectedEntity = copy;
        emit oneEntityHaveBeenSelected(selectedEntity);
        emit oneEntityHaveBeenAdded(selectedEntity);
        StaticObjects.push_back(copy);
    }
    else{
        selectedEntity = object;
        emit oneEntityHaveBeenSelected(selectedEntity);
        emit oneEntityHaveBeenAdded(selectedEntity);
        StaticObjects.push_back(object);
    }
}


void Scene::selectEntity(Entity* entity)
{
    selectedEntity = static_cast<Object3DStatic *>(entity);
    _entitySelected = true;
    emit oneEntityHaveBeenSelected(selectedEntity);
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
        shader.loadProjectionMatrix(camera.getProjectionMatrix());
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


#if IN_EDITOR
    if(_entitySelected){
        glDepthFunc(GL_GREATER);
        pivotShader.start();
        pivotShader.loadViewMatrix(camera.getPosition(),camera.getViewMatrix());
        glm::mat4 modelMatrix = selectedEntity->getModelMatrix();
        pivotShader.loadModelMatrix(modelMatrix);
        glm::vec3 upVector = glm::vec3(modelMatrix[1][0],modelMatrix[1][1],modelMatrix[1][2]);
        glm::vec3 position = selectedEntity->getPostion()+upVector;

        pivotShader.loadPoint(selectedEntity->getPostion(),position);
        glLineWidth(10);
        glDrawArrays(GL_POINTS,0,1);
        pivotShader.stop();
        glDepthFunc(GL_LEQUAL);
    }
#endif

    m_skyShader.start();
    m_skyShader.loadView( camera.getViewMatrix());
    m_skyShader.loadProjection(camera.getProjectionMatrix());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxHdr);
    RenderTools::renderCube(*cubeVAO);
    m_skyShader.stop();

}

void Scene::resize(glm::mat4 matrix)
{
    camera.setProjectionMatrix(matrix);
}
