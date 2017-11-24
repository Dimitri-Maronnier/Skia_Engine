#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <QMainWindow>
#include "src/Engine/Materials/texture.h"

namespace Ui {
    class ImageEditor;
}


class ImageEditor : public QMainWindow
{
    Q_OBJECT
public:
    explicit ImageEditor( QWidget *parent = 0);
    void init(QString path="");
    ~ImageEditor();
signals:

public slots:
    void setMinFiltering(int index);
    void setMagFiltering(int index);
    void setWrapS(int index);
    void setWrapT(int index);
    void setUseMipMap(bool check);
    void setBaseMipMap(double value);
    void setMaxMipMap(double value);
    void setBiasMipMap(double value);
    void setUseAnisotropy(bool check);
    void setQuantityAnisotropy(double value);
    void setCompressionFormat(int index);

    void saveTexture();

private:
    Ui::ImageEditor *ui;
    Texture *m_textureData;
    QVector<QString> list_files;
    QString ext;
    QString m_archive;
};

#endif // IMAGEEDITOR_H
