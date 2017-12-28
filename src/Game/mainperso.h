#ifndef MAINPERSO_H
#define MAINPERSO_H
#include "src/Engine/Entities/actor.h"
#include "src/Engine/scene.h"
#include "src/Engine/Controller/inputhandler.h"
#include "src/Engine/Controller/inputabstract.h"
#include "src/Assets/assetscollections.h"
#include "src/Engine/Entities/object3dstatic.h"
#include "src/Game/projectile.h"

class MainPerso : public Actor
{
public:
    MainPerso();
    void setUp(Scene*scene)override;
    void update(double deltaTime)override;
    Entity* getEntity();

    int hitPoint;
private:
    Entity* e;
    float idleStateCooldown;
    float shootCooldown;
    float dashCooldown;
    float gunOrientation;
    std::vector<Projectile> shotFired;
    int state; //0 is idle, 1 is shooting, 2 is dashing
    std::vector<int> readyBullet;
    std::vector<int> cooldDownBullet;
    int bulletPartCount;
    InputAbstract input;
    Object3DStatic* bullet;

    void shootAlternate();
    void shootDefault();
    void handleCollision();
    void loseHP();

    void updateShotFired(double deltaTime);
    Scene* scene;

};

#endif // MAINPERSO_H
