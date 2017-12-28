#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#pragma once
#include <map>
#include <QString>
#include <glm/vec2.hpp>

class InputHandler{
public:
    static std::map<int,bool> KeyboardHandle;
    static std::map<Qt::MouseButton,bool> MouseHandle;
    static glm::vec2 MousePos;
    static glm::vec2 MouseDelta;

};

#endif // INPUTHANDLER_H
