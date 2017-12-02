#include "globjectpreview.h"
#include "src/mainwindow.h"
#include "src/glwidget.h"
#include "src/Editor/Materials/glmaterialpreview.h"
#include "src/Assets/assetscollections.h"

GLObjectPreview::GLObjectPreview( QWidget *parent) : QGLWidget(parent){
    click = false;
    haveBeenRezizeOnce = false;
    camera = CameraThird(50);
    camera.move(0,0,0,0,0,0,0);

    time = new QTimer;
    connect(time,SIGNAL(timeout()),this,SLOT(update()));
    time->start(16);
    timer.start();
    //this->setContext(MainWindow::contxt,GLWidget::mainHandle);
    //this->setContext(GLWidget::mainHandle,GLMaterialPreview::materialHandle);
    this->context()->create(GLWidget::mainHandle);
    this->makeCurrent();
    m_isInitialize = false;
}

GLObjectPreview::~GLObjectPreview(){
    destroy(time);
    shader.cleanUp();
    SAFE_DELETE(cubeVAO);
    SAFE_DELETE(cubeVBO);
}

void GLObjectPreview::mouseMoveEvent(QMouseEvent *event){

    float dx = (event->x()-anchor.x());
    float dy = (event->y()-anchor.y());
    if(click)
        camera.move(dx,dy,0,0,0,0,0);
    anchor = event->pos();
}

void GLObjectPreview::mousePressEvent(QMouseEvent * event){
    if (event->buttons() & Qt::LeftButton) {
        click = true;
    } else if (event->buttons() & Qt::RightButton) {

    }
}

void GLObjectPreview::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        click = false;
    }
}

void GLObjectPreview::wheelEvent ( QWheelEvent * event )
{
    camera.move(0,0,event->delta()/120,0,0,0,0);
}


void GLObjectPreview::keyPressEvent(QKeyEvent* event){
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

}

void GLObjectPreview::initializeGL(){

    this->makeCurrent();

    width = this->size().width();
    height = this->size().height();

    cubeVAO = new GLuint(1);
    cubeVBO = new GLuint(1);
    Utils::generateCube(cubeVAO,cubeVBO);

    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);
    camera.setProjectionMatrix(glm::perspective(45.0f,(float)width/(float)height,1.0f,1000.0f));

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
    shader.stop();

    glViewport(0, 0, (GLint)width, (GLint)height);

    light.setColor(glm::vec3(1,1,1));
    light.setPosition(glm::vec3(-200,200,0));

    m_isInitialize = true;
}


void GLObjectPreview::update()
{
    paintGL();
}


void GLObjectPreview::paintGL(){
    this->makeCurrent();
    if(m_isInitialize){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        GLenum err;
        if ((err = glGetError()) != GL_NO_ERROR) {
           std::cerr << "OpenGL error: " << err << " " << gluErrorString(err)<< std::endl;
        }


        shader.start();
        shader.loadModelMatrix(m_object->getModelMatrix());
        shader.stop();

        foreach(Model* model,m_object->getModels()){
            glBindVertexArray(model->getMesh().getVaoID());
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);

            if(model->getMaterial() && model->getMaterial()->isShaderInit()){
                model->getMaterial()->getShader().start();
                model->getMaterial()->getShader().loadModelMatrix(m_object->getModelMatrix());
                model->getMaterial()->getShader().loadProjectionMatrix(camera.getProjectionMatrix());
                model->getMaterial()->getShader().loadViewMatrix(camera.getPosition(), camera.getViewMatrix());
                model->getMaterial()->getShader().loadLightPosition(light.getPosition());
                model->getMaterial()->getShader().loadLightColor(light.getColor());
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, m_brdfMap);
                for(int i=0;i<model->getMaterial()->getTexturesPath().size();i++){

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
                shader.loadLightColor(light.getColor());
                glDrawElements(GL_TRIANGLES,model->getMesh().getVertexCount(),GL_UNSIGNED_INT, 0);
                shader.stop();
            }


            glDisableVertexAttribArray(3);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        }

        m_skyShader.start();
        m_skyShader.loadView( camera.getViewMatrix());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxHdr);
        Utils::renderCube(*cubeVAO);
        m_skyShader.stop();
        swapBuffers();

    }
}

void GLObjectPreview::rezizeGL(int w, int h){
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

void GLObjectPreview::resizeEvent(QResizeEvent * event){
    rezizeGL(event->size().width(),event->size().height());
}

Scene* GLObjectPreview::getScene()
{
    return &scene;
}

void GLObjectPreview::setScene(Scene scene)
{
    //this->scene = scene;
}

int GLObjectPreview::setObjectToRender(std::string fileName, std::string path)
{
    time->blockSignals(false);

    unsigned int handle = AssetsCollections::Object3DStaticCollection.Add(fileName,path);
    AssetsCollections::HandlesObject3DStatic.push_back(handle);
    m_object = AssetsCollections::Object3DStaticCollection.GetElement(handle);
    return handle;

}

void GLObjectPreview::close()
{
    //time->blockSignals(true);
}
