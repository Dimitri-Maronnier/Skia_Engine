#include "glsltreewidgetitem.h"

GlslTreeWidgetItem::GlslTreeWidgetItem()
{

}

GlslTreeWidgetItem::GlslTreeWidgetItem(QString name, QString type, QStringList inputs, QStringList outputs, bool hasImage, bool isVariable, int nbParams, QString outputsType,bool isUniform, QString code)
{
    m_name = name;
    m_type = type;
    m_inputs = inputs;
    m_outputs = outputs;
    m_hasImage = hasImage;
    m_isVariable = isVariable;
    m_nbParams = nbParams;
    m_outputsType = outputsType;
    m_isUniform = isUniform;
    m_code = code;
}

QString GlslTreeWidgetItem::getName()
{
    return m_name;
}

QString GlslTreeWidgetItem::getType()
{
    return m_type;
}

QStringList GlslTreeWidgetItem::getInputs()
{
    return m_inputs;
}

QStringList GlslTreeWidgetItem::getOutputs()
{
    return m_outputs;
}

bool GlslTreeWidgetItem::getHasImage()
{
    return m_hasImage;
}

bool GlslTreeWidgetItem::getIsVariable()
{
    return m_isVariable;
}

int GlslTreeWidgetItem::getNbParams()
{
    return m_nbParams;
}

bool GlslTreeWidgetItem::getIsUniform()
{
    return m_isUniform;
}

QString GlslTreeWidgetItem::getOutputsType()
{
    return m_outputsType;
}

QString GlslTreeWidgetItem::getCode()
{
    return m_code;
}
