/**************************************************
 * Window.h
 * in charge of the game window, storing the
 * game scene, and setting up input handling
 *****************************************************/

#pragma once

#include <network/message.hpp>

#include "Scene.h"

////////////////////////////////////////////////////////////////////////////////

class Window {
 public:
  // Window Properties
  static int width;
  static int height;
  static const char* windowTitle;

  // Objects to render
  static Scene* gameScene;

  // Act as Constructors and desctructors
  static bool initializeProgram(GLFWwindow* window);
  static bool initializeObjects();

  static void cleanObjects();
  static void cleanUp();

  // for the Window
  static GLFWwindow* createWindow(int width, int height);
  static void resizeCallback(GLFWwindow* window, int width, int height);

  // update and draw functions
  static message::Message idleCallback(GLFWwindow* window, float deltaTime,
                                       message::Message gamestate);
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
