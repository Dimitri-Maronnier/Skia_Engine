#ifndef INPUTABSTRACT_H
#define INPUTABSTRACT_H
#include <glm/vec2.hpp>

class InputAbstract
{
public:
    InputAbstract();
    void connect(int materialID);
    void update();
    glm::vec2 _speed;
    glm::vec2 _aim;
    int shotPressed;
    int alternateShotPressed;
    int dashPressed;
private:
    glm::vec2 _mouseLastPos;
    int _input;

    void updateAim();
    void updateMove();
    void updateButtonPressed();


    void getMoveInputFromKeyboard();
    void getAimInputFromMouse();
    void getButtonFromKeyboard();


};

#endif // INPUTABSTRACT_H
