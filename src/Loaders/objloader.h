#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "vertex.h"
#include "glm/vec3.hpp"
#include <vector>
#include <string>
#include <windows.h>

class OBJLoader
{
public:
    OBJLoader();
    static void loadObjModel(std::string path, std::string pathToData);
    static bool LoadModelFromFile(std::string sFilePath, std::string pathToData);
private:
    static void calculateTangents(Vertex* v0, Vertex* v1, Vertex* v2,std::vector<glm::vec3> textures);
    static void calculateTangents(Vertex* v0, Vertex* v1, Vertex* v2, Vertex* v3,std::vector<glm::vec3> textures);
    static Vertex* processVertex(std::vector<std::string> vertex, std::vector<Vertex> *vertices, std::vector<int> *indices, int lastVertices, int lastTextures, int lastNormals);
    static void storeDataInArrays( std::vector<Vertex> vertices,  std::vector<glm::vec3> textures, std::vector<glm::vec3> normals, float verticesArray[], float texturesArray[],float normalsArray[], float tangentsArray[]);
    static Vertex *doProccess(Vertex *previousVertex, int newTextureIndex, int newNormalIndex, std::vector<int>* indices, std::vector<Vertex>* vertices);
    static void removeUnusedVertices(std::vector<Vertex> *vertices);
};

#endif // OBJLOADER_H
