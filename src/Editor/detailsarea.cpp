#include "detailsarea.h"
#include <iostream>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <glm/vec3.hpp>
#include <limits>

DetailsArea::DetailsArea( QWidget *parent) : QToolBox(parent)
{
    this->setVisible(false);
    this->setEnabled(false);
}

void DetailsArea::initDetailArena(Object3DStatic* entity)
{

    m_entity = entity;
    this->setVisible(true);
    this->setEnabled(true);
    QGridLayout * layout = (QGridLayout *)this->currentWidget()->children().at(0)->children().at(0);
    QDoubleSpinBox *locationX = (QDoubleSpinBox *)layout->itemAtPosition(0,1)->widget();
    QDoubleSpinBox *locationY = (QDoubleSpinBox *)layout->itemAtPosition(0,2)->widget();
    QDoubleSpinBox *locationZ = (QDoubleSpinBox *)layout->itemAtPosition(0,3)->widget();

    QDoubleSpinBox *rotationX = (QDoubleSpinBox *)layout->itemAtPosition(1,1)->widget();
    QDoubleSpinBox *rotationY = (QDoubleSpinBox *)layout->itemAtPosition(1,2)->widget();
    QDoubleSpinBox *rotationZ = (QDoubleSpinBox *)layout->itemAtPosition(1,3)->widget();

    QDoubleSpinBox *scaleX = (QDoubleSpinBox *)layout->itemAtPosition(2,1)->widget();
    QDoubleSpinBox *scaleY = (QDoubleSpinBox *)layout->itemAtPosition(2,2)->widget();
    QDoubleSpinBox *scaleZ = (QDoubleSpinBox *)layout->itemAtPosition(2,3)->widget();

    locationX->setMaximum(std::numeric_limits<float>::max());
    locationX->setMinimum(-std::numeric_limits<float>::max());
    locationY->setMaximum(std::numeric_limits<float>::max());
    locationY->setMinimum(-std::numeric_limits<float>::max());
    locationZ->setMaximum(std::numeric_limits<float>::max());
    locationZ->setMinimum(-std::numeric_limits<float>::max());

    rotationX->setMaximum(std::numeric_limits<float>::max());
    rotationX->setMinimum(-std::numeric_limits<float>::max());
    rotationY->setMaximum(std::numeric_limits<float>::max());
    rotationY->setMinimum(-std::numeric_limits<float>::max());
    rotationZ->setMaximum(std::numeric_limits<float>::max());
    rotationZ->setMinimum(-std::numeric_limits<float>::max());

    scaleX->setMaximum(std::numeric_limits<float>::max());
    scaleX->setMinimum(-std::numeric_limits<float>::max());
    scaleY->setMaximum(std::numeric_limits<float>::max());
    scaleY->setMinimum(-std::numeric_limits<float>::max());
    scaleZ->setMaximum(std::numeric_limits<float>::max());
    scaleZ->setMinimum(-std::numeric_limits<float>::max());

    glm::vec3 position = entity->getPostion();
    glm::vec3 rotation = entity->getRotation();
    glm::vec3 scale = entity->getScale();

    locationX->setValue(position.x);
    locationY->setValue(position.y);
    locationZ->setValue(position.z);

    rotationX->setValue(rotation.x);
    rotationY->setValue(rotation.y);
    rotationZ->setValue(rotation.z);

    scaleX->setValue(scale.x);
    scaleY->setValue(scale.y);
    scaleZ->setValue(scale.z);

    connect(locationX,SIGNAL(valueChanged(double)),this,SLOT(setTransformationLX(double)));
    connect(locationY,SIGNAL(valueChanged(double)),this,SLOT(setTransformationLY(double)));
    connect(locationZ,SIGNAL(valueChanged(double)),this,SLOT(setTransformationLZ(double)));

    connect(rotationX,SIGNAL(valueChanged(double)),this,SLOT(setTransformationRX(double)));
    connect(rotationY,SIGNAL(valueChanged(double)),this,SLOT(setTransformationRY(double)));
    connect(rotationZ,SIGNAL(valueChanged(double)),this,SLOT(setTransformationRZ(double)));

    connect(scaleX,SIGNAL(valueChanged(double)),this,SLOT(setTransformationSX(double)));
    connect(scaleY,SIGNAL(valueChanged(double)),this,SLOT(setTransformationSY(double)));
    connect(scaleZ,SIGNAL(valueChanged(double)),this,SLOT(setTransformationSZ(double)));

}

void DetailsArea::setTransformationLX(double value)
{
    m_entity->setPositionX(value);
}

void DetailsArea::setTransformationLY(double value)
{
    m_entity->setPositionY(value);
}

void DetailsArea::setTransformationLZ(double value)
{
    m_entity->setPositionZ(value);
}

void DetailsArea::setTransformationRX(double value)
{
    m_entity->setRotationX(value);
}

void DetailsArea::setTransformationRY(double value)
{
    m_entity->setRotationY(value);
}

void DetailsArea::setTransformationRZ(double value)
{
    m_entity->setRotationZ(value);
}

void DetailsArea::setTransformationSX(double value)
{
    m_entity->setScaleX(value);
}

void DetailsArea::setTransformationSY(double value)
{
    m_entity->setScaleY(value);
}

void DetailsArea::setTransformationSZ(double value)
{
    m_entity->setScaleZ(value);
}

