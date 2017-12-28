#ifndef TEXTURE_H
#define TEXTURE_H
#include <GL/glew.h>
#include <boost/uuid/uuid.hpp>
#include "src/Assets/asset.h"
#include <QPixmap>


class Texture :public Asset
{
public:
    Texture();
    ~Texture();
    Texture(const unsigned int handle, const std::string name, const std::string path );
    Texture(const unsigned int handle, const std::string name, const std::string path,GLuint min_filter,GLuint max_filter,GLuint s_wrap, GLuint t_wrap, bool useMipMap,float baseLevel, float maxLevel, float lodBias, bool useAnisotropy, float quantity,bool useCompresion, GLuint compressionFormat);

    void reLoad();

    GLuint getMinFilter();
    GLuint getMagFilter();
    GLuint getWrapS();
    GLuint getWrapT();
    bool useMipMap();
    float getBaseLevelMipMap();
    float getMaxLevelMipMap();
    float getLodBiasMipMap();
    bool useAnisotropy();
    float getQuantityAnisotropy();
    bool useCompression();
    GLuint getCompressionFormat();
    GLuint getTextureID();
    QPixmap getThumnail();
    bool isHDR();


    void setMinFilter(GLuint min_filter);
    void setMagFilter(GLuint mag_filter);
    void setWrapS(GLuint s_wrap);
    void setWrapT(GLuint t_wrap);
    void setUseMipMap(bool useMipMap);
    void setBaseLevelMipMap(float baseLevel);
    void setMaxLevelMipMap(float maxLevel);
    void setLodBiasMipMap(float lodBias);
    void setUseAnisotropy(bool useAnisotropy);
    void setQuantityAnisotropy(float quantity);
    void setUseCompression(bool useCompression);
    void setCompressionFormat(GLuint compressionFormat);
    void setIsHDR(bool hdr);

private:
    GLuint m_min_filter;
    GLuint m_mag_filter;
    GLuint m_s_wrap;
    GLuint m_t_wrap;
    bool m_useMipMap;
    float m_baseLevel;
    float m_maxLevel;
    float m_lodBias;
    bool m_useAnisotropy;
    float m_quantity;
    bool m_useCompression;
    GLuint m_compressionFormat;
    GLuint m_textureID;
    QPixmap m_thumnail;
    bool m_isHDR;

};

#endif // TEXTURE_H
