#ifndef LOADER_H
#define LOADER_H
#include "src/Engine/Models/mesh.h"
#include <GL/glew.h>
#include <vector>
#include "imgloader.h"
#include <assimp/mesh.h>
#include <assimp/vector3.h>
#include "src/Engine/Materials/texture.h"
#include "src/Engine/Entities/object3dstatic.h"

class Loader
{
public:
    Loader();
    /*static Mesh loadToVAO(std::vector<float> positions, std::vector<float> textureCoords, std::vector<float> normals,std::vector<float> tangents, std::vector<int> indices);
    static Mesh loadToVAO(std::vector<float> positions, std::vector<float> textureCoords, std::vector<float> normals, std::vector<int> indices);*/
    static GLuint loadTexture(char* fileName, Texture *texture);
    static GLuint loadHdr(char* fileName);

    static Mesh loadToVAO(aiMesh *amesh);
    static Mesh loadToVAO(Object3dData m);
private:
    static int createVAO();
    static void bindIndicesBuffer(unsigned int *indices, int indicesLenght);
    static void storeDataInAttributeList(int attributeNumber,int coordinateSize, aiVector3D* data,int dataLenght);
    static void storeDataInAttributeList(int attributeNumber,int coordinateSize, float* data,int dataLenght);
    static void unbindVAO();
};

#endif // LOADER_H
