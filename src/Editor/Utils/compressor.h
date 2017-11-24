#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#include <QString>
#include <QVector>
#include "src/Engine/Materials/texture.h"
#include <GL/glew.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include "src/Engine/Entities/object3dstatic.h"

class Compressor
{
public:
    static bool compressTextureData(QString archive, QVector<QString> files, QVector<QString> fileNames, Texture textureData);
    static QVector<QString> uncompressTextureData(QString archive,Texture *textureData);

    /** @brief Description
       @param QString archive : Archive file
       @param aiScene* scene : refenrece to loading assimp scene
       @return int : 0 cannot open Archive File,1 sucess */
    static int compressObject3D(QString archive, const aiScene *scene);
    static bool uncompressObject3D(QString archive, unsigned int *nomberMeshs , std::vector<Object3dData *> *meshs);
};

#endif // COMPRESSOR_H
