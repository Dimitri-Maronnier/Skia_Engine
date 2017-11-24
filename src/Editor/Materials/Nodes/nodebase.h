#ifndef NODEBASE_H
#define NODEBASE_H
#include "src/qnodeseditor/qneblock.h"
#include "src/Editor/Materials/Nodes/nodematerial.h"
#include <QGraphicsScene>
#include <QObject>
#include <QPoint>
#include <vector>
#include <algorithm>
#include <QDataStream>

class NodeMaterial;
class QNEBlock;
class QNEPort;

class NodeBase:public QObject
{
   Q_OBJECT

public:
    NodeBase();
    NodeBase(QNEBlock *b);
    NodeBase(QPoint pos,QGraphicsScene *scene);
    ~NodeBase();
    QString getFragmentSource();
    QString getVertexSource();
    std::vector<QString> getTexturesPath();

    void save(QDataStream &ds);
    void load(QDataStream &ds);

    void addPort(QNEPort *port);
    void emitt();

public slots:
    void oneInputPortHaveBeenConnect(QNEPort *in ,QNEPort *out);
    void onePortHaveBeenDisconnect(QNEPort *in, QNEPort *out);
    void buildShader();

private:
    void buildRecursive(NodeMaterial* node, QStringList *listUsedVar);
    QNEBlock *m_b;
    QGraphicsScene *m_scene;
    QString m_fragmentSource;
    QString m_vertexSource;
    std::vector<NodeMaterial*> m_parent;
    std::vector<int> m_parentIn;
    QString m_buildShaderString;
    QString m_buildShaderUniformString;
    QString m_fragmentSourceOld;
    std::vector<QString> m_texturesPath;
    QNEPort * port1;
    QNEPort * port2;
    QNEPort * port3;
    QNEPort * port4;
    QNEPort * port5;
    QNEPort * port6;

signals:
    void onFragmentSourceChanged(QString);

};

#endif // NODEBASE_H
