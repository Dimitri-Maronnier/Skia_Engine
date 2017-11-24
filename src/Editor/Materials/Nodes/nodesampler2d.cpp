#include "nodesampler2d.h"
#include <QFileDialog>
#include <QDir>
#include "src/qnodeseditor/qneport.h"

NodeSampler2D::NodeSampler2D(QGraphicsScene *scene)
{
    /*m_scene = scene;
    m_b = new QNEBlock(100,50,0);

    m_scene->addItem(m_b);
    m_b->addPort("Sample2D", 0,NULL, QNEPort::NamePort);
    m_b->addPort("Texture", 0,NULL, QNEPort::TypePort);
    m_b->addOutputPort("rgb",NULL);
    m_b->addOutputPort("r",NULL);
    m_b->addOutputPort("g",NULL);
    m_b->addOutputPort("b",NULL);
    m_b->addOutputPort("a",NULL);

    QString fileName =
            QFileDialog::getOpenFileName(nullptr,
                                         QObject::tr("Open Image"),
                                         QDir::homePath(),
                                         QObject::tr("Image Files (*.png *.jpg *.bmp)"));

    QPixmap pixmap = QPixmap(fileName);
    QSize myRessortSize(75,75);
    QPixmap myNewRessort = pixmap.scaled(myRessortSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    m_item = m_scene->addPixmap(myNewRessort);

    m_item->setPos(-75/2 -5,-25);

    m_item->setParentItem((QGraphicsItem*)m_b);*/

}
