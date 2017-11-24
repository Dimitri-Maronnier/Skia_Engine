#include "objloader.h"
#include "src/utils.h"
#include <QFile>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <glm/gtx/norm.hpp>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include "src/Editor/Utils/compressor.h"

OBJLoader::OBJLoader()
{

}

void OBJLoader::loadObjModel(std::string path,std::string pathToData){
    std::ifstream  fr;
    try {
         fr.open (path.c_str(), std::ifstream ::in);
    } catch (std::ifstream::failure e) {
        std::cerr << "Couldn't load file!\n";
    }


    std::string line;
    std::string fileMaterial="";
    std::vector<Vertex> vertices;
    std::vector<glm::vec3> textures;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<int> indices;


    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();

    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();

    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();


    try{
        while(true){

            std::getline(fr,line);
            std::vector<std::string> currentLine = Utils::split(line,' ');
            if(!currentLine.empty()){
                if(currentLine.at(0).compare("mtllib") == 0){
                    fileMaterial = currentLine[1];
                }
                else if(currentLine.at(0).compare("v") == 0){	//Vertices position
                    glm::vec3 vertex;

                    if(currentLine[1].empty()){
                        vertex = glm::vec3(strtof(currentLine[2].c_str(),NULL),strtof(currentLine[3].c_str(),NULL),strtof(currentLine[4].c_str(),NULL));
                    }else{

                        vertex = glm::vec3(strtof(currentLine[1].c_str(),NULL),strtof(currentLine[2].c_str(),NULL),strtof(currentLine[3].c_str(),NULL));
                    }
                    vertices.push_back(Vertex(vertices.size(),vertex));
                    if(vertex.x < minX)
                        minX = vertex.x;
                    if(vertex.x > maxX)
                        maxX = vertex.x;

                    if(vertex.y < minY)
                        minY = vertex.y;
                    if(vertex.y > maxY)
                        maxY = vertex.y;

                    if(vertex.z < minZ)
                        minZ = vertex.z;
                    if(vertex.z > maxX)
                        maxZ = vertex.z;
                }
                else if(currentLine.at(0).compare("vt") == 0){  //Texture coord
                    glm::vec3 texture;
                    if(currentLine.size()==4){
                        texture = glm::vec3(strtof(currentLine[1].c_str(),NULL),strtof(currentLine[2].c_str(),NULL),strtof(currentLine[3].c_str(),NULL));
                    }else{
                        texture = glm::vec3(strtof(currentLine[1].c_str(),NULL),strtof(currentLine[2].c_str(),NULL),0);
                    }
                    textures.push_back(texture);
                }
                else if(currentLine.at(0).compare("vn") == 0){  //normals vertex
                    glm::vec3 normal = glm::vec3(strtof(currentLine[1].c_str(),NULL),strtof(currentLine[2].c_str(),NULL),strtof(currentLine[3].c_str(),NULL));
                    normals.push_back(normal);
                }
                else if(currentLine.at(0).compare("f") == 0){  //Faces
                    break;
                }
            }
        }



        while(true){

            std::vector<std::string> currentLine = Utils::split(line,' ');
            if(!currentLine.empty()){
                if(currentLine.at(0).compare("f") != 0){
                    std::getline(fr,line);
                    continue;
                }


                std::vector<std::string> vertex1 = Utils::split(currentLine[1],'/');
                std::vector<std::string> vertex2 = Utils::split(currentLine[2],'/');
                std::vector<std::string> vertex3 = Utils::split(currentLine[3],'/');

                Vertex* v0 = processVertex(vertex1, &vertices, &indices,0,0,0);
                Vertex* v1 = processVertex(vertex2, &vertices, &indices,0,0,0);
                Vertex* v2 = processVertex(vertex3, &vertices, &indices,0,0,0);


                if(currentLine.size()==5){
                    std::vector<std::string> vertex4 = Utils::split(currentLine[4],'/');
                    v2 = processVertex(vertex3, &vertices, &indices,0,0,0);
                    Vertex* v3 = processVertex(vertex4, &vertices, &indices,0,0,0);
                    v0 = processVertex(vertex1,&vertices, &indices,0,0,0);
                    calculateTangents(v0,v1,v2,v3, textures);
                }else
                    calculateTangents(v0, v1, v2, textures);



            }
            if(!std::getline(fr,line))break;
        }
        fr.close();

    }catch(std::ifstream::failure e){
        std::cerr << "read failure\n";
    }

    int* indicesArray = new int[indices.size()];

    removeUnusedVertices(&vertices);
    for(int i=0;i<indices.size();i++){
        indicesArray[i] = indices.at(i);
    }



    float* verticesArray = new float[vertices.size() * 3];
    float* texturesArray = new float[vertices.size() * 3];
    float* normalsArray = new float[vertices.size() * 3];
    float* tangentsArray = new float[vertices.size() * 3];
    storeDataInArrays(vertices, textures, normals, verticesArray,
            texturesArray, normalsArray, tangentsArray);
    std::string pathTmp = boost::filesystem::temp_directory_path().string();
    std::string vertexFilePath = pathTmp;
    vertexFilePath += "\\vertices.ver";
    std::ofstream verfile (vertexFilePath.c_str());
    for(int i=0;i<vertices.size() * 3;i+=3)
        verfile << verticesArray[i] << " " << verticesArray[i+1] << " " << verticesArray[i+2] << " "
                << texturesArray[i] << " " << texturesArray[i+1] << " " << texturesArray[i+2] << " "
                << normalsArray[i] << " " << normalsArray[i+1] << " " << normalsArray[i+2] << " "
                << tangentsArray[i] << " " << tangentsArray[i+1] << " " << tangentsArray[i+2] <<std::endl;
    verfile.close();

    std::string indiceFilePath = pathTmp;
    indiceFilePath += "\\indices.ind";
    std::ofstream indfile (indiceFilePath.c_str());
    for(int i=0;i<indices.size();i++){
        indfile << indicesArray[i] << std::endl;
    }
    indfile.close();

    std::string cmd = "C:/Users/C17/Documents/build-Skia_Engine-MVSC/7-Zip/7z.exe a -tzip " + pathToData + " " + vertexFilePath + " " + indiceFilePath;
    WinExec(cmd.c_str(),SW_HIDE);




    delete []indicesArray;
    delete []verticesArray;
    delete []texturesArray;
    delete []normalsArray;
    delete []tangentsArray;

    //Material texture = loadMaterial(fileMaterial,fileName,loader);
    //Model model = new Model(loader.loadToVAO(verticesArray, texturesArray,normalsArray, indicesArray,tangentsArray),texture);
    //BoundingBox box = new BoundingBox(new Vector3f(minX,minY,minZ),new Vector3f(maxX,maxY,maxZ));
    //model.setBoundingBox(box);
    //return model ;
}

bool OBJLoader::LoadModelFromFile(std::string sFilePath,std::string pathToData)
{

    const aiScene* scene = aiImportFile(sFilePath.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);

    if(!scene)
    {
       std::cerr << "Couldn't load model Error Importing Asset :" << sFilePath <<  std::endl;;
    }

    if(Compressor::compressObject3D(QString(pathToData.c_str()),scene) == 0){
        std::cerr << "Couldn't compress Asset :" << sFilePath << std::endl;
    }
    SAFE_DELETE(scene);
    return true;
}

void OBJLoader::calculateTangents(Vertex* v0, Vertex* v1, Vertex* v2,
        std::vector<glm::vec3> textures) {

    glm::vec3 delatPos1 = v1->position - v0->position;
    glm::vec3 delatPos2 = v2->position - v0->position;

    glm::vec3 uv0 = glm::vec3(0,0,0);
    glm::vec3 uv1 = glm::vec3(0,0,0);
    glm::vec3 uv2 = glm::vec3(0,0,0);
    if(!textures.empty()){
        uv0 = textures.at(v0->getTextureIndex());

        uv1 = textures.at(v1->getTextureIndex());
        uv2 = textures.at(v2->getTextureIndex());
    }

    glm::vec3 deltaUv1 = uv1 - uv0;
    glm::vec3 deltaUv2 = uv2 - uv0;

    float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
    delatPos1 *= deltaUv2.y;
    delatPos2 *= deltaUv1.y;
    glm::vec3 tangent = delatPos1 - delatPos2;
    tangent *= r;
    v0->addTangent(tangent);
    v1->addTangent(tangent);
    v2->addTangent(tangent);
}

void OBJLoader::calculateTangents(Vertex* v0, Vertex* v1, Vertex* v2, Vertex* v3,
        std::vector<glm::vec3> textures) {
    glm::vec3 delatPos1 = v1->position - v0->position;
    glm::vec3 delatPos2 = v2->position - v0->position;
    glm::vec3 uv0 = glm::vec3(0,0,0);
    glm::vec3 uv1 = glm::vec3(0,0,0);
    glm::vec3 uv2 = glm::vec3(0,0,0);
    if(!textures.empty()){
        uv0 = textures.at(v0->getTextureIndex());
        uv1 = textures.at(v1->getTextureIndex());
        uv2 = textures.at(v2->getTextureIndex());
    }
    glm::vec3 deltaUv1 = uv1 - uv0;
    glm::vec3 deltaUv2 = uv2 - uv0;

    float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
    delatPos1 *= deltaUv2.y;
    delatPos2 *= deltaUv1.y;
    glm::vec3 tangent = delatPos1 - delatPos2;
    tangent *= r;
    v0->addTangent(tangent);
    v1->addTangent(tangent);
    v2->addTangent(tangent);
    v3->addTangent(tangent);
}

Vertex* OBJLoader::processVertex(std::vector<std::string> vertex, std::vector<Vertex>* vertices,
        std::vector<int>* indices, int lastVertices, int lastTextures, int lastNormals) {
    int index = atoi(vertex[0].c_str()) - 1 - lastVertices;
    Vertex* currentVertex = &vertices->at(index);

    int textureIndex = 0;
    if(vertex[1].compare("") != 0)
        textureIndex = atoi(vertex[1].c_str()) - 1 - lastTextures;
    int normalIndex = atoi(vertex[2].c_str()) - 1 - lastNormals;
    if (!currentVertex->isSet()) {

        currentVertex->setTextureIndex(textureIndex);

        currentVertex->setNormalIndex(normalIndex);
        indices->push_back(index);
        return currentVertex;
    } else {
        return doProccess(currentVertex, textureIndex, normalIndex, indices,
                vertices);
    }
}

void OBJLoader::storeDataInArrays(std::vector<Vertex> vertices, std::vector<glm::vec3> textures,std::vector<glm::vec3> normals, float verticesArray[], float texturesArray[],
        float normalsArray[], float tangentsArray[]) {

    for (int i = 0; i < vertices.size(); i++) {
        Vertex currentVertex = vertices.at(i);

        glm::vec3 position = currentVertex.position;
        glm::vec3 textureCoord = glm::vec3(0,0,0);
        glm::vec3 normalVector = glm::vec3(0,0,0);

        if(currentVertex.isSet()){
            if(!textures.empty())
                textureCoord = textures.at(currentVertex.getTextureIndex());

            normalVector = normals.at(currentVertex.getNormalIndex());
        }

        glm::vec3 tangent = currentVertex.tangent;
        verticesArray[i * 3] = position.x;
        verticesArray[i * 3 + 1] = position.y;
        verticesArray[i * 3 + 2] = position.z;
        texturesArray[i * 3] = textureCoord.x;
        texturesArray[i * 3 + 1] = 1 - textureCoord.y;
        texturesArray[i * 3 + 2] = textureCoord.z;
        normalsArray[i * 3] = normalVector.x;
        normalsArray[i * 3 + 1] = normalVector.y;
        normalsArray[i * 3 + 2] = normalVector.z;
        tangentsArray[i * 3] = tangent.x;
        tangentsArray[i * 3 + 1] = tangent.y;
        tangentsArray[i * 3 + 2] = tangent.z;

    }

}

Vertex* OBJLoader::doProccess(Vertex* previousVertex, int newTextureIndex,
        int newNormalIndex, std::vector<int> *indices, std::vector<Vertex> *vertices) {
    if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
        indices->push_back(previousVertex->getIndex());
        return previousVertex;
    } else {
        Vertex* anotherVertex = previousVertex->getDuplicateVertex();
        if (anotherVertex != NULL) {
            return doProccess(anotherVertex, newTextureIndex,
                    newNormalIndex, indices, vertices);
        } else {
            Vertex* duplicateVertex = new Vertex(vertices->size(), previousVertex->position);
            duplicateVertex->setTextureIndex(newTextureIndex);
            duplicateVertex->setNormalIndex(newNormalIndex);
            previousVertex->setDuplicateVertex(duplicateVertex);
            vertices->push_back(*duplicateVertex);
            indices->push_back(duplicateVertex->getIndex());
            return duplicateVertex;
        }

    }
}

void OBJLoader::removeUnusedVertices(std::vector<Vertex>* vertices) {
    for(int i=0;i<vertices->size();i++) {
        Vertex *currentVertex = &vertices->at(i);
        currentVertex->computeTangents();
        if (!currentVertex->isSet()) {
            currentVertex->setTextureIndex(0);
            currentVertex->setNormalIndex(0);
        }
    }
}
