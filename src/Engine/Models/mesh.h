#ifndef MESH_H
#define MESH_H


class Mesh
{
public:
    Mesh();
    Mesh(int vaoID,unsigned int vertexCount);
    int getVaoID();
    int getVertexCount();

private:
    int vaoID;
    int vertexCount;
};

#endif // MESH_H
