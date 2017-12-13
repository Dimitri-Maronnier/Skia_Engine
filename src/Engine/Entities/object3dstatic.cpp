#include "object3dstatic.h"
#include "src/Loaders/loader.h"
#include <iostream>
#include "src/Editor/Utils/compressor.h"

Object3DStatic::Object3DStatic()
    :Asset(-1,"name","path")
{
}

Object3DStatic::Object3DStatic(Object3DStatic const&toCopy)
    :Asset(toCopy), Entity(toCopy),
      m_models(toCopy.m_models),m_materialsPath(toCopy.m_materialsPath)
{
    /*foreach(Model*model,toCopy.getModels()){
        m_models.push_back(model);
    }
    foreach(QString string,toCopy.getMaterialsPath()){
        m_materialsPath.push_back(string);
    }*/
}

Object3DStatic::~Object3DStatic()
{

}

Object3DStatic::Object3DStatic(const unsigned int handle, const std::string name, const std::string path)
    :Asset(handle,name,path), Entity(glm::vec3(0,0,0),glm::vec3(0,0,0),glm::vec3(1,1,1),QString(name.c_str()),handle,name,path)
{

    try{
        std::vector<Object3dData*> *meshs = new  std::vector<Object3dData*>;
        unsigned int nomberMesh;

        Compressor::uncompressObject3D(QString(path.c_str()),&nomberMesh,meshs,&m_materialsPath);

        for(unsigned int numMesh=0;numMesh<nomberMesh;numMesh++){

            Mesh mesh = Loader::loadToVAO(*meshs->at(numMesh));

            m_models.push_back(new Model(mesh,NULL,meshs->at(numMesh)->name));
        }

        for(unsigned int numMesh=0;numMesh<nomberMesh;numMesh++){
            SAFE_DELETE(meshs->at(numMesh)->facesArray);
            SAFE_DELETE(meshs->at(numMesh)->positionsArray);
            SAFE_DELETE(meshs->at(numMesh)->normalsArray);
            SAFE_DELETE(meshs->at(numMesh)->texturesArray);
            SAFE_DELETE(meshs->at(numMesh)->tangentsArray);
            SAFE_DELETE(meshs->at(numMesh));
        }
        SAFE_DELETE(meshs);

        /*Load Materials*/
        if(m_materialsPath.size()>0){
            for(unsigned int numMesh=0;numMesh<nomberMesh;numMesh++){
                if(m_materialsPath.at(numMesh) != ""){
                    std::replace( m_materialsPath.at(numMesh).begin(), m_materialsPath.at(numMesh).end(), '/', '\\');
                    std::vector<std::string> split = Utils::split(m_materialsPath.at(numMesh).toStdString(),'\\');
                    unsigned int handle = AssetsCollections::MaterialsCollection.Add(split.at(split.size()-1),m_materialsPath.at(numMesh).toStdString());
                    if(handle!=-1){
                        AssetsCollections::HandlesMaterials.push_back(handle);

                        m_models.at(numMesh)->setMaterial(AssetsCollections::MaterialsCollection.GetElement(handle));


                    }
                }
            }
        }
    }catch(const std::exception &e){
        std::cerr << "Exception rise in Object static loader " << e.what() << std::endl;;
    }

}

Object3DStatic::Object3DStatic(const unsigned int handle, const std::string name, const std::string path, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string label)
    :Asset(handle,name,path),Entity(position,rotation,scale,QString(name.c_str()),handle,name,path)
{

}

Object3DStatic::Object3DStatic(const unsigned int handle, const std::string name, const std::string path ,glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string label, std::vector<Model*> model)
    :Asset(handle,name,path),Entity(position,rotation,scale,QString(name.c_str()),handle,name,path)
{
   m_models = model;
}

std::vector<QString> *Object3DStatic::getMaterialsPath()
{
    return &m_materialsPath;
}

Model* Object3DStatic::getModel(int i)
{
    return m_models.at(i);
}

std::vector<Model*> Object3DStatic::getModels()
{
    return m_models;
}

void Object3DStatic::setModel(Model* model,int i)
{
    m_models.at(i) = model;
}

void Object3DStatic::createModel(const aiScene* scene)
{

    /*for(int numMesh=0;numMesh<scene->mNumMeshes;numMesh++){
        Mesh mesh = Loader::loadToVAO(scene->mMeshes[numMesh]);
        Material material = Material();
        m_models.push_back(Model(mesh,material,scene->mMeshes[numMesh]->mName));
    }*/
}

