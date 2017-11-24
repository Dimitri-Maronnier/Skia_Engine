#include "compressor.h"
#include <QFile>
#include <QBytearray>
#include <QFileInfo>
#include <QDataStream>
#include <boost/filesystem.hpp>
#include "src/utils.h"
#include <iostream>

bool Compressor::compressTextureData(QString archive, QVector<QString> files, QVector<QString> fileNames,Texture textureData)
{
    QFile archiveFile;
    archiveFile.setFileName(archive);
    if (!archiveFile.open(QIODevice::WriteOnly))
        return false;

    QDataStream dataStream;
    dataStream.setDevice(&archiveFile);

    dataStream << textureData.isHDR();
    dataStream << textureData.getMinFilter();
    dataStream << textureData.getMagFilter();
    dataStream << textureData.getWrapS();
    dataStream << textureData.getWrapT();
    dataStream << textureData.useMipMap();
    dataStream << textureData.getBaseLevelMipMap();
    dataStream << textureData.getMaxLevelMipMap();
    dataStream << textureData.getLodBiasMipMap();
    dataStream << textureData.useAnisotropy();
    dataStream << textureData.getQuantityAnisotropy();
    dataStream << textureData.useCompression();
    dataStream << textureData.getCompressionFormat();

    for(int i=0;i<files.size();i++){
        QFile file(files.at(i));

        if (!file.open(QIODevice::ReadOnly))
            return false;
        dataStream << fileNames.at(i);
        dataStream << qCompress(file.readAll());

        file.close();
    }
    archiveFile.close();
    return true;
}

QVector<QString> Compressor::uncompressTextureData(QString archive,Texture *textureData)
{
    boost::filesystem::path tempPath = boost::filesystem::temp_directory_path();


    /*If skiaTmp exits delete it*/
    QString skiaTmpStr = tempPath.string().c_str();
    skiaTmpStr += "\\skiaTmp";
    boost::filesystem::path skiaTmp = skiaTmpStr.toStdString();

    if(boost::filesystem::exists(skiaTmp)){

        boost::filesystem::directory_iterator end_itr;
        for ( boost::filesystem::directory_iterator itr(skiaTmp); itr != end_itr; itr++)
        {
            remove_all(itr->path());
        }
    }

    /*Create tmp folder*/
    boost::filesystem::create_directory(skiaTmp);
    QString folderDataStr =  skiaTmp.string().c_str();
    folderDataStr += "\\" ;
    std::vector<std::string> split = Utils::split(archive.toStdString(),'\\');
    folderDataStr += Utils::removeExtension(QString(split.at(split.size()-1).c_str()));

    boost::filesystem::path folderData = folderDataStr.toStdString();


    boost::filesystem::create_directory(folderData);
    QVector<QString> list_files;

    QFile archiveFile;
    archiveFile.setFileName(archive);
    if (!archiveFile.open(QIODevice::ReadOnly)){
        std::cerr << "Couldn't open archive file" << std::endl;
        return list_files;
    }

    QDataStream dataStream;
    dataStream.setDevice(&archiveFile);

    GLuint minfilter,maxfilter,wraps,wrapt,compressionformat;
    bool isHdr,useMipMap,useAnisotropy,ueCompression;
    float base,max,bias,quatity;


    dataStream >> isHdr;
    dataStream >> minfilter;
    dataStream >> maxfilter;
    dataStream >> wraps;
    dataStream >> wrapt;
    dataStream >> useMipMap;
    dataStream >> base;
    dataStream >> max;
    dataStream >> bias;
    dataStream >> useAnisotropy;
    dataStream >> quatity;
    dataStream >> ueCompression;
    dataStream >> compressionformat;

    textureData->setIsHDR(isHdr);
    textureData->setMinFilter(minfilter);
    textureData->setMagFilter(maxfilter);
    textureData->setWrapS(wraps);
    textureData->setWrapT(wrapt);
    textureData->setUseMipMap(useMipMap);
    textureData->setBaseLevelMipMap(base);
    textureData->setMaxLevelMipMap(max);
    textureData->setLodBiasMipMap(bias);
    textureData->setUseAnisotropy(useAnisotropy);
    textureData->setQuantityAnisotropy(quatity);
    textureData->setUseCompression(ueCompression);
    textureData->setCompressionFormat(compressionformat);

    while (!dataStream.atEnd()) {
        QString fileName;
        QByteArray data;

        dataStream >> fileName >> data;


        QFile outFile;
        outFile.setFileName(folderDataStr + "\\" + fileName);
        if (!outFile.open(QIODevice::WriteOnly)) {
            archiveFile.close();
            list_files.clear();
            return list_files;
        }
        list_files.push_back(outFile.fileName());
        outFile.write(qUncompress(data));
        outFile.close();
    }

    archiveFile.close();
    return list_files;
}

int Compressor::compressObject3D(QString archive,const aiScene *scene)
{
    QFile archiveFile;
    archiveFile.setFileName(archive);
    if (!archiveFile.open(QIODevice::WriteOnly))
        return 0;

    QDataStream dataStream;
    dataStream.setDevice(&archiveFile);

    dataStream << scene->mNumMeshes;

    for(int numMesh=0;numMesh<scene->mNumMeshes;numMesh++){
        aiMesh* amesh = scene->mMeshes[numMesh];
        QString name = (amesh->mName.length)?QString(amesh->mName.C_Str()):"Element_" + QString::number(numMesh);
        dataStream << name;

        dataStream << amesh->mNumFaces * 3;
        dataStream << 3*amesh->mNumVertices;
        dataStream << 2*amesh->mNumVertices;

        unsigned int *faceArray;
        faceArray = (unsigned int *)malloc(sizeof(unsigned int) * amesh->mNumFaces * 3);

        unsigned int faceIndex = 0;

        for (unsigned int t = 0; t < amesh->mNumFaces; ++t) {
            const aiFace* face = &amesh->mFaces[t];

            memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
            faceIndex += 3;
         }

         //Write face Array
         for(int i=0;i<amesh->mNumFaces * 3;i++)
            dataStream << faceArray[i];

         //Write positions array
         if (amesh->HasPositions()){
             for(int i=0;i<amesh->mNumVertices ;i++){
                dataStream << amesh->mVertices[i].x;
                dataStream << amesh->mVertices[i].y;
                dataStream << amesh->mVertices[i].z;
             }
         }else{
             for(int i=0;i<amesh->mNumVertices ;i++){
                dataStream << 0;
                dataStream << 0;
                dataStream << 0;
             }
         }


         //Write normals array
         if(amesh->HasNormals()){
             for(int i=0;i<amesh->mNumVertices ;i++){
                dataStream << amesh->mNormals[i].x;
                dataStream << amesh->mNormals[i].y;
                dataStream << amesh->mNormals[i].z;
             }
         }else{
             for(int i=0;i<amesh->mNumVertices;i++){
                dataStream << 0;
                dataStream << 0;
                dataStream << 0;
             }
         }

         //Write texture array
         float *texCoords = (float *)malloc(sizeof(float)*2*amesh->mNumVertices);
         if(amesh->HasTextureCoords(0)){

             for (unsigned int k = 0; k < amesh->mNumVertices; ++k) {

                 texCoords[k*2]   = amesh->mTextureCoords[0][k].x;
                 texCoords[k*2+1] = amesh->mTextureCoords[0][k].y;
             }

         }else{

             for (unsigned int k = 0; k < amesh->mNumVertices; ++k) {

                 texCoords[k*2]   = 0;
                 texCoords[k*2+1] = 0;
             }

         }
         for(int i=0;i<2*amesh->mNumVertices;i++)
             dataStream << texCoords[i];

         //Write tangent array
         if(amesh->HasTangentsAndBitangents()){
             for(int i=0;i<amesh->mNumVertices;i++){
                dataStream << amesh->mTangents[i].x;
                dataStream << amesh->mTangents[i].y;
                dataStream << amesh->mTangents[i].z;
             }
         }else{
             for(int i=0;i<amesh->mNumVertices;i++){
                dataStream << 0;
                dataStream << 0;
                dataStream << 0;
             }
         }


    }


    archiveFile.close();
    return 1;
}

bool Compressor::uncompressObject3D(QString archive, unsigned int *nomberMeshs, std::vector<Object3dData *> *meshs)
{
    QFile archiveFile;
    archiveFile.setFileName(archive);
    if (!archiveFile.open(QIODevice::ReadOnly))
        return false;

    QDataStream dataStream;
    dataStream.setDevice(&archiveFile);

    dataStream >> *nomberMeshs;


    for(int numMesh=0;numMesh<*nomberMeshs;numMesh++){

        QString name;

        unsigned int nomberFaces,nomberVertices,nomberTextures;

        //Reading name
        dataStream >> name;
        meshs->push_back(new Object3dData);
        meshs->at(numMesh)->name = name.toStdString();

        //Reading size
        dataStream >> nomberFaces;
        dataStream >> nomberVertices;
        dataStream >> nomberTextures;

        meshs->at(numMesh)->nomberFace = nomberFaces;
        meshs->at(numMesh)->nomberVertices = nomberVertices;
        meshs->at(numMesh)->nomberTextures = nomberTextures;

        //Allocation
        meshs->at(numMesh)->facesArray = (unsigned int *)malloc(sizeof(unsigned int) * nomberFaces);
        meshs->at(numMesh)->positionsArray = (float*)malloc(sizeof(float) * nomberVertices);
        meshs->at(numMesh)->normalsArray = (float*)malloc(sizeof(float) * nomberVertices);
        meshs->at(numMesh)->tangentsArray = (float*)malloc(sizeof(float) * nomberVertices);
        meshs->at(numMesh)->texturesArray = (float*)malloc(sizeof(float) * nomberTextures);


         //Read face Array
         for(int i=0;i<nomberFaces;i++)
            dataStream >> meshs->at(numMesh)->facesArray[i];

         //Read positions array
         for(int i=0;i<nomberVertices ;i++)
            dataStream >> meshs->at(numMesh)->positionsArray[i];


         //Read normals array
         for(int i=0;i<nomberVertices ;i++)
            dataStream >> meshs->at(numMesh)->normalsArray[i];


         //Read texture array
         for(int i=0;i<nomberTextures;i++)
             dataStream >> meshs->at(numMesh)->texturesArray[i];

         //Read tangent array
         for(int i=0;i<nomberVertices;i++)
            dataStream >> meshs->at(numMesh)->tangentsArray[i];

    }

    archiveFile.close();
}
