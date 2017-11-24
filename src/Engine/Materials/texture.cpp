#include "texture.h"
#include <GL/glut.h>
#include <iostream>
#include <algorithm>
#include "src/Loaders/loader.h"
#include "src/Editor/Utils/compressor.h"

Texture::Texture()
    :Asset(-1, "", "")
{
    m_min_filter = GL_LINEAR;
    m_mag_filter = GL_LINEAR;
    m_s_wrap = GL_REPEAT;
    m_t_wrap = GL_REPEAT;
    m_useMipMap = true;
    m_baseLevel = 0.0f;
    m_maxLevel = 4.0f;
    m_lodBias = 0.0f;

    m_useAnisotropy = GLEW_EXT_texture_filter_anisotropic;
    m_quantity = GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT;
    m_useCompression = false;
    m_compressionFormat = GL_COMPRESSED_RGBA;

}

Texture::Texture(const unsigned int handle, const std::string name, const std::string path)
    :Asset(handle, name, path)
{

    QVector<QString> list_files = Compressor::uncompressTextureData(QString(path.c_str()),this);
    if(list_files.size()==0){
        std::cerr << "Couldn't decompress Texture Data" << std::endl;

    }else{

        foreach(QString file,list_files){
            if(file.contains("texture", Qt::CaseSensitive)){

                if(!m_isHDR)
                    m_textureID = Loader::loadTexture((char*)file.toStdString().c_str(),this);
                else
                    m_textureID = Loader::loadHdr((char*)file.toStdString().c_str());
            }
            else if(file.contains("thumnail", Qt::CaseSensitive)){
                m_thumnail.load(file);
                break;
            }
        }
    }


}

Texture::Texture(const unsigned int handle, const std::string name, const std::string path,GLuint min_filter,GLuint max_filter,GLuint s_wrap, GLuint t_wrap, bool useMipMap,float baseLevel, float maxLevel, float lodBias, bool useAnisotropy, float quantity,bool useCompresion, GLuint compressionFormat)
:Asset(handle, name, path)
{
    m_min_filter = min_filter;
    m_mag_filter = max_filter;
    m_s_wrap = s_wrap;
    m_t_wrap = t_wrap;
    m_useMipMap = useMipMap;
    float MAXLEVEL = 1000;
    m_baseLevel = std::min(std::min(baseLevel,MAXLEVEL),maxLevel);
    m_maxLevel = std::min(maxLevel,MAXLEVEL);
    m_lodBias = lodBias;

    float QUANTITY;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&QUANTITY);
    m_useAnisotropy = useAnisotropy && GLEW_EXT_texture_filter_anisotropic;
    m_quantity = std::min(quantity,QUANTITY);
    m_useCompression = useCompresion;
    m_compressionFormat = compressionFormat;
}

void Texture::reLoad()
{
    QVector<QString> list_files = Compressor::uncompressTextureData(QString(this->GetPath().c_str()),this);
    if(list_files.size()==0){
        std::cerr << "Couldn't decompress Texture Data" << std::endl;

    }else{
        foreach(QString file,list_files){
            if(file.contains("texture", Qt::CaseSensitive)){
                m_textureID = Loader::loadTexture((char*)file.toStdString().c_str(),this);
            }
            else if(file.contains("thumnail", Qt::CaseSensitive)){
                m_thumnail.load(file);
                break;
            }
        }
    }
}

GLuint Texture::getMinFilter()
{
    return m_min_filter;
}

GLuint Texture::getMagFilter()
{
    return m_mag_filter;
}

GLuint Texture::getWrapS()
{
    return m_s_wrap;
}

GLuint Texture::getWrapT()
{
    return m_t_wrap;
}

bool Texture::useMipMap()
{
    return m_useMipMap;
}

float Texture::getBaseLevelMipMap()
{
    return m_baseLevel;
}

float Texture::getMaxLevelMipMap()
{
    return m_maxLevel;
}

float Texture::getLodBiasMipMap()
{
    return m_lodBias;
}

bool Texture::useAnisotropy()
{
    return m_useAnisotropy;
}

float Texture::getQuantityAnisotropy()
{
    return m_quantity;
}

bool Texture::useCompression()
{
    return m_useCompression;
}

GLuint Texture::getCompressionFormat()
{
    return m_compressionFormat;
}

GLuint Texture::getTextureID()
{
    return m_textureID;
}

QPixmap Texture::getThumnail()
{
    return m_thumnail;
}

bool Texture::isHDR()
{
    return m_isHDR;
}

void Texture::setMinFilter(GLuint min_filter)
{
    m_min_filter = min_filter;
}

void Texture::setMagFilter(GLuint mag_filter)
{
    m_mag_filter = mag_filter;
}

void Texture::setWrapS(GLuint s_wrap)
{
    m_s_wrap = s_wrap;
}

void Texture::setWrapT(GLuint t_wrap)
{
    m_t_wrap = t_wrap;
}

void Texture::setUseMipMap(bool useMipMap)
{
    m_useMipMap = useMipMap;
}

void Texture::setBaseLevelMipMap(float baseLevel)
{
    float MAXLEVEL = 9;
    m_baseLevel = std::min(std::min(baseLevel,MAXLEVEL),m_maxLevel);
}

void Texture::setMaxLevelMipMap(float maxLevel)
{
    float MAXLEVEL = 1000;
    m_maxLevel = std::min(maxLevel,MAXLEVEL);
}

void Texture::setLodBiasMipMap(float lodBias)
{
    m_lodBias = lodBias;
}

void Texture::setUseAnisotropy(bool useAnisotropy)
{
    m_useAnisotropy = useAnisotropy && GLEW_EXT_texture_filter_anisotropic;
}

void Texture::setQuantityAnisotropy(float quantity)
{
    float QUANTITY;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&QUANTITY);
    m_quantity = std::min(quantity,QUANTITY);
}

void Texture::setUseCompression(bool useCompression)
{
    m_useCompression = useCompression;
}

void Texture::setCompressionFormat(GLuint compressionFormat)
{
    m_compressionFormat = compressionFormat;
}

void Texture::setIsHDR(bool hdr)
{
    m_isHDR = hdr;
}
