#include "projectile.h"


unsigned int Projectile::idGen = 0;
Object3DStatic* Projectile::bullet;

Projectile::Projectile()
{

}

Projectile::Projectile( int type, float uptime, glm::vec3 direction, glm::vec3 position, float orientation, float offset, float velocity)
{
    id = idGen++;
    _type = type;
    _uptime = uptime;
    _direction = direction;
    _position = position;
    _orientation = orientation;
    _offset = offset;
    _velocity = velocity;
}

void Projectile::setUp(Scene *scene)
{
    static bool once = false;
    if(!once){
        std::string path = ":/Mesh/cube_previewMat.sobj";
        unsigned int handle = AssetsCollections::Object3DStaticCollection.AddR("defaultBullet",path);
        AssetsCollections::HandlesObject3DStatic.push_back(handle);
        std::cout << handle << std::endl;
        bullet = (AssetsCollections::Object3DStaticCollection.GetElement(handle));
        bullet->setScale(glm::vec3(0.05,0.05,0.05));

    }
    once = true;
}

void Projectile::update(double deltaTime)
{
    this->_uptime-= deltaTime;
    if(_uptime<0.0){
        scene->destroyObject(_bullet);
        _type = -1;
        return;

    }
    else{
        switch(_type){
            case 0:
            break;
        }
    }
    updateDefault( deltaTime);

    _bullet->setPosition(_position);
}

void Projectile::createBullet(Scene *scene)
{

    setUp(scene);
    this->scene = scene;
    if(bullet!=nullptr){


        _bullet = scene->addObject(bullet);

    }
}

void Projectile::updateDefault(double deltaTime){
    _position = _position + this->_direction * this->_velocity * (float)deltaTime;
}
