/**************************************************
 * Input.h
 * static class for input management
 * make sure to call handle() in a draw/update function
 * usage:
 *	- Input::GetInputState(InputAction::MoveForward)
 *	- or see InputListener
 *****************************************************/

#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "client/graphics/InputListener.h"

class Input {
 private:
  static const int num_actions = 11;

  // default bindings
  static std::map<int, InputAction> inputmap;

  // raw state machine
  static std::pair<bool, bool> pressed[400];

  // one true map, and one cache that is updated mid frame
  static std::map<InputAction, InputState> inputStates;
  static std::map<InputAction, InputState> inputStateCache;

  static void setInputState(InputAction e, InputState s) {
    if (s > Input::inputStateCache[e]) Input::inputStateCache[e] = s;

    // do listener function on press/release
    if (s == InputState::Press || s == InputState::Release) {
      InputEvent r = InputEvent(e, s);
      Broadcast(r);
    }
  }

  // observer/listener pattern
  static std::vector<InputListener*> listeners;

  static void Broadcast(const InputEvent msg) {
    for (InputListener* li : Input::listeners) {
      li->OnEvent(msg);
    }
  }

 public:
  static void init() {
    // populate states maps
    for (int i = 0; i <= num_actions; i++) {
      InputAction e = static_cast<InputAction>(i);

      Input::inputStates.insert(
          std::pair<InputAction, InputState>(e, InputState::None));
      Input::inputStateCache.insert(
          std::pair<InputAction, InputState>(e, InputState::None));
    }
  }

  static InputState GetInputState(InputAction e) {
    return Input::inputStates[e];
  }

  static void AddListener(InputListener* p) { Input::listeners.push_back(p); }
  static void Clear() { Input::listeners.clear(); }

  static void handle(bool gui) {
    if (gui) ImGui::Begin("input");

    // reset state cache...
    for (auto& state : inputStateCache) {
      state.second = InputState::None;
    }

    // key loop
    for (const auto& e : inputmap) {
      std::string v = "nothing";
      const std::pair<bool, bool>* istate = &Input::pressed[e.first];

      if (istate->first && istate->second) {  // held
        setInputState(e.second, InputState::Hold);
        v = "held";
      } else if (istate->first) {  // press
        setInputState(e.second, InputState::Press);
        v = "press";
      } else if (istate->second) {  // release
        setInputState(e.second, InputState::Release);
        v = "released";
      } else {
        setInputState(e.second, InputState::None);
        continue;
      }

      if (gui)
        ImGui::Text((std::string("") + std::to_string(e.first) + v).c_str());

      // progress state:
      Input::pressed[e.first].second = Input::pressed[e.first].first;
    }

    if (gui) ImGui::End();

    inputStates = inputStateCache;  // end of frame: apply
  }

  static void keyListener(GLFWwindow* win, int key, int scancode, int action,
                          int mods) {
    if (action == GLFW_PRESS) {
      Input::pressed[key].first = true;
    } else if (action == GLFW_RELEASE) {
      Input::pressed[key].first = false;
    }

    // edge cases
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      // Close the window. This causes the program to also terminate.
      glfwSetWindowShouldClose(win, GL_TRUE);
    }
  }
};
