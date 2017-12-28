#include "inputabstract.h"
#include "inputhandler.h"
#include <glm/geometric.hpp>

InputAbstract::InputAbstract()
{
    _aim = glm::vec2(1,0);
}

void InputAbstract::connect(int materialID)
{
    _input = materialID;
}
void InputAbstract::update(){
    updateMove();
    updateAim();
    updateButtonPressed();
}

void InputAbstract::updateButtonPressed(){
    if(_input==0)
        getButtonFromKeyboard();
    else
        getButtonFromKeyboard();
}

void InputAbstract::getButtonFromKeyboard(){
    this->alternateShotPressed = InputHandler::MouseHandle[ Qt::RightButton];
    this->shotPressed = InputHandler::MouseHandle[Qt::LeftButton];
    this->dashPressed = InputHandler::KeyboardHandle[Qt::Key_Space];
}

void InputAbstract::updateMove(){
    if(_input ==0)
       getMoveInputFromKeyboard();
    else
        getMoveInputFromKeyboard();
}

void InputAbstract::getMoveInputFromKeyboard(){
    if(InputHandler::KeyboardHandle[Qt::Key_Q])
        _speed.x = 1;
    else if(InputHandler::KeyboardHandle[Qt::Key_D])
        _speed.x = -1;
    else
        _speed.x =0;

    if(InputHandler::KeyboardHandle[Qt::Key_Z])
        _speed.y = 1;
    else if(InputHandler::KeyboardHandle[Qt::Key_S])
        _speed.y = -1;
    else
        _speed.y=0;

    if(_speed.x || _speed.y)
        _speed = glm::normalize(_speed);
}

void InputAbstract::updateAim(){
    if(_input==0)
        getAimInputFromMouse();
    else
        getAimInputFromMouse();
}

void  InputAbstract::getAimInputFromMouse(){
    glm::vec2 delta = (InputHandler::MousePos - _mouseLastPos)*-1.0f;
    _mouseLastPos = InputHandler::MousePos;
    _aim = glm::normalize(_aim*30.0f+delta);
}
