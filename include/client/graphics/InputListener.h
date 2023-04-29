#pragma once

enum class InputState {
	None,
	Release,
	Press,
	Hold
};

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
	CameraZoomOut
};

struct InputEvent {
	InputAction action;
	InputState state;

	InputEvent(InputAction a, InputState s) {
		action = a;
		state = s;
	}
};

class InputListener
{
public:
	virtual void OnEvent(const InputEvent evt) {
		// yeah (override me)
	}
};