
#include "glmaterialpreview.h"
#include "src/Engine/Models/model.h"
#include "src/Editor/Materials/materialeditorwindow.h"
#include "src/Editor/Materials/Nodes/nodebase.h"
#include <QDir>
#include <QFileDialog>
#include <iostream>
#include "src/Assets/assetscollections.h"
#include <QOpenGLContext>
#include "src/glwidget.h"
#include "src/mainwindow.h"
#include "src/define.h"
#include "src/Editor/Utils/foldergestion.h"

QGLContext * GLMaterialPreview::materialHandle;

GLMaterialPreview::GLMaterialPreview( QWidget *parent) : QGLWidget(parent){

    click = false;
    haveBeenRezizeOnce = false;
    time = new QTimer;
    connect(time,SIGNAL(timeout()),this,SLOT(update()));
    time->start(16);
    timer.start();
    m_currentShader = 0;
    camera = CameraThird(50);
    camera.move(0,0,0,0,0,0,0);
    m_lightAutoRotate = false;
    m_isInitialize = false;
    //this->setContext(MainWindow::contxt,GLWidget::mainHandle);
    this->context()->create(GLWidget::mainHandle);
    materialHandle = this->context();
    m_cleanFirstHdrUse = true;
    m_callSphere = false;m_callCube = false;m_callPlane = false;m_callCylinder = false;m_callCustom = false;
}

GLMaterialPreview::~GLMaterialPreview(){

    destroy(time);
    m_shader.at(m_currentShader).cleanUp();
    m_skyShader.cleanUp();
    m_deferredShader.cleanUp();
    this->makeCurrent();

    SAFE_DELETE(cubeVAO);
    SAFE_DELETE(cubeVBO);

    SAFE_DELETE(quadVAO);
    SAFE_DELETE(quadVBO);

    GLuint textures[9];
    textures[0] = hdr;
    textures[1] = m_skyboxHdr;
    textures[2] = m_irradianceMap;
    textures[3] = m_prefilterMap;
    textures[4] = m_brdfMap;
    textures[5] = *m_position;
    textures[6] = *m_normal;
    textures[7] = *m_albedoRoughness;
    textures[8] = *m_metalOcclusion;

    glDeleteTextures(9,textures);

    GLuint frameBuffers[1];
    frameBuffers[0] = *m_gBuffer;

    glDeleteFramebuffers(1,frameBuffers);
    SAFE_DELETE(m_gBuffer);
    SAFE_DELETE(m_position);
    SAFE_DELETE(m_normal);
    SAFE_DELETE(m_albedoRoughness);
    SAFE_DELETE(m_metalOcclusion);

}

void GLMaterialPreview::mouseDoubleClickEvent(QMouseEvent *event)
{
    this->makeCurrent();
    if ( event->button() == Qt::LeftButton )
    {
        QWidget* win = (QWidget*)this->parent();
        if(width == m_size.width() && height == m_size.height()){

            this->setMinimumSize(win->geometry().width()/1.3,win->geometry().height()/1.3);

        }else{

            this->setMinimumSize(m_size);
        }
    }

}

void GLMaterialPreview::mouseMoveEvent(QMouseEvent *event){

    float dx = (event->x()-anchor.x());
    float dy = (event->y()-anchor.y());
    if(click)
        camera.move(dx,dy,0,0,0,0,0);
    anchor = event->pos();
    this->makeCurrent();
}

void GLMaterialPreview::mousePressEvent(QMouseEvent * event){
    if (event->buttons() & Qt::LeftButton) {
        click = true;
    } else if (event->buttons() & Qt::RightButton) {

    }
}

void GLMaterialPreview::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        click = false;
    }
}

void GLMaterialPreview::wheelEvent ( QWheelEvent * event )
{
    camera.move(0,0,event->delta()/120,0,0,0,0);
    this->makeCurrent();
}


void GLMaterialPreview::keyPressEvent(QKeyEvent* event){

}

void GLMaterialPreview::initializeGL(){

    this->makeCurrent();


    width = this->size().width();
    height = this->size().height();

    cubeVAO = new GLuint();
    cubeVBO = new GLuint();
    RenderTools::generateCube(cubeVAO,cubeVBO);

    quadVAO = new GLuint;
    quadVBO = new GLuint;
    RenderTools::generateQuad(quadVAO,quadVBO);

    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);

    camera.setProjectionMatrix(Matrix::createProjectionMatrix(camera,width,height));

    hdr = Loader::loadHdr((char*)"reflexion.hdr");
    m_skyboxHdr = RenderTools::equirectangularToCubeMap(hdr);
    m_irradianceMap = RenderTools::irradianceConvolution(m_skyboxHdr);
    m_prefilterMap = RenderTools::prefilterCubeMap(m_skyboxHdr);
    m_brdfMap = RenderTools::generate2DLut();


    m_skyShader.init("simpleSyboxVertex.glsl","simpleSkyboxFragment.glsl");
    m_skyShader.start();
    m_skyShader.connectTextureUnits();
    m_skyShader.loadProjection(camera.getProjectionMatrix());
    m_skyShader.stop();

    //m_deferredShader.init("deferredShadingVertex.glsl","deferredShadingFragment.glsl");

    /*get Node Base*/
    MaterialEditorWindow* win = (MaterialEditorWindow*)this->parent()->parent();
    m_node = (NodeBase*)win->getMaterialView()->getNodeBase();


    connect(m_node,SIGNAL(onFragmentSourceChanged(QString)),this,SLOT(compilShader(QString)));
    m_shader.push_back(Shader());
    m_shader.at(m_currentShader).initWithSource(m_node->getVertexSource(),m_node->getFragmentSource());



    m_shader.at(m_currentShader).start();
    m_shader.at(m_currentShader).loadProjectionMatrix(camera.getProjectionMatrix());

    m_shader.at(m_currentShader).stop();

    glViewport(0, 0, (GLint)width, (GLint)height);

    light.setPosition(glm::vec3(1500,1500,0));
    light.setColor(glm::vec3(1,1,1));
    haveBeenRezizeOnce = true;
    std::string nameFile = "sphere_previewMat.sobj";
    std::string path = QDir::currentPath().toStdString() + "/mesh/sphere_previewMat.sobj";

    unsigned int handle = AssetsCollections::Object3DStaticCollection.Add(nameFile,path);
    AssetsCollections::HandlesObject3DStatic.push_back(handle);


    m_newObject = AssetsCollections::Object3DStaticCollection.GetElement(handle);
    m_size.setWidth(width);
    m_size.setHeight(height);

    m_isInitialize = true;

    //Setup GBuffer
    m_gBuffer = new GLuint;
    m_position = new GLuint;
    m_normal = new GLuint;
    m_albedoRoughness = new GLuint;
    m_metalOcclusion = new GLuint;

    //RenderTools::setupDeferredShading(this->width,this->height,m_gBuffer,m_position,m_normal,m_albedoRoughness,m_metalOcclusion);

}


void GLMaterialPreview::update()
{
    paintGL();
}

void GLMaterialPreview::compilShader(QString)
{

    m_shader.at(m_currentShader).cleanUp();

    if(m_currentShader==2)
        m_shader.erase(m_shader.begin());
    m_currentShader =( m_currentShader + 1)%3;
    m_shader.push_back(Shader());

    /*if(m_texturePath.empty()){
        for(int i=0;i<m_node->getTexturesPath().size();i++){
            m_texture.push_back(AssetsCollections::TexturesCollection.GetElementByPath(m_node->getTexturesPath().at(i).toStdString())->getTextureID());
            m_texturePath.insert(m_node->getTexturesPath().at(i));
        }
    }
    else{
        /*Clean*/
        /*std::vector<QString> paths;
        std::vector<QString> pathsCopy = m_node->getTexturesPath();
        foreach(QString path,m_texturePath){
            if(std::find(pathsCopy.begin(),pathsCopy.end(), path) == pathsCopy.end() )
                paths.push_back(path);
        }
        foreach(QString path,paths){
            int i=0;
            foreach(QString path2,m_texturePath){
                if(path2.compare(path) == 0){
                    m_texturePath.erase(path);
                    m_texture.erase(m_texture.begin() + i);
                }
                i++;
            }

        }
        foreach(QString path,m_node->getTexturesPath()){
            if(m_texturePath.find(path) == m_texturePath.end()) {
                m_texture.push_back( AssetsCollections::TexturesCollection.GetElementByPath(path.toStdString())->getTextureID());
                m_texturePath.insert(path);
            }
        }

    }*/

    m_shader.at(m_currentShader).initWithSource(m_node->getVertexSource(),m_node->getFragmentSource());


    m_shader.at(m_currentShader).start();
    m_shader.at(m_currentShader).loadProjectionMatrix(camera.getProjectionMatrix());

    m_shader.at(m_currentShader).stop();


}

void GLMaterialPreview::switchToSphere()
{
    time->blockSignals(true);

    std::string nameFile = "sphere_previewMat.sobj";
    std::string path = QDir::currentPath().toStdString() + "/mesh/sphere_previewMat.sobj";

    unsigned int handle = AssetsCollections::Object3DStaticCollection.Add(nameFile,path);
    if(handle!=-1){
        AssetsCollections::HandlesObject3DStatic.push_back(handle);
        m_newObject = AssetsCollections::Object3DStaticCollection.GetElement(handle);
    }
    time->blockSignals(false);
}

void GLMaterialPreview::switchToCube()
{
    time->blockSignals(true);


    std::string nameFile = "cube_previewMat.sobj";
    std::string path = QDir::currentPath().toStdString() + "/mesh/cube_previewMat.sobj";

    unsigned int handle = AssetsCollections::Object3DStaticCollection.Add(nameFile,path);
    if(handle!=-1){
        AssetsCollections::HandlesObject3DStatic.push_back(handle);
        m_newObject = AssetsCollections::Object3DStaticCollection.GetElement(handle);
    }
    time->blockSignals(false);
}

void GLMaterialPreview::switchToPlane()
{
    time->blockSignals(true);

    std::string nameFile = "plane_previewMat.sobj";
    std::string path = QDir::currentPath().toStdString() + "/mesh/plane_previewMat.sobj";

    unsigned int handle = AssetsCollections::Object3DStaticCollection.Add(nameFile,path);
    if(handle!=-1){
        AssetsCollections::HandlesObject3DStatic.push_back(handle);
        m_newObject = AssetsCollections::Object3DStaticCollection.GetElement(handle);
    }
    time->blockSignals(false);
}

void GLMaterialPreview::switchToCylinder()
{
    time->blockSignals(true);

    std::string nameFile = "cylinder_previewMat.sobj";
    std::string path = QDir::currentPath().toStdString() + "/mesh/cylinder_previewMat.sobj";

    unsigned int handle = AssetsCollections::Object3DStaticCollection.Add(nameFile,path);
    AssetsCollections::HandlesObject3DStatic.push_back(handle);

    if(handle!=-1){
        AssetsCollections::HandlesObject3DStatic.push_back(handle);
        m_newObject = AssetsCollections::Object3DStaticCollection.GetElement(handle);
    }
    time->blockSignals(false);
}

void GLMaterialPreview::switchToCustom()
{

    time->blockSignals(true);
    QString filter = "Wavefront format (*.sobj)";

    std::string path = QFileDialog::getOpenFileName(this, "Select a file...",QString(FolderGestion::rootProjectsFolderPath), filter).toStdString();
    std::vector<std::string> splitPath = Utils::split(path,'/');
    std::string nameFile = splitPath.at(splitPath.size()-1);
    glFinish();
    if(path.size()>0){
        unsigned int handle = AssetsCollections::Object3DStaticCollection.Add(nameFile,path);
        AssetsCollections::HandlesObject3DStatic.push_back(handle);
        std::cout << path << " " << nameFile << " " << handle << std::endl;
        if(handle!=-1)
            m_newObject = AssetsCollections::Object3DStaticCollection.GetElement(handle);
    }

    time->blockSignals(false);
}

void GLMaterialPreview::lightAutoRotate(bool check)
{
    m_lightAutoRotate = check;
}

void GLMaterialPreview::setLightColor(glm::vec3 color)
{
    light.setColor(color);
}

void GLMaterialPreview::changeHDRI(Texture* texture)
{

    this->makeCurrent();
    if(m_cleanFirstHdrUse){
        m_cleanFirstHdrUse = false;
        GLuint textures[5];
        textures[0] = hdr;
        textures[1] = m_skyboxHdr;
        textures[2] = m_irradianceMap;
        textures[3] = m_prefilterMap;
        textures[4] = m_brdfMap;

        glDeleteTextures(5,textures);
    }

    hdr = texture->getTextureID();
    m_skyboxHdr = RenderTools::equirectangularToCubeMap(hdr);
    m_irradianceMap = RenderTools::irradianceConvolution(m_skyboxHdr);
    m_prefilterMap = RenderTools::prefilterCubeMap(m_skyboxHdr);
    m_brdfMap = RenderTools::generate2DLut();



}

void GLMaterialPreview::paintGL(){
    this->makeCurrent();
    if(m_isInitialize){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        frameCount++;
        i+=0.1;
        j+=0.1;
        w+=0.1;

        if(m_lightAutoRotate)
            light.setPosition(glm::vec3(1000*cos(i),0,1000*sin(j)));

        Object3DStatic *object = m_newObject;
        if(object){
            m_shader.at(m_currentShader).start();
            m_shader.at(m_currentShader).loadModelMatrix(object->getModelMatrix());
            m_shader.at(m_currentShader).stop();

            foreach(Model* model,object->getModels()){
                if(model){
                    //glBindFramebuffer(GL_FRAMEBUFFER, *m_gBuffer);
                    glBindVertexArray(model->getMesh().getVaoID());

                    glEnableVertexAttribArray(0);
                    glEnableVertexAttribArray(1);
                    glEnableVertexAttribArray(2);
                    glEnableVertexAttribArray(3);
                    m_shader.at(m_currentShader).start();
                    m_shader.at(m_currentShader).loadViewMatrix(camera.getPosition(), camera.getViewMatrix());
                    m_shader.at(m_currentShader).loadLightPosition(light.getPosition());
                    m_shader.at(m_currentShader).loadLightColor(light.getColor());
                    m_shader.at(m_currentShader).loadTime(frameCount/1000.0);

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
                    glActiveTexture(GL_TEXTURE2);
                    glBindTexture(GL_TEXTURE_2D, m_brdfMap);

                    for(int i=0;i<m_node->getTexturesPath().size();i++){

                        glActiveTexture(GL_TEXTURE0+i+3);
                        Texture *t = AssetsCollections::TexturesCollection.GetElementByPath(m_node->getTexturesPath().at(i).toStdString());
                        if (t)
                            glBindTexture(GL_TEXTURE_2D, t->getTextureID() );
                    }
                    glFinish();
                    glDrawElements(GL_TRIANGLES,model->getMesh().getVertexCount(),GL_UNSIGNED_INT, 0);

                    m_shader.at(m_currentShader).stop();

                    glDisableVertexAttribArray(3);
                    glDisableVertexAttribArray(2);
                    glDisableVertexAttribArray(1);
                    glDisableVertexAttribArray(0);
                    glBindVertexArray(0);
                    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
                }

            }

        }
        m_skyShader.start();
        m_skyShader.loadView( camera.getViewMatrix());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxHdr);
        RenderTools::renderCube(*cubeVAO);
        m_skyShader.stop();

        /*m_deferredShader.start();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_brdfMap);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, *m_position);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, *m_normal);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, *m_albedoRoughness);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, *m_metalOcclusion);

        m_deferredShader.loadViewPos(camera.getPosition());
        m_deferredShader.loadLight(light);


        Utils::renderQuad(*quadVAO);
        m_deferredShader.stop();*/

        swapBuffers();
        GLenum err;
        if ((err = glGetError()) != GL_NO_ERROR) {
           std::cerr << "OpenGL error: " << err << " " << gluErrorString(err)<< std::endl;
        }
    }
    if(m_callSphere){
        m_callSphere = false;
        switchToSphere();
    }
    else if(m_callCube){
        m_callCube = false;
        switchToCube();
    }
    else if(m_callCylinder){
        m_callCylinder = false;
        switchToCylinder();
    }
    else if(m_callPlane){
        m_callPlane = false;
        switchToPlane();
    }
    else if(m_callCustom){
        m_callCustom = false;
        switchToCustom();
    }
}

void GLMaterialPreview::rezizeGL(int w, int h){
    width=w;
    height = h;
    if(haveBeenRezizeOnce){

        glViewport(0, 0, (GLint)w, (GLint)h);
        camera.setProjectionMatrix(Matrix::createProjectionMatrix(camera,width,height));
        m_shader.at(m_currentShader).start();
        m_shader.at(m_currentShader).loadProjectionMatrix(camera.getProjectionMatrix());
        m_shader.at(m_currentShader).stop();
        m_skyShader.start();
        m_skyShader.loadProjection(camera.getProjectionMatrix());
        m_skyShader.stop();
    }
}

void GLMaterialPreview::resizeEvent(QResizeEvent * event){
    rezizeGL(event->size().width(),event->size().height());
}

Scene* GLMaterialPreview::getScene()
{
    return &scene;
}

void GLMaterialPreview::setScene(Scene scene)
{
    //this->scene = scene;
}

void GLMaterialPreview::setNodeBase(NodeBase *node)
{
    time->blockSignals(false);
    m_node = node;
    if(m_node){

        connect(m_node,SIGNAL(onFragmentSourceChanged(QString)),this,SLOT(compilShader(QString)));
        m_shader.push_back(Shader());
        m_shader.at(m_currentShader).initWithSource(m_node->getVertexSource(),m_node->getFragmentSource());

        m_shader.at(m_currentShader).start();
        m_shader.at(m_currentShader).loadProjectionMatrix(camera.getProjectionMatrix());

        m_shader.at(m_currentShader).stop();
    }
}

void GLMaterialPreview::close()
{
    time->blockSignals(true);

}
