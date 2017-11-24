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

#include "qneblock.h"

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include <QDir>
#include <QFileDialog>
#include <iostream>
#include "qneport.h"

QNEBlock::QNEBlock(int w,int h,QGraphicsItem *parent,NodeMaterial * node,NodeBase* nodeBase) : QGraphicsPathItem(parent)
{
	QPainterPath p;
    p.addRoundedRect(-50, -15, 100, 30, 5, 5);
	setPath(p);
	setPen(QPen(Qt::darkGreen));
	setBrush(Qt::green);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
    horzMargin = 20;
    vertMargin = 5;
    width = vertMargin;
    height = horzMargin;
    m_node = node;
    m_nodeBase = nodeBase;
    m_isSelected = false;

}

QNEBlock::~QNEBlock()
{
    if(m_node)
        m_node->freeBinding();
}

QNEPort* QNEBlock::addPort(const QString &name, bool isOutput, NodeMaterial * node,NodeBase* nodeBase,int number, int flags, int ptr)
{
    QNEPort *port = new QNEPort(this,node,nodeBase,number);
	port->setName(name);
	port->setIsOutput(isOutput);
	port->setNEBlock(this);
	port->setPortFlags(flags);
	port->setPtr(ptr);

	QFontMetrics fm(scene()->font());
	int w = fm.width(name);
	int h = fm.height();
	// port->setPos(0, height + h/2);
	if (w > width - horzMargin)
		width = w + horzMargin;
	height += h;

	QPainterPath p;
	p.addRoundedRect(-width/2, -height/2, width, height, 5, 5);
	setPath(p);

    int y = -height / 2 + vertMargin + port->radius();
    foreach(QGraphicsItem *port_, childItems()) {
		if (port_->type() != QNEPort::Type)
			continue;

		QNEPort *port = (QNEPort*) port_;
		if (port->isOutput())
			port->setPos(width/2 + port->radius(), y);
		else
			port->setPos(-width/2 - port->radius(), y);
		y += h;
	}

	return port;
}

QNEPort* QNEBlock::addInputPort(const QString &name,NodeMaterial * node,NodeBase* nodeBase,int number)
{
    return addPort(name, false,node,nodeBase,number);
}

QNEPort* QNEBlock::addOutputPort(const QString &name, NodeMaterial * node, NodeBase* nodeBase, int number)
{
    return addPort(name, true,node,nodeBase,number);
}

QVector<QNEPort*> QNEBlock::addInputPorts(const QStringList &names,NodeMaterial * node,NodeBase* nodeBase)
{
    QVector<QNEPort*> res;
    int i=0;
	foreach(QString n, names)
        res.append(addInputPort(n,node,nodeBase,i++));
    return res;
}

QVector<QNEPort*> QNEBlock::addOutputPorts(const QStringList &names,NodeMaterial * node,NodeBase* nodeBase)
{
    QVector<QNEPort*> res;
	foreach(QString n, names)
        res.append(addOutputPort(n,node,nodeBase,-5));
    return res;
}

void QNEBlock::save(QDataStream &ds)
{
	ds << pos();

	int count(0);

    foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() != QNEPort::Type)
			continue;

		count++;
	}

	ds << count;
    if(m_nodeBase){
        ds << 1;
        m_nodeBase->save(ds);
    }
    else{
        ds << 0;
        m_node->save(ds);
    }



    foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() != QNEPort::Type)
			continue;

		QNEPort *port = (QNEPort*) port_;
		ds << (quint64) port;
		ds << port->portName();
		ds << port->isOutput();
		ds << port->portFlags();
        ds << port->number();

	}
}

NodeBase* QNEBlock::load(QDataStream &ds, QMap<quint64, QNEPort*> &portMap,MaterialGraphWidget* graph)
{
	QPointF p;
	ds >> p;
	setPos(p);
	int count;
	ds >> count;
    int b;
    ds >> b;
    if(b){
        m_nodeBase = new NodeBase(this);
        m_nodeBase->load(ds);
    }
    else{
        if(graph->getNodeBase()){
            m_node = new NodeMaterial(this->scene(),this,graph->getNodeBase());
            m_node->load(ds);
        }
    }

	for (int i = 0; i < count; i++)
	{
		QString name;
		bool output;
		int flags;
		quint64 ptr;
        int number;

		ds >> ptr;
		ds >> name;
		ds >> output;
		ds >> flags;
        ds >> number;
        portMap[ptr] = addPort(name, output,m_node,m_nodeBase,number, flags, ptr);
        if(b){
            m_nodeBase->addPort(portMap[ptr]);
        }
        else
            m_node->addPort(portMap[ptr]);
	}
    return m_nodeBase;
}

#include <QStyleOptionGraphicsItem>

void QNEBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

    if(m_isSelected != isSelected() && isSelected())
        emit selected(m_node);
    m_isSelected = isSelected();
	if (isSelected()) {
		painter->setPen(QPen(Qt::darkYellow));
		painter->setBrush(Qt::yellow);
	} else {
        painter->setPen(QPen(QColor(194,126,0)));
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(QColor(255,166,0));
        painter->setBrush(brush);
	}

	painter->drawPath(path());
}

QNEBlock* QNEBlock::clone()
{
    QNEBlock *b = new QNEBlock(20,5,0);
    this->scene()->addItem(b);

	foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == QNEPort::Type)
		{
			QNEPort *port = (QNEPort*) port_;
            b->addPort(port->portName(), port->isOutput(),port->getNode(),port->getNodeBase(),port->number(), port->portFlags(), port->ptr());
		}
	}

	return b;
}

QVector<QNEPort*> QNEBlock::ports()
{
	QVector<QNEPort*> res;
	foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == QNEPort::Type)
			res.append((QNEPort*) port_);
	}
    return res;
}

void QNEBlock::setSize(int w, int h)
{
    width = w;
    height = h;
    QPainterPath p;
    p.addRoundedRect(-width/2, -height/2, width, height, 5, 5);
    setPath(p);
}

NodeBase *QNEBlock::getNodeBase()
{
    return m_nodeBase;
}

QVariant QNEBlock::itemChange(GraphicsItemChange change, const QVariant &value)
{

    Q_UNUSED(change);

	return value;
}

