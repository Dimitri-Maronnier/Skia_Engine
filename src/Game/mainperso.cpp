#include "mainperso.h"
#include <iostream>
#include "src/Engine/Utils/smath.h"
#include <QMessageBox>



MainPerso::MainPerso()
{

}

void MainPerso::setUp(Scene *scene)
{
    e = scene->getEntity("barrel");
    this->scene = scene;
    idleStateCooldown = 0.0f;
    state = 0;
}

void MainPerso::update(double deltaTime)
{
    //handleCollision();
    if(this->idleStateCooldown>0.0f){

        idleStateCooldown -= deltaTime;
        idleStateCooldown = std::max(0.0f, idleStateCooldown);
        if(idleStateCooldown==0.0f){
            this->state=0;

        }
    }
    const float maxSpeed = 5.0;
    const float dashSpeed = 10.0;
    const float shootingSpeed = 2.0;
    if(e!=nullptr){
        e->setPosition(e->getPosition()+e->getSpeed()*(float)deltaTime);
        glm::vec2 speedXY = input._speed;
        glm::vec3 speed = glm::vec3(speedXY.x, e->getSpeed().y, speedXY.y);
        input.update();
        float orientation = SMath::toDegree(std::atan2(input._aim.y, input._aim.x)*-1)+90;

        if(!e->getChilds().empty()){
            Entity* child = e->getChilds().at(0);
            if(child!=nullptr){

                child->setRotationY(orientation);
            }
        }

        if(input.dashPressed && this->state == 0){
            this->idleStateCooldown = 0.5f;
            this->state = 2;
        }
        if(input.alternateShotPressed && this->state ==0){
            this->idleStateCooldown = 0.5f;
            this->state = 1;
            shootAlternate();
        }

        if(input.shotPressed&& this->state ==0){
            this->idleStateCooldown = 0.5f;
            this->state = 1;
            shootDefault();
        }
        if(this->state==0)
            e->setSpeed(speed*maxSpeed);
        else if (this->state == 1)
            e->setSpeed(speed*shootingSpeed);
        else
            e->setSpeed(speed*dashSpeed);
        updateShotFired(deltaTime);
    }
}

void MainPerso::handleCollision(){
    auto collisions = e->lastFrameCollision;
    foreach(auto c, collisions){
        if(c.tag >= 2 && c.tag <= 3){
            loseHP();
        }
    }
}

void MainPerso::loseHP(){
    std::cout << "Hp has been lost" << std::endl;
}

void MainPerso::updateShotFired(double deltaTime){
    int i=0;
    while(i < this->shotFired.size()){

        if(shotFired.at(i)._type != -1){
            shotFired.at(i).update(deltaTime);
            i++;
        }
        else{
            auto it = find(shotFired.begin(),shotFired.end(),shotFired.at(i));

            if(it != shotFired.end())
                shotFired.erase(it);
        }

    }
}

void MainPerso::shootAlternate(){
    glm::vec3 direction = glm::vec3(input._aim.x, 0, input._aim.y);
    Projectile p = Projectile::Projectile(0,
                                          7.5f,
                                          direction,
                                          e->getPosition()+direction*2.0f,
                                          SMath::toDegree(std::atan2(input._aim.y, input._aim.x)*-1),
                                          3.0,
                                          3.0);
    p.createBullet(scene);
    this->shotFired.push_back(p);
}

void MainPerso::shootDefault(){}


Entity* MainPerso::getEntity()
{
    return e;
}
