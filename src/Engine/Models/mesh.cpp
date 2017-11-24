#include "mesh.h"

Mesh::Mesh(){
    this->vaoID=0;
    this->vertexCount=0;
}

Mesh::Mesh(int vaoID, unsigned int vertexCount)
{
    this->vaoID = vaoID;
    this->vertexCount = vertexCount;
}

int Mesh::getVaoID(){
    return vaoID;
}

int Mesh::getVertexCount(){
    return vertexCount;
}
