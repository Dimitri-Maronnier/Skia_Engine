#include "glwidget.h"
#include <iostream>
#include <stdlib.h>
#include <gl/freeglut.h>
#include <QMimeData>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "Editor/ctreewidgetitem.h"
#include "Editor/Utils/foldergestion.h"
#include "Editor/Utils/projectinfo.h"
#include "Engine/Models/model.h"
#include "Loaders/loader.h"
#include <QGLContext>
#include <src/mainwindow.h>

QGLContext *GLWidget::mainHandle;

GLWidget::GLWidget( QWidget *parent) : QGLWidget(parent){

    click = false;
    haveBeenRezizeOnce = false;
    time = new QTimer;
    connect(time,SIGNAL(timeout()),this,SLOT(update()));
    time->start(16);
    timer.start();
    this->setContext(MainWindow::contxt);
    mainHandle = this->context();

    this->makeCurrent();
    m_isInitialize = false;

    _zKeyPressed=false;_sKeyPressed=false;_qKeyPressed=false;_dKeyPressed=false;
}

GLWidget::~GLWidget(){
    destroy(time);
    scene.cleanUp();

}

void GLWidget::mouseMoveEvent(QMouseEvent *event){

    float dx = (event->x()-anchor.x());
    float dy = (event->y()-anchor.y());
    if(click)
        Scene::camera.move(dx,dy,0,0,0,0,0);
    anchor = event->pos();

}

void GLWidget::mousePressEvent(QMouseEvent * event){
    if (event->buttons() & Qt::LeftButton) {
        click = true;
    } else if (event->buttons() & Qt::RightButton) {

    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        click = false;
    }
}

void GLWidget::wheelEvent ( QWheelEvent * event )
{
    Scene::camera.move(0,0,event->delta()/120,0,0,0,0);
}


void GLWidget::keyPressEvent(QKeyEvent* event){

    if(event->text() == "z"){
        _zKeyPressed=true;
    }
    if(event->text() == "s"){
        _sKeyPressed=true;
    }
    if(event->text() == "q"){
        _qKeyPressed=true;
    }
    if(event->text() == "d"){
        _dKeyPressed=true;
    }

}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->text() == "z"){
        _zKeyPressed=false;
    }
    if(event->text() == "s"){
        _sKeyPressed=false;
    }
    if(event->text() == "q"){
        _qKeyPressed=false;
    }
    if(event->text() == "d"){
        _dKeyPressed=false;
    }

}

void GLWidget::initializeGL(){

    GLenum err = glewInit();

    width = this->size().width();
    height = this->size().height();

    if(err != GLEW_OK)
    {
       std::cout << "glewInit() error: " << glewGetErrorString(err) << std::endl;
    }
    else
    {
       std::cout << "glew initialized" << std::endl;
    }

    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);



    Scene::camera.setProjectionMatrix(Matrix::createProjectionMatrix(Scene::camera,width,height));
    scene.init();
    glViewport(0, 0, (GLint)width, (GLint)height);

    haveBeenRezizeOnce = true;
    m_isInitialize = true;
}


void GLWidget::update()
{
    paintGL();
}

void GLWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QTreeWidget *tree = dynamic_cast<QTreeWidget *>(event->source());

        QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
        QDataStream stream(&itemData, QIODevice::ReadOnly);

        int r, c;
        QMap<int, QVariant> v;
        stream >> r >> c >> v;

        CTreeWidgetItem *item = (CTreeWidgetItem *)tree->currentItem();

        if( item && !item->isDirectory() && item->getExt().compare("sobj") == 0)
        {
            std::string path = FolderGestion::rootProjectsFolderPath;
            path += "\\";
            path += ProjectInfo::name;
            CTreeWidgetItem *upperParent = item;
            std::vector<std::string> reversePath;
            reversePath.push_back(item->text(0).toStdString());
            while(upperParent->parent()){

               upperParent = (CTreeWidgetItem *)upperParent->parent();
               reversePath.push_back(upperParent->text(0).toStdString());
            }
            for (int i=reversePath.size()-1;i>=0;i--) {
               path += "\\";
               path += reversePath.at(i);

            }
            path += ".sobj";


            unsigned int handle = AssetsCollections::Object3DStaticCollection.Add(item->text(0).toStdString(),path);
            AssetsCollections::HandlesObject3DStatic.push_back(handle);
            scene.addObject3DStatic(AssetsCollections::Object3DStaticCollection.GetElement(handle));

        }
    }
}

void GLWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void GLWidget::paintGL(){
    this->makeCurrent();
    if(click)
        Scene::camera.move(0,0,0,_zKeyPressed,_sKeyPressed,_qKeyPressed,_dKeyPressed);
    if(m_isInitialize){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


        scene.render();


        swapBuffers();
        GLenum err;
        if ((err = glGetError()) != GL_NO_ERROR) {
           std::cerr << "OpenGL error: " << err << " " << gluErrorString(err)<< std::endl;
        }
    }

}

void GLWidget::rezizeGL(int w, int h){
    width=w;
    height = h;
    if(haveBeenRezizeOnce){

        glViewport(0, 0, (GLint)w, (GLint)h);

        scene.resize(Matrix::createProjectionMatrix(Scene::camera,width,height));

    }
}

void GLWidget::resizeEvent(QResizeEvent * event){
    rezizeGL(event->size().width(),event->size().height());
}

Scene* GLWidget::getScene()
{
    return &scene;
}

void GLWidget::setScene(Scene scene)
{
    //this->scene = scene;
}
