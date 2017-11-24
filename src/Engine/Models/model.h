#ifndef MODEL_H
#define MODEL_H
#include "src/Engine/Models/mesh.h"
#include "src/Engine/Materials/material.h"

class Material;

class Model
{
public:
    Model();
    ~Model();
    Model(Mesh mesh,Material *material,std::string name);

    void setMesh(Mesh mesh);
    void setMaterial(Material *material);
    void setName(std::string name);

    Mesh getMesh();
    Material *getMaterial();
    std::string getName();

private:
    Mesh m_mesh;
    Material *m_material;
    std::string m_name;
};

#endif // MODEL_H
