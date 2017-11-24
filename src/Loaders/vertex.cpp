#include "vertex.h"
#include <glm/gtx/norm.hpp>
Vertex::Vertex()
{
    textureIndex = -1;
    normalIndex = -1;
    duplicateVertex = NULL;
    tangent = glm::vec3(0, 0, 0);
}

Vertex::Vertex(int index,glm::vec3 position){
    this->index = index;
    this->position = position;
    textureIndex = -1;
    normalIndex = -1;
    duplicateVertex = NULL;
    tangent = glm::vec3(0, 0, 0);

}

void Vertex::addTangent(glm::vec3 tangent){
    tangents.push_back(tangent);
}

void Vertex::computeTangents(){
    if(tangents.empty()){
        return;
    }
    for(int i=0 ;i< tangents.size();i++){
        tangent += tangents.at(i);
    }
    if( glm::length(tangent.length())>0)
        glm::normalize(tangent);
}

int Vertex::getIndex(){
    return index;
}


bool Vertex::isSet(){
    return textureIndex!=-1 && normalIndex!=-1;
}

bool Vertex::hasSameTextureAndNormal(int textureIndexOther,int normalIndexOther){
    return textureIndexOther==textureIndex && normalIndexOther==normalIndex;
}

void Vertex::setTextureIndex(int textureIndex){
    this->textureIndex = textureIndex;
}

void Vertex::setNormalIndex(int normalIndex){
    this->normalIndex = normalIndex;
}


int Vertex::getTextureIndex() {
    return textureIndex;
}

int Vertex::getNormalIndex() {
    return normalIndex;
}

Vertex* Vertex::getDuplicateVertex() {
    return duplicateVertex;
}

void Vertex::setDuplicateVertex(Vertex* duplicateVertex) {
    this->duplicateVertex = duplicateVertex;
}
