#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include "src/Engine/Entities/object3dstatic.h"
#include "src/Engine/Entities/entity.h"
#include "src/Engine/Shaders/simpleskyboxshader.h"
#include "src/Loaders/loader.h"
#include "src/Engine/Entities/light.h"
#include "Utils/rendertools.h"
#include "src/Editor/scenetree.h"
#include "Shaders/pivotshader.h"
#include <QObject>

class Scene: public QObject
{
   Q_OBJECT
public:
    /**
     * @brief Scene
     */
    Scene();



    /**
     * @brief init
     */
    void init(SceneTree *scenetree);

    /**
     * @brief addObject3DStatic
     * @param object
     */
    void addObject3DStatic(Object3DStatic* object);

    /**
     * @brief cleanUp
     */
    void cleanUp();

    /**
     * @brief render
     */
    void render();

    /**
     * @brief resize, update camera
     * @param matrix
     */
    void resize(glm::mat4 matrix);

    std::vector<Object3DStatic*> StaticObjects;
    Object3DStatic* selectedEntity;

    static CameraThird camera;//Main current Camera

signals:
    void oneEntityHaveBeenSelected(Object3DStatic* entity);
    void oneEntityHaveBeenAdded(Entity* entity);
public slots:
    void selectEntity(Entity* entity);
private:
    Light light;
    GLuint texture;
    GLuint hdr;
    GLuint m_skyboxHdr;
    GLuint m_prefilterMap;
    GLuint m_irradianceMap;
    GLuint m_brdfMap;
    SimpleSkyboxShader m_skyShader;
    PivotShader pivotShader;
    GLuint *cubeVAO;
    GLuint *cubeVBO;
    Shader shader;

    bool _entitySelected;
};


#endif // SCENE_H
