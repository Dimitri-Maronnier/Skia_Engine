#include "imgloader.h"
#include <Windows.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include "src/utils.h"
#include "src/Editor/Utils/compressor.h"
#include "src/Engine/Materials/texture.h"

bool ImgLoader::import(std::string sFilePath,std::string pathToData,bool isHDR)
{
    Texture textureDefault;
    textureDefault.setIsHDR(isHDR);
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
    std::vector<std::string> split = Utils::split(pathToData,'\\');
    folderDataStr += Utils::removeExtension(QString(split.at(split.size()-1).c_str()));

    boost::filesystem::path folderData = folderDataStr.toStdString();


    boost::filesystem::create_directory(folderData);



    QPixmap image(QString(sFilePath.c_str()));
    QSize myRessortSize(200,200);
    QPixmap myNewRessort = image.scaled(myRessortSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    QFile file(folderDataStr + "/thumnail.png");
    file.open(QIODevice::WriteOnly);
    myNewRessort.save(&file, "PNG");
    file.close();

    QVector<QString> fileToArchive;
    QVector<QString> fileNameToArchive;
    fileToArchive.push_back(QString(sFilePath.c_str()));
    fileToArchive.push_back(folderDataStr + "/thumnail.png");


    QString textureFile = "texture";
    textureFile += QString(Utils::getExtension(sFilePath).c_str());


    fileNameToArchive.push_back(textureFile);
    fileNameToArchive.push_back("thumnail.png");

    if(!Compressor::compressTextureData(QString(pathToData.c_str()),fileToArchive,fileNameToArchive,textureDefault)){
        std::cerr << "Couldn't compress texture data" << std::endl;
    }


    return true;
}
