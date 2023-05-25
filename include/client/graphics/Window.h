/**************************************************
 * Window.h
 * in charge of the game window, storing the
 * game scene, and setting up input handling
 *****************************************************/

#pragma once

#include <network/message.hpp>

#include "client/graphics/Camera.h"
#include "client/graphics/HUD.h"
#include "client/graphics/Load.h"
#include "client/graphics/Lobby.h"
#include "client/graphics/Scene.h"
#include "client/graphics/Start.h"
#include "client/graphics/main.h"

////////////////////////////////////////////////////////////////////////////////

class Window {
 public:
  // Window Properties
  static int width;
  static int height;
  static const char* windowTitle;
  static bool inGame;

  // Objects to render
  static Scene* gameScene;
  static HUD* hud;

  // Act as Constructors and desctructors
  static bool initializeProgram(GLFWwindow* window);
  static bool initializeObjects();

  static void cleanObjects();
  static void cleanUp();

  // for the Window
  static GLFWwindow* createWindow(int width, int height);
  static void resizeCallback(GLFWwindow* window, int width, int height);

  // update and draw functions
  static message::UserStateUpdate idleCallback(GLFWwindow* window,
                                               float deltaTime);
  static void displayCallback(GLFWwindow*);

  // helper to reset the camera
  static void resetCamera();

  // callbacks - for interaction
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                          int mods);
  static void charCallback(GLFWwindow* window, unsigned int codepoint);
  static void mouse_callback(GLFWwindow* window, int button, int action,
                             int mods);
  static void scroll_callback(GLFWwindow* window, double xoffset,
                              double yoffset);
  static void cursor_callback(GLFWwindow* window, double currX, double currY);
};

////////////////////////////////////////////////////////////////////////////////
