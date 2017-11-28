#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <glm/vec3.hpp>
#include <vector>
#include "src/Engine/Entities/entity.h"
#include "src/Engine/Models/model.h"
#include <assimp/scene.h>
#include "src/Assets/asset.h"

class Model;

class Object3dData{
public:
    Object3dData(){}
    std::string name;
    unsigned int nomberFace;
    unsigned int nomberVertices;
    unsigned int nomberTextures;
    unsigned int *facesArray;
    float *positionsArray;
    float *normalsArray;
    float *texturesArray;
    float *tangentsArray;
};

class Object3DStatic :public Asset, public Entity
{
public:
    Object3DStatic();
    ~Object3DStatic();
    Object3DStatic(const unsigned int handle, const std::string name, const std::string path);
    Object3DStatic(const unsigned int handle, const std::string name, const std::string path ,glm::vec3 position,glm::vec3 rotation,glm::vec3 scale,std::string label);
    Object3DStatic(const unsigned int handle, const std::string name, const std::string path ,glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string label, std::vector<Model*> model);

    std::vector<QString> *getMaterialsPath();
    Model* getModel(int i);
    std::vector<Model*> getModels();
    void setModel(Model* model, int i);

    void createModel(const aiScene *scene);

private:
    std::vector<Model*> m_models;
    std::vector<QString> m_materialsPath;


};

#endif // OBJECT3D_H
