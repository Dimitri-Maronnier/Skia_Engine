#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "src/Engine/Entities/object3dstatic.h"
#include "src/Assets/assetscollections.h"
#include "src/Engine/scene.h"


class Projectile:public Actor
{

public:
    enum BulletType{defaultBullet=0,fast=1,slow=2,fire=3,windLance=4,iceBall=5};
    Projectile();
    Projectile(int type, float uptime, glm::vec3 direction, glm::vec3 position, float orientation, float offset, float velocity);
    void setUp(Scene *scene)override;
    void update(double deltaTime);
    void createBullet(Scene*scene);

    int _type;
    glm::vec3 _position;
    float _orientation;
    unsigned int id;
    static unsigned idGen;
    bool operator==(const Projectile& p1) {
      return p1.id == id ;
    }
private:
    float _uptime;
    float _hit_box_size;
    glm::vec3 _direction;
    float _offset;
    float _velocity;
    static Object3DStatic* bullet;
    Object3DStatic* _bullet;
    Scene* scene;

    void updateDefault(double deltaTime);

};

#endif // PROJECTILE_H
