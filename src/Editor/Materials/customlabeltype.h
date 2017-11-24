#ifndef CUSTOMLABELTYPE_H
#define CUSTOMLABELTYPE_H
#include <QObject>
#include <QLabel>
#include "src/Engine/Materials/texture.h"


class CustomLabelType:public QLabel
{
    Q_OBJECT
public:
    CustomLabelType(QString path = "", bool isHdriLoader = false, QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(Texture*);
private:
    bool m_isHdriLoader;
};

#endif // CUSTOMLABELTYPE_H
