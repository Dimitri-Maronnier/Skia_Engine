#include "ui_imageeditor.h"
#include "imageeditor.h"
#include "src/Editor/Utils/compressor.h"
#include <iostream>
#include "src/utils.h"
#include "src/Assets/assetscollections.h"

ImageEditor::ImageEditor(QWidget * parent) : QMainWindow(parent),
    ui(new Ui::ImageEditor)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->gridLayout);


}

void ImageEditor::init(QString path)
{
    std::vector<std::string> split = Utils::split(path.toStdString(),'\\');

    unsigned int handle = AssetsCollections::TexturesCollection.Add(split.at(split.size()-1),path.toStdString());

    AssetsCollections::HandlesTextures.push_back(handle);
    list_files = Compressor::uncompressTextureData(path,AssetsCollections::TexturesCollection[handle]);
    m_textureData = AssetsCollections::TexturesCollection[handle];

    if(list_files.size()==0){
        std::cerr << "Couldn't decompress Texture Data" << std::endl;
        ui->label->setText("Error texture");
    }else{
        foreach(QString file,list_files){
            if(file.contains("texture", Qt::CaseSensitive)){
                ext = QString(Utils::getExtension(file.toStdString()).c_str());
                QPixmap image(file);
                ui->label->setPixmap(image);
                break;
            }
        }
    }
    m_archive = path;
    /*Filtering*/
    switch(m_textureData->getMinFilter()){
        case GL_LINEAR:
        ui->MinFilterCombo->setCurrentIndex(0);
        break;
        case GL_NEAREST:
        ui->MinFilterCombo->setCurrentIndex(1);
        break;
        case GL_NEAREST_MIPMAP_NEAREST:
        ui->MinFilterCombo->setCurrentIndex(2);
        break;
        case GL_NEAREST_MIPMAP_LINEAR:
        ui->MinFilterCombo->setCurrentIndex(3);
        break;
        case GL_LINEAR_MIPMAP_NEAREST:
        ui->MinFilterCombo->setCurrentIndex(4);
        break;
        case GL_LINEAR_MIPMAP_LINEAR:
        ui->MinFilterCombo->setCurrentIndex(5);
        break;
        default:
        ui->MinFilterCombo->setCurrentIndex(0);
        break;
    }

    switch(m_textureData->getMagFilter()){
        case GL_LINEAR:
        ui->MagFilterCombo->setCurrentIndex(0);
        break;
        case GL_NEAREST:
        ui->MagFilterCombo->setCurrentIndex(1);
        break;
        case GL_NEAREST_MIPMAP_NEAREST:
        ui->MagFilterCombo->setCurrentIndex(2);
        break;
        case GL_NEAREST_MIPMAP_LINEAR:
        ui->MagFilterCombo->setCurrentIndex(3);
        break;
        case GL_LINEAR_MIPMAP_NEAREST:
        ui->MagFilterCombo->setCurrentIndex(4);
        break;
        case GL_LINEAR_MIPMAP_LINEAR:
        ui->MagFilterCombo->setCurrentIndex(5);
        break;
        default:
        ui->MagFilterCombo->setCurrentIndex(0);
        break;
    }

    /*Wrapping*/

    switch(m_textureData->getWrapS()){
        case GL_REPEAT:
        ui->WrapSCombo->setCurrentIndex(0);
        break;
        case GL_MIRRORED_REPEAT:
        ui->WrapSCombo->setCurrentIndex(1);
        break;
        case GL_CLAMP_TO_EDGE:
        ui->WrapSCombo->setCurrentIndex(2);
        break;
        case GL_CLAMP_TO_BORDER:
        ui->WrapSCombo->setCurrentIndex(3);
        break;
        default:
        ui->WrapSCombo->setCurrentIndex(0);
        break;
    }

    switch(m_textureData->getWrapT()){
        case GL_REPEAT:
        ui->WrapTCombo->setCurrentIndex(0);
        break;
        case GL_MIRRORED_REPEAT:
        ui->WrapTCombo->setCurrentIndex(1);
        break;
        case GL_CLAMP_TO_EDGE:
        ui->WrapTCombo->setCurrentIndex(2);
        break;
        case GL_CLAMP_TO_BORDER:
        ui->WrapTCombo->setCurrentIndex(3);
        break;
        default:
        ui->WrapTCombo->setCurrentIndex(0);
        break;
    }

    /*MipMapping*/
    ui->UseMipMap->setChecked(m_textureData->useMipMap());
    ui->BaseSpin->setValue(m_textureData->getBaseLevelMipMap());
    ui->MaxSpin->setValue(m_textureData->getMaxLevelMipMap());

    /*Anisotropy*/
    ui->UseAnisotropy->setChecked(m_textureData->useAnisotropy());
    ui->QuatitySpin->setValue(m_textureData->getQuantityAnisotropy());

    /*Compression*/
    if(m_textureData->useCompression()){
        switch(m_textureData->getCompressionFormat()){
            case GL_COMPRESSED_RED:
            ui->CompressionCombo->setCurrentIndex(1);
            break;
            case GL_COMPRESSED_RG:
            ui->CompressionCombo->setCurrentIndex(2);
            break;
            case GL_COMPRESSED_RGB:
            ui->CompressionCombo->setCurrentIndex(3);
            break;
            case GL_COMPRESSED_RGBA:
            ui->CompressionCombo->setCurrentIndex(4);
            break;
            case GL_COMPRESSED_SRGB:
            ui->CompressionCombo->setCurrentIndex(5);
            break;
            case GL_COMPRESSED_SRGB_ALPHA:
            ui->CompressionCombo->setCurrentIndex(6);
            break;
            case GL_COMPRESSED_RED_RGTC1:
            ui->CompressionCombo->setCurrentIndex(7);
            break;
            case GL_COMPRESSED_SIGNED_RED_RGTC1:
            ui->CompressionCombo->setCurrentIndex(8);
            break;
            case GL_COMPRESSED_RG_RGTC2:
            ui->CompressionCombo->setCurrentIndex(9);
            break;
            case GL_COMPRESSED_SIGNED_RG_RGTC2:
            ui->CompressionCombo->setCurrentIndex(10);
            break;
            case GL_COMPRESSED_RGBA_BPTC_UNORM:
            ui->CompressionCombo->setCurrentIndex(11);
            break;
            case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
            ui->CompressionCombo->setCurrentIndex(12);
            break;
            case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT:
            ui->CompressionCombo->setCurrentIndex(13);
            break;
            case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:
            ui->CompressionCombo->setCurrentIndex(14);
            break;
            case GL_COMPRESSED_RGB8_ETC2:
            ui->CompressionCombo->setCurrentIndex(15);
            break;
            case GL_COMPRESSED_SRGB8_ETC2:
            ui->CompressionCombo->setCurrentIndex(16);
            break;
            case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
            ui->CompressionCombo->setCurrentIndex(17);
            break;
            case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
            ui->CompressionCombo->setCurrentIndex(18);
            break;
            case GL_COMPRESSED_RGBA8_ETC2_EAC:
            ui->CompressionCombo->setCurrentIndex(19);
            break;
            case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
            ui->CompressionCombo->setCurrentIndex(20);
            break;
            case GL_COMPRESSED_R11_EAC:
            ui->CompressionCombo->setCurrentIndex(21);
            break;
            case GL_COMPRESSED_SIGNED_R11_EAC:
            ui->CompressionCombo->setCurrentIndex(22);
            break;
            case GL_COMPRESSED_RG11_EAC:
            ui->CompressionCombo->setCurrentIndex(23);
            break;
            case GL_COMPRESSED_SIGNED_RG11_EAC:
            ui->CompressionCombo->setCurrentIndex(24);
            break;
            default:
            ui->CompressionCombo->setCurrentIndex(0);
            break;
        }
    }else{
        ui->CompressionCombo->setCurrentIndex(0);
    }

    connect(ui->MinFilterCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setMinFiltering(int)));
    connect(ui->MagFilterCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setMagFiltering(int)));
    connect(ui->WrapSCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setWrapS(int)));
    connect(ui->WrapTCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setWrapT(int)));
    connect(ui->CompressionCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setCompressionFormat(int)));

    connect(ui->UseMipMap,SIGNAL(clicked(bool)),this,SLOT(setUseMipMap(bool)));
    connect(ui->UseAnisotropy,SIGNAL(clicked(bool)),this,SLOT(setUseAnisotropy(bool)));

    connect(ui->BaseSpin,SIGNAL(valueChanged(double)),this,SLOT(setBaseMipMap(double)));
    connect(ui->MaxSpin,SIGNAL(valueChanged(double)),this,SLOT(setMaxMipMap(double)));
    connect(ui->BiasSpin,SIGNAL(valueChanged(double)),this,SLOT(setBiasMipMap(double)));
    connect(ui->QuatitySpin,SIGNAL(valueChanged(double)),this,SLOT(setQuantityAnisotropy(double)));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(saveTexture()));
}

ImageEditor::~ImageEditor()
{
    delete ui;
    //SAFE_DELETE(m_textureData);
}

void ImageEditor::setMinFiltering(int index)
{
    switch(index){
        case 0:
        m_textureData->setMinFilter(GL_LINEAR);
        break;
        case 1:
        m_textureData->setMinFilter(GL_NEAREST);
        break;
        case 2:
        m_textureData->setMinFilter(GL_NEAREST_MIPMAP_NEAREST);
        break;
        case 3:
        m_textureData->setMinFilter(GL_NEAREST_MIPMAP_LINEAR);
        break;
        case 4:
        m_textureData->setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
        break;
        case 5:
        m_textureData->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
        break;
        default:
        m_textureData->setMinFilter(GL_LINEAR);
        break;
    }
}

void ImageEditor::setMagFiltering(int index)
{
    switch(index){
        case 0:
        m_textureData->setMagFilter(GL_LINEAR);
        break;
        case 1:
        m_textureData->setMagFilter(GL_NEAREST);
        break;
        case 2:
        m_textureData->setMagFilter(GL_NEAREST_MIPMAP_NEAREST);
        break;
        case 3:
        m_textureData->setMagFilter(GL_NEAREST_MIPMAP_LINEAR);
        break;
        case 4:
        m_textureData->setMagFilter(GL_LINEAR_MIPMAP_NEAREST);
        break;
        case 5:
        m_textureData->setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
        break;
        default:
        m_textureData->setMagFilter(GL_LINEAR);
        break;
    }
}

void ImageEditor::setWrapS(int index)
{
    switch(index){
        case 0:
        m_textureData->setWrapS(GL_REPEAT);
        break;
        case 1:
        m_textureData->setWrapS(GL_MIRRORED_REPEAT);
        break;
        case 2:
        m_textureData->setWrapS(GL_CLAMP_TO_EDGE);
        break;
        case 3:
        m_textureData->setWrapS(GL_CLAMP_TO_BORDER);
        break;
        default:
        m_textureData->setWrapS(GL_REPEAT);
        break;
    }
}

void ImageEditor::setWrapT(int index)
{
    switch(index){
        case 0:
        m_textureData->setWrapT(GL_REPEAT);
        break;
        case 1:
        m_textureData->setWrapT(GL_MIRRORED_REPEAT);
        break;
        case 2:
        m_textureData->setWrapT(GL_CLAMP_TO_EDGE);
        break;
        case 3:
        m_textureData->setWrapT(GL_CLAMP_TO_BORDER);
        break;
        default:
        m_textureData->setWrapT(GL_REPEAT);
        break;
    }
}

void ImageEditor::setUseMipMap(bool check)
{
    m_textureData->setUseMipMap(check);
}

void ImageEditor::setBaseMipMap(double value)
{
    m_textureData->setBaseLevelMipMap(value);
}

void ImageEditor::setMaxMipMap(double value)
{
    m_textureData->setMaxLevelMipMap(value);
}

void ImageEditor::setBiasMipMap(double value)
{
    m_textureData->setLodBiasMipMap(value);
}

void ImageEditor::setUseAnisotropy(bool check)
{
    m_textureData->setUseAnisotropy(check);
}

void ImageEditor::setQuantityAnisotropy(double value)
{
    m_textureData->setQuantityAnisotropy(value);
}

void ImageEditor::setCompressionFormat(int index)
{
    m_textureData->setUseCompression(true);
    switch(index){
        case 0:
        m_textureData->setUseCompression(false);
        m_textureData->setCompressionFormat(GL_COMPRESSED_RED);
        break;
        case 1:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RED);
        break;
        case 2:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RG);
        break;
        case 3:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RGB);
        break;
        case 4:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RGBA);
        break;
        case 5:
        m_textureData->setCompressionFormat(GL_COMPRESSED_SRGB);
        break;
        case 6:
        m_textureData->setCompressionFormat(GL_COMPRESSED_SRGB_ALPHA);
        break;
        case 7:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RED_RGTC1);
        break;
        case 8:
        m_textureData->setCompressionFormat(GL_COMPRESSED_SIGNED_RED_RGTC1);
        break;
        case 9:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RG_RGTC2);
        break;
        case 10:
        m_textureData->setCompressionFormat(GL_COMPRESSED_SIGNED_RG_RGTC2);
        break;
        case 11:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RGBA_BPTC_UNORM);
        break;
        case 12:
        m_textureData->setCompressionFormat(GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM);
        break;
        case 13:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT);
        break;
        case 14:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT);
        break;
        case 15:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RGB8_ETC2);
        break;
        case 16:
        m_textureData->setCompressionFormat(GL_COMPRESSED_SRGB8_ETC2);
        break;
        case 17:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2);
        break;
        case 18:
        m_textureData->setCompressionFormat(GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2);
        break;
        case 19:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RGBA8_ETC2_EAC);
        break;
        case 20:
        m_textureData->setCompressionFormat(GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC);
        break;
        case 21:
        m_textureData->setCompressionFormat(GL_COMPRESSED_R11_EAC);
        break;
        case 22:
        m_textureData->setCompressionFormat(GL_COMPRESSED_SIGNED_R11_EAC);
        break;
        case 23:
        m_textureData->setCompressionFormat(GL_COMPRESSED_RG11_EAC);
        break;
        case 24:
        m_textureData->setCompressionFormat(GL_COMPRESSED_SIGNED_RG11_EAC);
        break;
        default:
        m_textureData->setUseCompression(false);
        m_textureData->setCompressionFormat(GL_LINEAR);
        break;
    }

}

void ImageEditor::saveTexture()
{

    QVector<QString> fileNameToArchive;
    QString textureFile = "texture";
    textureFile += ext;

    fileNameToArchive.push_back(textureFile);
    fileNameToArchive.push_back("thumnail.png");

    if(!Compressor::compressTextureData(m_archive,list_files,fileNameToArchive,*m_textureData)){
        std::cerr << "Couldn't compress texture data" << std::endl;
    }

    m_textureData->reLoad();

}
