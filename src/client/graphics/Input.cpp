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
    {GLFW_KEY_RIGHT, InputAction::MoveRight},
    {GLFW_KEY_ENTER, InputAction::Enter},
    {GLFW_KEY_I, InputAction::WALK},
    {GLFW_KEY_K, InputAction::IDLE},
    {GLFW_KEY_1, InputAction::WINNING11},
    {GLFW_KEY_2, InputAction::WINNING12},
    {GLFW_KEY_3, InputAction::WINNING21},
    {GLFW_KEY_4, InputAction::WINNING22},
    {GLFW_KEY_5, InputAction::WINNING31},
    {GLFW_KEY_6, InputAction::WINNING32},
    {GLFW_KEY_7, InputAction::WINNING41},
    {GLFW_KEY_8, InputAction::WINNING42},
    {GLFW_KEY_F1, InputAction::LOBBY1},
    {GLFW_KEY_F2, InputAction::LOBBY2},
    {GLFW_KEY_F3, InputAction::LOBBY3},
    {GLFW_KEY_F4, InputAction::LOBBY4},
    {GLFW_KEY_J, InputAction::TRIP},
    {GLFW_KEY_L, InputAction::FALL},
    {GLFW_KEY_E, InputAction::TAG}};

std::pair<bool, bool> Input::pressed[400];

std::map<InputAction, InputState> Input::inputStates;
std::map<InputAction, InputState> Input::inputStateCache;

std::vector<InputListener*> Input::listeners;
