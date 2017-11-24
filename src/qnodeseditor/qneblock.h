/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef QNEBLOCK_H
#define QNEBLOCK_H
#include <QLabel>
#include <QGraphicsPathItem>
#include "src/Editor/Materials/Nodes/nodematerial.h"
#include "src/Editor/Materials/Nodes/nodebase.h"
#include "src/Editor/Materials/materialgraphwidget.h"

class QNEPort;
class NodeMaterial;
class NodeBase;
class MaterialGraphWidget;

class QNEBlock : public QObject,public QGraphicsPathItem
{
    Q_OBJECT
public:
	enum { Type = QGraphicsItem::UserType + 3 };

    QNEBlock(int w, int h, QGraphicsItem *parent = 0, NodeMaterial *node=NULL, NodeBase *nodeBase=NULL);
    ~QNEBlock();
    QNEPort* addPort(const QString &name, bool isOutput, NodeMaterial *node, NodeBase *nodeBase, int number, int flags = 0, int ptr = 0);
    QNEPort* addInputPort(const QString &name,NodeMaterial * node,NodeBase* nodeBase,int number);
    QNEPort *addOutputPort(const QString &name,NodeMaterial * node,NodeBase* nodeBase,int number);
    QVector<QNEPort *> addInputPorts(const QStringList &names,NodeMaterial * node,NodeBase* nodeBase);
    QVector<QNEPort*> addOutputPorts(const QStringList &names,NodeMaterial * node,NodeBase* nodeBase);
	void save(QDataStream&);
    NodeBase* load(QDataStream&, QMap<quint64, QNEPort*> &portMap, MaterialGraphWidget *graph);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QNEBlock* clone();
	QVector<QNEPort*> ports();
    void setSize(int w,int h);
    NodeBase *getNodeBase();
    int type() const { return Type; }

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	int horzMargin;
	int vertMargin;
	int width;
    int height;
    NodeMaterial* m_node;
    NodeBase* m_nodeBase;
    bool m_isSelected;
signals:
    void inputChanged(int);
    void outputChanged(int);
    void selected(NodeMaterial*);
};

#endif // QNEBLOCK_H
