#include "customlabeltype.h"
#include <iostream>
#include <QDir>
#include <QFileDialog>
#include "src/Editor/Utils/foldergestion.h"
#include "src/Assets/assetscollections.h"
#include "src/utils.h"

CustomLabelType::CustomLabelType(QString path, bool isHdriLoader, QWidget *parent)
    : QLabel(parent)
{
    if(path.size()>0){
        QPixmap image(path);
        QSize myRessortSize(200,200);
        QPixmap myNewRessort = image.scaled(myRessortSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        this->setPixmap(myNewRessort);
    }else{
        setText("Pick Image");
    }

    m_isHdriLoader =  isHdriLoader;

}

void CustomLabelType::mousePressEvent(QMouseEvent *event)
{
    QString fileName;
    if(m_isHdriLoader){
        fileName =
                QFileDialog::getOpenFileName(nullptr,
                                             QObject::tr("Open HDR Texture"),
                                             QString(FolderGestion::rootProjectsFolderPath),
                                             QObject::tr("HDR Texture Files (*.shdrtex)"));
    }else{
        fileName =
                QFileDialog::getOpenFileName(nullptr,
                                             QObject::tr("Open Texture"),
                                             QString(FolderGestion::rootProjectsFolderPath),
                                             QObject::tr("Texture Files (*.stex)"));
    }
    if(fileName.at(0)!=0){

        std::replace( fileName.begin(), fileName.end(), '/', '\\');
        std::vector<std::string> split = Utils::split(fileName.toStdString(),'\\');
        /*unsigned int handle = AssetsCollections::TexturesCollection.Add(split.at(split.size()-1),fileName.toStdString());

        AssetsCollections::HandlesTextures.push_back(handle);*/
        unsigned int handle = AssetsCollections::addTexture(fileName.toStdString(),fileName.toStdString());
        if(!m_isHdriLoader)
            this->setPixmap(AssetsCollections::TexturesCollection[handle]->getThumnail());
        else
            this->setText(QString(split.at(split.size()-1).c_str()));
        emit clicked(AssetsCollections::TexturesCollection[handle]);
    }
}
