#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDropEvent>
#include "Engine/Entities/camerathird.h"
#include "Engine/Shaders/shader.h"
#include "Loaders/loader.h"
#include "Engine/Models/mesh.h"
#include "Engine/Entities/light.h"
#include "Engine/scene.h"
#include <QTime>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget( QWidget *parent = 0);
    ~GLWidget();
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

    static QGLContext *mainHandle;

public slots:
    void update();

private:

    virtual  void dragEnterEvent(QDragEnterEvent *event);

    virtual  void dragLeaveEvent(QDragLeaveEvent *event){

    }
    virtual void dragMoveEvent ( QDragMoveEvent * event ){

    }

    virtual void dropEvent(QDropEvent *event);
    float i;
    float j;
    float w;
    float lastTime;
    int frameCount;
    double fps;
    CameraThird camera;
    Shader shader;
    int width;
    int height;
    QPoint anchor;
    Loader loader;
    Mesh mesh;
    bool click;
    Light light;
    GLuint texture;
    QTimer * time;
    QTime timer;
    bool haveBeenRezizeOnce;
    Scene scene;
};

#endif // GLWIDGET_H
