#include "game.h"
#include <iostream>

void Game::gameRun(Scene *scene)
{
    perso.setUp(scene);
}

void Game::gameLoop(double deltaTime)
{
    perso.update(deltaTime);

    if(perso.getEntity()!=nullptr){
        Scene::camera.move(0,90,0,perso.getEntity());
        Scene::camera.setDistanceFromPivot(20);
    }

}
