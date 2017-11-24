#include "model.h"

Model::Model()
{

}

Model::~Model()
{
}

Model::Model(Mesh mesh, Material *material, std::string name){
    m_mesh = mesh;
    m_material = material;
    m_name = name;
}

void Model::setMesh(Mesh mesh){
    m_mesh = mesh;
}

void Model::setMaterial(Material *material){
    m_material = material;
}

void Model::setName(std::string name)
{
    m_name = name;
}

Mesh Model::getMesh(){
    return m_mesh;
}

Material* Model:: getMaterial(){
    return m_material;
}

std::string Model::getName()
{
    return m_name;
}
