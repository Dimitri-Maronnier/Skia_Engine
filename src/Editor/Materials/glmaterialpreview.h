#ifndef GLPREVIEW_H
#define GLPREVIEW_H
#include <GL/glew.h>
#include <QGLWidget>
#include "src/Engine/scene.h"
#include "src/Engine/Shaders/shader.h"
#include "src/Engine/Entities/camerathird.h"
#include "src/Editor/Materials/Nodes/nodebase.h"
#include "src/Engine/Shaders/simpleskyboxshader.h"
#include "src/Engine/Shaders/PBR/deferredshader.h"
#include "src/Engine/Utils/rendertools.h"
#include "src/Engine/Entities/light.h"
#include "src/Loaders/loader.h"
#include <QTime>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <set>

class GLMaterialPreview: public QGLWidget
{
    Q_OBJECT
public:
    explicit GLMaterialPreview( QWidget *parent = 0);
    ~GLMaterialPreview();
    void mouseDoubleClickEvent(QMouseEvent *event);
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
    void setNodeBase(NodeBase *node);
    void close();
    void switchToSphere();
    void switchToCube();
    void switchToPlane();
    void switchToCylinder();
    void switchToCustom();

    static QGLContext *materialHandle;

public slots:
    void update();
    void compilShader(QString);

    void switchToSphereSlot(){m_callSphere = true;}
    void switchToCubeSlot(){m_callCube = true;}
    void switchToPlaneSlot(){m_callPlane = true;}
    void switchToCylinderSlot(){m_callCylinder = true;}
    void switchToCustomSlot(){m_callCustom = true;}

    void lightAutoRotate(bool check);
    void setLightColor(glm::vec3 color);
    void changeHDRI(Texture* texture);

private:
    float i;
    float j;
    float w;
    bool m_lightAutoRotate;
    float lastTime;
    int frameCount;
    double fps;
    CameraThird camera;
    std::vector<Shader> m_shader;
    int m_currentShader;
    int width;
    int height;
    QPoint anchor;
    Mesh mesh;
    bool click;
    Light light;
    GLuint hdr;
    GLuint m_skyboxHdr;
    GLuint m_prefilterMap;
    GLuint m_irradianceMap;
    GLuint m_brdfMap;
    QTimer * time;
    QTime timer;
    bool haveBeenRezizeOnce;
    Scene scene;
    NodeBase *m_node;
    std::vector<GLuint> m_texture;
    std::set<QString> m_texturePath;
    QSize m_size;
    SimpleSkyboxShader m_skyShader;
    DeferredShader m_deferredShader;
    bool m_isInitialize;
    bool m_cleanFirstHdrUse;
    Object3DStatic * m_newObject;
    bool m_callSphere,m_callCube,m_callPlane,m_callCylinder,m_callCustom;

    GLuint *cubeVAO;
    GLuint *cubeVBO;
    GLuint *quadVAO;
    GLuint *quadVBO;

    GLuint *m_gBuffer;
    GLuint *m_position,*m_normal,*m_albedoRoughness,*m_metalOcclusion;

};

#endif // GLPREVIEW_H
