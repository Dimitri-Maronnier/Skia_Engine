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
#include "Entities/camerathird.h"
#include "src/Engine/Entities/collision.h"

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

    Object3DStatic* addObject(Object3DStatic* entity);
    void destroyObject(Object3DStatic* entity);
    void cleanGameAsset();
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

    Entity* getEntity(QString label);

    std::vector<Entity*> StaticObjects;

    Entity* selectedEntity;

    static CameraThird camera;//Main current Camera

    Light light;
signals:
    void oneEntityHaveBeenSelected(Object3DStatic* entity);
    void oneEntityHaveBeenAdded(Entity* entity);
public slots:
    void selectEntity(Entity* entity);
    void setScene(const QString& map);

private:
    std::vector<Entity*> Cache;
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
    SceneTree* _sceneTree;

    bool _entitySelected;
};


#endif // SCENE_H
