#ifndef VERTEX_H
#define VERTEX_H
#include <vector>
#include "glm/vec3.hpp"

class Vertex
{
public:

    Vertex();
    Vertex(int index,glm::vec3 position);
    void addTangent(glm::vec3 tangent);
    void computeTangents();
    int getIndex();
    bool isSet();
    bool hasSameTextureAndNormal(int textureIndexOther,int normalIndexOther);
    void setTextureIndex(int textureIndex);
    void setNormalIndex(int normalIndex);
    int getTextureIndex();
    int getNormalIndex();
    Vertex *getDuplicateVertex();
    void setDuplicateVertex(Vertex *duplicateVertex);

    glm::vec3 position;
    glm::vec3 tangent;

private:
    int textureIndex;
    int normalIndex;
    Vertex* duplicateVertex;
    int index;

    std::vector<glm::vec3> tangents;
};

#endif // VERTEX_H
