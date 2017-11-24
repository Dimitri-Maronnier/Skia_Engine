#ifndef GLSLTREEWIDGETITEM_H
#define GLSLTREEWIDGETITEM_H
#include <QString>
#include <QStringList>
#include <QTreeWidgetItem>

class GlslTreeWidgetItem : public QTreeWidgetItem
{
public:
    GlslTreeWidgetItem();
    GlslTreeWidgetItem(QString name, QString type, QStringList inputs, QStringList outputs, bool hasImage, bool isVariable, int nbParams, QString outputsType, bool isUniform, QString code);

    QString getName();
    QString getType();
    QStringList getInputs();
    QStringList getOutputs();
    bool getHasImage();
    bool getIsVariable();
    int getNbParams();
    bool getIsUniform();
    QString getOutputsType();
    QString getCode();

private:
    QString m_name;
    QString m_type;
    QStringList m_inputs;
    QStringList m_outputs;
    bool m_hasImage;
    bool m_isVariable;
    int m_nbParams;
    QString m_code;
    QString m_outputsType;
    bool m_isUniform;
};

#endif // GLSLTREEWIDGETITEM_H
