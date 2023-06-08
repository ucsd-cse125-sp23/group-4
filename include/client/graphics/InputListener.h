#pragma once

enum class InputState { None, Release, Press, Hold };

enum class InputAction {
  Help,
  Quit,
  Reset,
  Screenshot,
  MoveForward,
  MoveBack,
  MoveLeft,
  MoveRight,
  MoveJump,
  CameraZoomIn,
  CameraZoomOut,
  Enter,
  WALK,
  IDLE,
  TRIP,
  FALL,
  LOBBY1,
  LOBBY2,
  LOBBY3,
  LOBBY4,
  WINNING11,
  WINNING12,
  WINNING21,
  WINNING22,
  WINNING31,
  WINNING32,
  WINNING41,
  WINNING42,
  TAG
};

struct InputEvent {
  InputAction action;
  InputState state;

  InputEvent(InputAction a, InputState s) {
    action = a;
    state = s;
  }
};

class InputListener {
 public:
  virtual void OnEvent(const InputEvent evt) {
    // yeah (override me)
  }
};
