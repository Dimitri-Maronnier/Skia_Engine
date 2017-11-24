#include "scene.h"
#include <Windows.h>
#include <boost/filesystem.hpp>
#include "src/utils.h"
#include <iostream>
#include <stdlib.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <iostream>

Scene::Scene()
{

}

void Scene::addObject3DStatic(std::string completePath,std::string objectname)
{



    std::string path = boost::filesystem::temp_directory_path().string() + "\\" + objectname;
    boost::filesystem::path bpath = path;
    boost::filesystem::create_directory(bpath);

    std::string cmdS = "C:/Users/C17/Documents/build-Skia_Engine-MVSC/7-Zip/7z.exe x -tzip " + completePath + " -o" + path;


    WinExec(cmdS.c_str(),SW_HIDE);


    std::string cmd2 = "TASKKILL /IM 7z.exe";
    Utils::createProcess(cmd2);


   /* std::string verticeFile = path + "\\vertices.ver";
    std::string indiceFile = path + "\\indices.ind";
    /*Load objFile*/
    /*std::ifstream  fr;
    try {
         fr.open (verticeFile.c_str(), std::ifstream ::in);
    } catch (std::ifstream::failure e) {
        std::cerr << "Couldn't load file!\n";
    }

    std::string line;
    std::vector<float> vertices;
    std::vector<float> textureCoords;
    std::vector<float> normals;
    std::vector<float> tangents;

    try{
        if(std::getline(fr,line)){
            while(true){

                std::vector<std::string> currentLine = Utils::split(line,' ');
                if(!currentLine.empty()){
                    vertices.push_back(strtof(currentLine[0].c_str(),NULL));vertices.push_back(strtof(currentLine[1].c_str(),NULL));vertices.push_back(strtof(currentLine[2].c_str(),NULL));
                    textureCoords.push_back(strtof(currentLine[3].c_str(),NULL));textureCoords.push_back(strtof(currentLine[4].c_str(),NULL));textureCoords.push_back(strtof(currentLine[5].c_str(),NULL));
                    normals.push_back(strtof(currentLine[6].c_str(),NULL));normals.push_back(strtof(currentLine[7].c_str(),NULL));normals.push_back(strtof(currentLine[8].c_str(),NULL));
                    tangents.push_back(strtof(currentLine[9].c_str(),NULL));tangents.push_back(strtof(currentLine[10].c_str(),NULL));tangents.push_back(strtof(currentLine[11].c_str(),NULL));

                }
                if(!std::getline(fr,line))break;
            }
        }
        fr.close();

    }catch(std::ifstream::failure e){
        std::cerr << "read failure\n";
    }


    try {
         fr.open (indiceFile.c_str(), std::ifstream ::in);
    } catch (std::ifstream::failure e) {
        std::cerr << "Couldn't load file!\n";
    }

    std::vector<int> indices;

    try{
        if(std::getline(fr,line)){
            while(true){

                std::vector<std::string> currentLine = Utils::split(line,' ');
                if(!currentLine.empty()){
                    indices.push_back(atoi(currentLine[0].c_str()));
                }
                if(!std::getline(fr,line))break;
            }
        }
        fr.close();

    }catch(std::ifstream::failure e){
        std::cerr << "read failure\n";
    }
    std::cout << vertices.size() << " " << indices.size() << std::endl;*/
    boost::filesystem::directory_iterator end_itr;
    std::string finalPath;
    for (boost::filesystem::directory_iterator itr(bpath); itr != end_itr; ++itr)
        {
            // If it's not a directory, list it. If you want to list directories too, just remove this check.
            if (boost::filesystem::is_regular_file(itr->path())) {
                // assign current file name to current_file and echo it out to the console.
                std::string current_file = itr->path().string();
                if(current_file.find(".obj") != std::string::npos || current_file.find(".FBX") != std::string::npos){
                    finalPath = current_file;
                    break;
                }
            }
    }

    const aiScene* scene = aiImportFile(finalPath.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);

    if(!scene)
    {
       std::cerr << "Couldn't load model Error Importing Asset :" << objectname << std::endl;;
    }

    Object3DStatic* object = new Object3DStatic();

    object->createModel(scene);
    selectedEntity = object;
    emit oneEntityHaveBeenSelected(selectedEntity);
    StaticObjects.push_back(object);
}

void Scene::addObject3DStatic(Object3DStatic *object)
{
    selectedEntity = object;
    emit oneEntityHaveBeenSelected(selectedEntity);
    StaticObjects.push_back(object);
}

void Scene::cleanUp()
{

}
