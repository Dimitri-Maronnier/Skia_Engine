#include "inputhandler.h"
std::map<int,bool> InputHandler::KeyboardHandle;
std::map<Qt::MouseButton,bool> InputHandler::MouseHandle;
glm::vec2 InputHandler::MousePos;
glm::vec2 InputHandler::MouseDelta;
