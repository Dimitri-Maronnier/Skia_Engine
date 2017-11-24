#include "loader.h"
#include <SOIL2/SOIL2.h>
#include <SOIL2/stb_image.h>
#include <iostream>

Loader::Loader()
{

}

Mesh Loader::loadToVAO(aiMesh* amesh){

    unsigned int *faceArray;
    faceArray = (unsigned int *)malloc(sizeof(unsigned int) * amesh->mNumFaces * 3);
    unsigned int faceIndex = 0;

    for (unsigned int t = 0; t < amesh->mNumFaces; ++t) {
        const aiFace* face = &amesh->mFaces[t];

        memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
        faceIndex += 3;
     }
    int vaoID = createVAO();

    bindIndicesBuffer(faceArray,sizeof(unsigned int) * amesh->mNumFaces * 3);
    if (amesh->HasPositions())
        storeDataInAttributeList(0,3,amesh->mVertices,sizeof(float)*3*amesh->mNumVertices);
    if(amesh->HasNormals())
        storeDataInAttributeList(1,3,amesh->mNormals,sizeof(float)*3*amesh->mNumVertices);
    if(amesh->HasTextureCoords(0)){
        float *texCoords = (float *)malloc(sizeof(float)*2*amesh->mNumVertices);
        for (unsigned int k = 0; k < amesh->mNumVertices; ++k) {

            texCoords[k*2]   = amesh->mTextureCoords[0][k].x;
            texCoords[k*2+1] = amesh->mTextureCoords[0][k].y;
        }
        storeDataInAttributeList(2,2,texCoords,sizeof(float)*2*amesh->mNumVertices);
    }else{
        float *texCoords = (float *)malloc(sizeof(float)*2*amesh->mNumVertices);
        for (unsigned int k = 0; k < amesh->mNumVertices; ++k) {

            texCoords[k*2]   = 0;
            texCoords[k*2+1] = 0;
        }
        storeDataInAttributeList(2,2,texCoords,sizeof(float)*2*amesh->mNumVertices);
    }
    if(amesh->HasTangentsAndBitangents())
        storeDataInAttributeList(3,3,amesh->mTangents,sizeof(float)*3*amesh->mNumVertices);
    unbindVAO();
    Mesh mesh(vaoID,amesh->mNumFaces * 3);
    return mesh;
}

Mesh Loader::loadToVAO(Object3dData m)
{

    int vaoID = createVAO();

    bindIndicesBuffer(m.facesArray,sizeof(unsigned int) * m.nomberFace);

    storeDataInAttributeList(0,3,m.positionsArray,sizeof(float)*m.nomberVertices);
    storeDataInAttributeList(1,3,m.normalsArray,sizeof(float)*m.nomberVertices);
    storeDataInAttributeList(2,2,m.texturesArray,sizeof(float)*m.nomberTextures);
    storeDataInAttributeList(3,3,m.tangentsArray,sizeof(float)*m.nomberVertices);

    unbindVAO();
    Mesh mesh(vaoID,m.nomberFace);
    return mesh;
}

/*Mesh Loader::loadToVAO(std::vector<float> positions, std::vector<float> textureCoords, std::vector<float> normals, std::vector<float> tangents, std::vector<int> indices){
    float *positionsArray = &positions[0];
    int *indicesArray = &indices[0];
    float *normalsArray = &normals[0];
    float *textureCoordsArray = &textureCoords[0];
    float *tangentsArray = &tangents[0];
    int vaoID = createVAO();
    bindIndicesBuffer(indicesArray,indices.size());
    storeDataInAttributeList(0,3,positionsArray,positions.size());
    storeDataInAttributeList(1,3,normalsArray,normals.size());
    storeDataInAttributeList(2,3,textureCoordsArray,textureCoords.size());
    storeDataInAttributeList(3,3,tangentsArray,tangents.size());
    unbindVAO();
    Mesh mesh(vaoID,indices.size());
    return mesh;
}

Mesh Loader::loadToVAO(std::vector<float> positions, std::vector<float> textureCoords, std::vector<float> normals, std::vector<int> indices)
{
    float *positionsArray = &positions[0];
    int *indicesArray = &indices[0];
    float *normalsArray = &normals[0];
    float *textureCoordsArray = &textureCoords[0];
    int vaoID = createVAO();
    bindIndicesBuffer(indicesArray,indices.size());
    storeDataInAttributeList(0,3,positionsArray,positions.size());
    storeDataInAttributeList(1,3,normalsArray,normals.size());
    storeDataInAttributeList(2,3,textureCoordsArray,textureCoords.size());
    unbindVAO();
    Mesh mesh(vaoID,indices.size());
    return mesh;
}*/

GLuint Loader::loadTexture(char *fileName,Texture* texture){

    GLuint tex_2d = SOIL_load_OGL_texture
        (
           fileName,
           SOIL_LOAD_AUTO,
           SOIL_CREATE_NEW_ID,
           0
        );

    /* check for an error during the load process */
    if( 0 == tex_2d )
    {
        std::cout << "SOIL loading error: "<< SOIL_last_result() << std::endl;
    }else{
        std::cout << "Texture loaded: " << SOIL_last_result() << std::endl;
    }

    if(texture->useMipMap()){

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS,texture->getLodBiasMipMap());
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,texture->getBaseLevelMipMap());
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,texture->getMaxLevelMipMap());
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->getMinFilter());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->getMagFilter());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->getWrapS());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->getWrapT());


    if(texture->useAnisotropy() && GLEW_EXT_texture_filter_anisotropic){

        float QUANTITY;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&QUANTITY);
        float quantity = std::min(texture->getQuantityAnisotropy(),QUANTITY);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, quantity);
    }

    return tex_2d;
}

GLuint Loader::loadHdr(char *fileName)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float *data = stbi_loadf(fileName, &width, &height, &nrComponents, 0);
    GLuint hdrTexture;
    if (data)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load HDR image." << std::endl;
    }
    return hdrTexture;
}

int Loader::createVAO(){
    GLuint  vaoID;
    glGenVertexArrays(1,&vaoID);
    //vaos.add(vaoID);
    glBindVertexArray(vaoID);
    return vaoID;
}

void Loader::bindIndicesBuffer(unsigned int* indices,int indicesLenght){
    GLuint  vboID;
    glGenBuffers(1,&vboID);
    //vbos.add(vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLenght, indices, GL_STREAM_DRAW); // Cannot be modified
}

void Loader::storeDataInAttributeList(int attributeNumber, int coordinateSize, aiVector3D *data, int dataLenght){
    GLuint  vboID;
    glGenBuffers(1,&vboID);
    //vbos.add(vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER,dataLenght,data, GL_STATIC_DRAW); // Cannot be modified
    glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false,0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::storeDataInAttributeList(int attributeNumber, int coordinateSize, float *data, int dataLenght)
{
    GLuint  vboID;
    glGenBuffers(1,&vboID);
    //vbos.add(vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER,dataLenght,data, GL_STATIC_DRAW); // Cannot be modified
    glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false,0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Loader::unbindVAO(){
    glBindVertexArray(0);
}
