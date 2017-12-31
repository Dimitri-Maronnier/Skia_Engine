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
#include "src/Editor/Utils/projectinfo.h"
#include "src/Editor/Utils/compressor.h"

CameraThird Scene::camera = CameraThird(50);


Scene::Scene()
{
    _entitySelected = false;
}

void Scene::init(SceneTree* scenetree){
    _sceneTree = scenetree;
    camera.move(0,0,0,0,0,0,0);


    cubeVAO = new GLuint();
    cubeVBO = new GLuint();
    RenderTools::generateCube(cubeVAO,cubeVBO);

    unsigned int handle = AssetsCollections::TexturesCollection.AddR("reflexion",":/Images/reflexion.shdrtex");
    AssetsCollections::HandlesTextures.push_back(handle);

    hdr = AssetsCollections::TexturesCollection.GetElement(handle)->getTextureID();
    m_skyboxHdr = RenderTools::equirectangularToCubeMap(hdr);
    m_irradianceMap = RenderTools::irradianceConvolution(m_skyboxHdr);
    m_prefilterMap = RenderTools::prefilterCubeMap(m_skyboxHdr);
    m_brdfMap = RenderTools::generate2DLut();

    m_skyShader.init("simpleSyboxVertex.glsl","simpleSkyboxFragment.glsl");
    m_skyShader.start();
    m_skyShader.connectTextureUnits();
    m_skyShader.loadProjectionMatrix(camera.getProjectionMatrix());
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
    auto it = find(StaticObjects.begin(),StaticObjects.end(),object);
    _entitySelected = true;
    if(it != StaticObjects.end()){
        auto copy = new Object3DStatic(*object);

        selectedEntity = copy;
        emit oneEntityHaveBeenSelected(dynamic_cast<Object3DStatic*>(selectedEntity));
        emit oneEntityHaveBeenAdded(selectedEntity);
        StaticObjects.push_back(copy);
    }
    else{
        selectedEntity = object;
        emit oneEntityHaveBeenSelected(dynamic_cast<Object3DStatic*>(selectedEntity));
        emit oneEntityHaveBeenAdded(selectedEntity);
        StaticObjects.push_back(object);
    }
}

Object3DStatic *Scene::addObject(Object3DStatic *entity)
{
    auto it = find(StaticObjects.begin(),StaticObjects.end(),entity);

    if(it != StaticObjects.end()){
        auto copy = new Object3DStatic(*entity);

        selectedEntity = copy;

        StaticObjects.push_back(copy);
        Cache.push_back(copy);

        return copy;
    }
    else{
        auto copy = new Object3DStatic(*entity);
        selectedEntity = copy;

        StaticObjects.push_back(copy);
        Cache.push_back(copy);
        return copy;
    }

}

void Scene::destroyObject(Object3DStatic *entity)
{
    auto it = find(StaticObjects.begin(),StaticObjects.end(),entity);
    auto it2 = find(Cache.begin(),Cache.end(),entity);

    if(it != StaticObjects.end()){
        SAFE_DELETE(entity);
        StaticObjects.erase(it);
        if(it2 != StaticObjects.end()){
            Cache.erase(it2);
        }
    }
}


void Scene::selectEntity(Entity* entity)
{
    selectedEntity = static_cast<Object3DStatic*>(entity);
    _entitySelected = true;
    emit oneEntityHaveBeenSelected(dynamic_cast<Object3DStatic*>(selectedEntity));
}


void Scene::cleanUp()
{
    shader.cleanUp();
    SAFE_DELETE(cubeVAO);
    SAFE_DELETE(cubeVBO);

    _sceneTree->clear();


    Cache.clear();
    StaticObjects.clear();
}

void Scene::render()
{
    size_t len = StaticObjects.size();
    for(int i=0; i< len; i++){
        StaticObjects.at(i)->lastFrameCollision.clear();
    }
    for(int i=0; i< len; i++){
        for(int j=i+1; j< len; j++){
            Collision c;
            if(StaticObjects.at(i)->sphereCollide(*StaticObjects.at(j),c))
            {
                StaticObjects.at(i)->lastFrameCollision.push_back(c);
                StaticObjects.at(j)->lastFrameCollision.push_back(c.reciproc(*StaticObjects.at(i)));
                //std::cout << "A collision has been detected!" << std::endl;
            }
        }
    }
    foreach(auto entity,StaticObjects){
        auto object = dynamic_cast<Object3DStatic*>(entity);
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
        glm::vec3 position = selectedEntity->getPosition()+upVector;

        pivotShader.loadPoint(selectedEntity->getPosition(),position);
        glLineWidth(10);
        glDrawArrays(GL_POINTS,0,1);
        pivotShader.stop();
        glDepthFunc(GL_LEQUAL);
    }
#endif

    m_skyShader.start();
    m_skyShader.loadViewMatrix( camera.getViewMatrix());
    m_skyShader.loadProjectionMatrix(camera.getProjectionMatrix());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxHdr);
    RenderTools::renderCube(*cubeVAO);
    m_skyShader.stop();

}

void Scene::resize(glm::mat4 matrix)
{
    camera.setProjectionMatrix(matrix);
}

Entity *Scene::getEntity(QString label)
{
    for(auto i = 0;i<StaticObjects.size();i++)
    {
        if(!StaticObjects.at(i)->getLabel().compare(label)){
            return StaticObjects.at(i);
        }
    }

    return nullptr;
}


void Scene::setScene(const QString &map)
{

    _sceneTree->clear();

    /*foreach(Entity*entity,StaticObjects){
        SAFE_DELETE(entity);
    }*/
    Cache.clear();
    StaticObjects.clear();

    ProjectInfo::currentMap = map;
    Compressor::uncompressMap(map,*this);

}

void Scene::cleanGameAsset()
{

    foreach(Entity*entity,Cache){
        auto it = find(StaticObjects.begin(),StaticObjects.end(),entity);
        SAFE_DELETE(entity);
        StaticObjects.erase(it);
    }
    Cache.clear();

}
