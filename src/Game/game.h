#ifndef GAME_H
#define GAME_H
#include "mainperso.h"
#include "src/Engine/Entities/camerafirst.h"
#include "src/Engine/Controller/inputhandler.h"
#include "src/Engine/scene.h"

class Game
{
public:
    void gameRun(Scene *scene);
    void gameLoop(double deltaTime);
private:
    MainPerso perso;

};

#endif // GAME_H
