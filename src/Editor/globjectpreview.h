#ifndef GLOBJECTPREVIEW_H
#define GLOBJECTPREVIEW_H
#include <GL/glew.h>
#include <QGLWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDropEvent>
#include "src/Engine/Entities/camerathird.h"
#include "src/Engine/Entities/camerafirst.h"
#include "src/Engine/Shaders/shader.h"
#include "src/Loaders/loader.h"
#include "src/Engine/Models/mesh.h"
#include "src/Engine/Entities/light.h"
#include "src/Engine/scene.h"
#include "src/Engine/Shaders/simpleskyboxshader.h"
#include "src/Engine/Utils/rendertools.h"
#include <QTime>



class GLObjectPreview : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLObjectPreview( QWidget *parent = 0);
    ~GLObjectPreview();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent ( QWheelEvent * event );
    void keyPressEvent(QKeyEvent* event);
    void initializeGL();
    void paintGL();
    void rezizeGL(int w,int h);
    virtual void resizeEvent(QResizeEvent * event);
    Scene* getScene();
    void setScene(Scene scene);
    int setObjectToRender(std::string fileName,std::string path);
    void close();

public slots:
    void update();

private:
    CameraThird camera;
    Shader shader;
    int width;
    int height;
    QPoint anchor;
    bool click;
    Light light;
    GLuint texture;
    QTimer * time;
    QTime timer;
    bool haveBeenRezizeOnce;
    Scene scene;
    bool m_isInitialize;
    GLuint hdr;
    GLuint m_skyboxHdr;
    GLuint m_prefilterMap;
    GLuint m_irradianceMap;
    GLuint m_brdfMap;
    SimpleSkyboxShader m_skyShader;

    Object3DStatic* m_object;

    GLuint *cubeVAO;
    GLuint *cubeVBO;


};

#endif // GLOBJECTPREVIEW_H
