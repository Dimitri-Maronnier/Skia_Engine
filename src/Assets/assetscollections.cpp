#include "assetscollections.h"
#include "src/utils.h"

AssetsManager<Texture> AssetsCollections::TexturesCollection;
std::vector<unsigned int> AssetsCollections::HandlesTextures;

AssetsManager<Material> AssetsCollections::MaterialsCollection;
std::vector<unsigned int> AssetsCollections::HandlesMaterials;

AssetsManager<Object3DStatic> AssetsCollections::Object3DStaticCollection;
std::vector<unsigned int> AssetsCollections::HandlesObject3DStatic;

void AssetsCollections::cleanUp()
{

    HandlesTextures.clear();
    HandlesMaterials.clear();
    HandlesObject3DStatic.clear();
}

unsigned int AssetsCollections::addTexture(std::string name, std::string path)
{
    unsigned int handle;
    std::replace( name.begin(), name.end(), '/', '\\');
    std::vector<std::string> split = Utils::split(name,'\\');
    if(split.size()>1){
        handle = AssetsCollections::TexturesCollection.Add(split.at(split.size()-1),path);
        AssetsCollections::HandlesTextures.push_back(handle);
    }else{
        split = Utils::split(name,'/');
        if(split.size()>1){
            handle = AssetsCollections::TexturesCollection.Add(split.at(split.size()-1),path);
            AssetsCollections::HandlesTextures.push_back(handle);
        }else{
            handle = AssetsCollections::TexturesCollection.Add(name,path);
            AssetsCollections::HandlesTextures.push_back(handle);
        }
    }
    return handle;
}

unsigned int AssetsCollections::addObject3D(std::string name, std::string path)
{
    unsigned int handle;
    std::replace( name.begin(), name.end(), '/', '\\');
    std::vector<std::string> split = Utils::split(name,'\\');
    if(split.size()>1){
        handle = AssetsCollections::Object3DStaticCollection.Add(split.at(split.size()-1),path);
        AssetsCollections::HandlesObject3DStatic.push_back(handle);
    }else{
        split = Utils::split(name,'/');
        if(split.size()>1){
            handle = AssetsCollections::Object3DStaticCollection.Add(split.at(split.size()-1),path);
            AssetsCollections::HandlesObject3DStatic.push_back(handle);
        }else{
            handle = AssetsCollections::Object3DStaticCollection.Add(name,path);
            AssetsCollections::HandlesObject3DStatic.push_back(handle);
        }
    }
    return handle;
}
