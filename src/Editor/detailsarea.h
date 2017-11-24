#ifndef DETAILSAREA_H
#define DETAILSAREA_H
#include <QToolBox>
#include "src/Engine/Entities/object3dstatic.h"

class DetailsArea: public QToolBox
{
    Q_OBJECT
public:
    explicit DetailsArea( QWidget *parent = 0);
public slots:
    void initDetailArena(Object3DStatic* entity);
    void setTransformationLX(double value);
    void setTransformationLY(double value);
    void setTransformationLZ(double value);

    void setTransformationRX(double value);
    void setTransformationRY(double value);
    void setTransformationRZ(double value);

    void setTransformationSX(double value);
    void setTransformationSY(double value);
    void setTransformationSZ(double value);

private:
    Object3DStatic* m_entity;
};

#endif // DETAILSAREA_H
