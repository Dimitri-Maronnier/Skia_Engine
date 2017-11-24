#ifndef NODEMATERIAL_H
#define NODEMATERIAL_H
#include "src/qnodeseditor/qneblock.h"
#include "src/qnodeseditor/qneport.h"
#include <QGraphicsScene>
#include <QObject>
#include "src/Editor/Materials/glsltreewidgetitem.h"
#include "src/Editor/Materials/Nodes/nodebase.h"
#include <vector>
#include <QGraphicsPixmapItem>
#include <QDataStream>
#include "src/Engine/Materials/texture.h"

enum TypeVarType{Default=0,Bool=1,Float=2,vec2=3,vec3=4,vec4=5,mat2=6,mat3=7,mat4=8};

class Param:public QObject
{
   Q_OBJECT
    public:
        Param(float v,bool b){ value = v; this->b = b;}
        float value;
        bool b;
     public slots:
        void changeParam(double v){
            value = v;
            emit paramHaveBeenChanged(value,b);
        }
        void changeBool(bool b){
            this->b = b;
            emit paramHaveBeenChanged(value,b);
        }

     signals:
        void paramHaveBeenChanged(float,bool);
};

class NodeMaterial:public QObject
{
   Q_OBJECT

public:
    NodeMaterial();
    NodeMaterial(QPoint pos, QGraphicsScene *scene, GlslTreeWidgetItem *item, NodeBase *nodeBase);
    NodeMaterial(QGraphicsScene *scene,QNEBlock* b,NodeBase* nodeBase);
    ~NodeMaterial();
    QString getFragmentSource();
    QString getName();
    bool isVariable();
    bool haveImage();
    int getNbParams();
    bool isUniform();
    QNEBlock* getQNEBlock();
    Param* getParam(int i);
    QString getVarName();
    std::vector<NodeMaterial*> getParents();
    QString getImagePath();
    int getBinding();
    void freeBinding();
    void setTypeVar();
    bool isAbstract();
    void setDefaultValue();
    QString getOutputType();
    TypeVarType getTypeVar();
    std::map<QString,QStringList> getSubOutputsType();

    void save(QDataStream &ds);
    void load(QDataStream &ds);

    void addPort(QNEPort* port);

private:
    QString m_name;
    QString m_type;
    QStringList m_inputs;
    QStringList m_outputs;
    QString m_outputsType;
    bool m_isVariable;
    bool m_hasImage;
    bool m_isUniform;
    QNEBlock *m_b;
    QGraphicsScene *m_scene;
    QString m_fragmentSource;
    QString m_fragmentSourceOld;
    QGraphicsPixmapItem* m_item;

    std::vector<NodeMaterial*> m_parents;
    std::vector<NodeMaterial*> m_child;
    QString m_varName;
    int m_nbParams;
    QNEPort* m_PortType;
    std::vector<Param*> m_value;

    static std::map<std::string,int> occurence;
    static std::vector<int> bindingAvailable;
    bool m_imageLoad;
    int m_binding;
    int m_isAbstract;
    QString m_imagePath;
    TypeVarType m_returnType;
    QVector<QNEPort*> inputs;
    QVector<QNEPort*> outputs;

    std::map<QString,QStringList> m_subOutputsType;

public slots:
    void oneInputPortHaveBeenConnect(QNEPort*in, QNEPort *out);
    void onePortHaveBeenDisconnect(QNEPort*in,QNEPort*out);
    void paramChanged(float value, bool b);
    void changeImage(Texture *texture);

signals:
    void onFragmentSourceChanged(QString);
};

#endif // NODEMATERIAL_H
