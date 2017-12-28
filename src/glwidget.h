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
#include "Engine/Shaders/simpleskyboxshader.h"
#include <QTime>
#include <QElapsedTimer>
#include "src/Game/game.h"


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
    void keyReleaseEvent(QKeyEvent* event);
    void initializeGL();
    void paintGL();
    void rezizeGL(int w,int h);
    virtual void resizeEvent(QResizeEvent * event);
    Scene* getScene();
    void setScene(Scene scene);

    static QGLContext *mainHandle;
    static bool GameRun;

public slots:
    void update();
    void gameRun();
signals:
    void quitGame();
    void drawStandAlone();
private:
    QElapsedTimer _elapsedTime;
    virtual  void dragEnterEvent(QDragEnterEvent *event);

    virtual  void dragLeaveEvent(QDragLeaveEvent *event){

    }
    virtual void dragMoveEvent ( QDragMoveEvent * event ){

    }


    virtual void dropEvent(QDropEvent *event);
    float lastTime;
    int frameCount;
    double fps;

    int width;
    int height;
    QPoint anchor;
    Loader loader;
    Mesh mesh;
    bool click;

    bool m_isInitialize;

    QTimer * time;
    QTime timer;
    bool haveBeenRezizeOnce;
    Scene scene;
    bool _zKeyPressed,_sKeyPressed,_qKeyPressed,_dKeyPressed;

    Game _game;
};

#endif // GLWIDGET_H
