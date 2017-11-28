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
    camera = CameraThird(50);
    camera.move(0,0,0,0,0,0,0);
}

GLWidget::~GLWidget(){
    destroy(time);
    shader.cleanUp();
    scene.cleanUp();
    SAFE_DELETE(cubeVAO);
    SAFE_DELETE(cubeVBO);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){

    float dx = (event->x()-anchor.x());
    float dy = (event->y()-anchor.y());
    if(click)
        camera.move(dx,dy,0,0,0,0,0);
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
    camera.move(0,0,event->delta()/120,0,0,0,0);
    //paintGL();
}


void GLWidget::keyPressEvent(QKeyEvent* event){

    int z=0,s=0,q=0,d=0;

    if(event->text() == "z"){
        z=1;
    }
    if(event->text() == "s"){
        s=1;
    }
    if(event->text() == "q"){
        q=1;
    }
    if(event->text() == "d"){
        d=1;
    }
    if(click)
        camera.move(0,0,0,z,s,q,d);
    //paintGL();

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

    camera.setProjectionMatrix(Matrix::createProjectionMatrix(camera,width,height));

    cubeVAO = new GLuint();
    cubeVBO = new GLuint();
    Utils::generateCube(cubeVAO,cubeVBO);

    hdr = Loader::loadHdr((char*)"reflexion.hdr");
    m_skyboxHdr = Utils::equirectangularToCubeMap(hdr);
    m_irradianceMap = Utils::irradianceConvolution(m_skyboxHdr);
    m_prefilterMap = Utils::prefilterCubeMap(m_skyboxHdr);
    m_brdfMap = Utils::generate2DLut();

    m_skyShader.init("simpleSyboxVertex.glsl","simpleSkyboxFragment.glsl");
    m_skyShader.start();
    m_skyShader.connectTextureUnits();
    m_skyShader.loadProjection(camera.getProjectionMatrix());
    m_skyShader.stop();

    shader.init("vs.glsl","fs.glsl");


    shader.start();
    shader.loadProjectionMatrix(camera.getProjectionMatrix());
    shader.connectTextureUnits();
    shader.stop();

    glViewport(0, 0, (GLint)width, (GLint)height);


    light.setTint(glm::vec3(1,1,1));
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
    if(m_isInitialize){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        i+=0.1;
        j+=0.1;
        w+=0.1;

        light.setPosition(glm::vec3(1000*sin(i),1000*sin(w),1000*cos(j)));
        foreach(Object3DStatic *object,scene.StaticObjects){

            foreach(Model* model,object->getModels()){

                glBindVertexArray(model->getMesh().getVaoID());
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glEnableVertexAttribArray(3);

                if(model->getMaterial() && model->getMaterial()->isShaderInit()){
                    model->getMaterial()->getShader().start();
                    model->getMaterial()->getShader().loadModelMatrix(object->getModelMatrix());
                    model->getMaterial()->getShader().loadProjectionMatrix(camera.getProjectionMatrix());
                    model->getMaterial()->getShader().loadViewMatrix(camera.getPosition(), camera.getViewMatrix());
                    model->getMaterial()->getShader().loadLightPosition(light.getPosition());
                    model->getMaterial()->getShader().loadLightTint(light.getTint());
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
                    glActiveTexture(GL_TEXTURE2);
                    glBindTexture(GL_TEXTURE_2D, m_brdfMap);
                    for(unsigned int i=0;i<model->getMaterial()->getTexturesPath().size();i++){

                        glActiveTexture(GL_TEXTURE0+i+3);
                        Texture *t = AssetsCollections::TexturesCollection.GetElementByPath(model->getMaterial()->getTexturesPath().at(i));
                        if (t)
                            glBindTexture(GL_TEXTURE_2D, t->getTextureID() );
                    }
                    glDrawElements(GL_TRIANGLES,model->getMesh().getVertexCount(),GL_UNSIGNED_INT, 0);
                    model->getMaterial()->getShader().stop();
                }else{
                    shader.start();
                    shader.loadViewMatrix(camera.getPosition(), camera.getViewMatrix());
                    shader.loadLightPosition(light.getPosition());
                    shader.loadLightTint(light.getTint());
                    glDrawElements(GL_TRIANGLES,model->getMesh().getVertexCount(),GL_UNSIGNED_INT, 0);
                    shader.stop();
                }


                glDisableVertexAttribArray(3);
                glDisableVertexAttribArray(2);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(0);
                glBindVertexArray(0);
            }
        }

        m_skyShader.start();
        m_skyShader.loadView( camera.getViewMatrix());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxHdr);
        Utils::renderCube(*cubeVAO);
        m_skyShader.stop();
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
        camera.setProjectionMatrix(Matrix::createProjectionMatrix(camera,width,height));
        shader.start();
        shader.loadProjectionMatrix(camera.getProjectionMatrix());
        shader.stop();
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
