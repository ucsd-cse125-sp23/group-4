#include "Input.h"

std::map<int, InputAction> Input::inputmap = {
    {GLFW_KEY_ESCAPE, InputAction::Quit},
    {GLFW_KEY_Q, InputAction::Quit},
    {GLFW_KEY_H, InputAction::Help},
    {GLFW_KEY_R, InputAction::Reset},
    {GLFW_KEY_P, InputAction::Screenshot},
    {GLFW_KEY_W, InputAction::MoveForward},
    {GLFW_KEY_S, InputAction::MoveBack},
    {GLFW_KEY_A, InputAction::MoveLeft},
    {GLFW_KEY_D, InputAction::MoveRight},
    {GLFW_KEY_SPACE, InputAction::MoveJump},
    {GLFW_KEY_UP, InputAction::MoveForward},
    {GLFW_KEY_DOWN, InputAction::MoveBack},
    {GLFW_KEY_LEFT, InputAction::MoveLeft},
    {GLFW_KEY_RIGHT, InputAction::MoveRight}};

std::pair<bool, bool> Input::pressed[400];

std::map<InputAction, InputState> Input::inputStates;
std::map<InputAction, InputState> Input::inputStateCache;

std::vector<InputListener*> Input::listeners;