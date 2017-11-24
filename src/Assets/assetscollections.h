#ifndef ASSETSCOLLECTIONS_H
#define ASSETSCOLLECTIONS_H

#include "src/Assets/assetsmanager.h"
#include "src/Engine/Materials/texture.h"
#include "src/Engine/Materials/material.h"
#include "src/Engine/Entities/object3dstatic.h"

class Material;
class Object3DStatic;

class AssetsCollections
{
public:
    static void cleanUp();
    static unsigned int addTexture(std::string name,std::string path);
    static unsigned int addObject3D(std::string name,std::string path);

    static AssetsManager<Texture> TexturesCollection;
    static std::vector<unsigned int> HandlesTextures;

    static AssetsManager<Material> MaterialsCollection;
    static std::vector<unsigned int> HandlesMaterials;

    static AssetsManager<Object3DStatic> Object3DStaticCollection;
    static std::vector<unsigned int> HandlesObject3DStatic;

};

#endif // ASSETSCOLLECTIONS_H
