////////////////////////////////////////
// Window.cpp
// please refrain from modifying this a lot
////////////////////////////////////////

#include "Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stddef.h>

#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include "Camera.h"
#include "Input.h"
#include "Scene.h"
#include "UserState.h"

////////////////////////////////////////////////////////////////////////////////

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "tagguys :O";
bool Window::inGame;

// Game stuff to render
Scene* Window::gameScene;
HUD* Window::hud;

Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

bool _debugmode = false;

////////////////////////////////////////////////////////////////////////////////

bool showSkelMode = true;
bool wireframeMode = false;
bool cullingMode = false;

bool fileIsType(std::string n, std::string type = ".txt") {
  return n.find(type) != std::string::npos;
}

// Constructors and desconstructors
bool Window::initializeProgram(GLFWwindow* window) {
  // Setup imgui --
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  // --

  return true;
}

bool Window::initializeObjects() {
  // gameScene = new Start(Cam);
  gameScene = new Scene(Cam);
  gameScene->init();
  hud = new HUD(gameScene);

  return true;
}

void Window::cleanObjects() {
  // Deallcoate the objects.
  delete gameScene;
}

void Window::cleanUp() {
  cleanObjects();

  // stop imgui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
}

////////////////////////////////////////////////////////////////////////////////

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
  // Initialize GLFW.
  inGame = false;
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return NULL;
  }

  // 4x antialiasing.
  glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
  // Apple implements its own version of OpenGL and requires special treatments
  // to make it uses modern OpenGL.

  // Ensure that minimum OpenGL version is 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Enable forward compatibility and allow a modern OpenGL context
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Create the GLFW window.
  GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

  // Check if the window could not be created.
  if (!window) {
    std::cerr << "Failed to open GLFW window." << std::endl;
    glfwTerminate();
    return NULL;
  }

  // Make the context of the window.
  glfwMakeContextCurrent(window);

#ifndef __APPLE__
  // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

  // Initialize GLEW.
  if (glewInit()) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return NULL;
  }
#endif

  // Set swap interval to 1.
  glfwSwapInterval(0);

  // imgui setup in initializeProgram()

  // set up the camera
  Cam = new Camera();
  Cam->SetAspect(static_cast<float>(width) / static_cast<float>(height));

  // initialize the interaction variables
  LeftDown = RightDown = false;
  MouseX = MouseY = 0;

  // Call the resize callback to make sure things get drawn immediately.
  Window::resizeCallback(window, width, height);

  return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
#ifdef __APPLE__
  // In case your Mac has a retina display.
  glfwGetFramebufferSize(window, &width, &height);
#endif
  Window::width = width;
  Window::height = height;
  // Set the viewport size.
  glViewport(0, 0, width, height);

  Cam->SetAspect(static_cast<float>(width) / static_cast<float>(height));

  // ImGui::WindowSize(w, h)?
}

////////////////////////////////////////////////////////////////////////////////

// update and draw functions
message::UserStateUpdate Window::idleCallback(GLFWwindow* window,
                                              float deltaTime) {
  // Perform any updates as necessary.
  Cam->UpdateView(window);

  UserState inputChanges = gameScene->update(deltaTime);
  /*
  Lobby* lobby;
  if (dynamic_cast<Start*>(gameScene) != nullptr) {
      Start* startScreen = dynamic_cast<Start*>(gameScene);
      if (startScreen->gameStart) {
          gameScene = new Lobby(Cam);
          gameScene->init();
      }
  }
  else if (dynamic_cast<Lobby*>(gameScene) != nullptr) {
      Lobby* lobby = dynamic_cast<Lobby*>(gameScene);
      if (lobby->gameStart) {
          gameScene = new Load(Cam);
          gameScene->init(lobby->selectedModel);
          //hud = new HUD(gameScene);
      }
  }
  else if (dynamic_cast<Load*>(gameScene) != nullptr) {
      Load* load = dynamic_cast<Load*>(gameScene);
      inGame = load->gameStart;
      if (inGame) {
          gameScene = new Scene(Cam);
          gameScene->init(load->selectedModel);
          hud = new HUD(gameScene);
      }
  }*/
  return inputChanges.toMessage();  // player input to be written to server
}

void Window::displayCallback(GLFWwindow* window) {
  // Gets events, including input such as keyboard and mouse or window resizing.
  glfwPollEvents();

  // Clear the color and depth buffers.                     ******
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  // Render the objects.
  gameScene->draw();
  hud->draw(window);

  Input::handle(false);
  if (_debugmode) {
    // imgui new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    gameScene->gui();

    // imguiDraw(skeleton, animClip);   // simple helper method

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  // Swap buffers.
  // ******
  glfwSwapBuffers(window);
}

////////////////////////////////////////////////////////////////////////////////

// helper to reset the camera
void Window::resetCamera() {
  Cam->Reset();
  Cam->SetAspect(static_cast<float>(Window::width) /
                 static_cast<float>(Window::height));
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
  if (_debugmode && ImGui::GetIO().WantCaptureKeyboard) return;

  // Check for a key presses
  Input::keyListener(window, key, scancode, action, mods);

  // Check for a key press.
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_ESCAPE:
        // Close the window. This causes the program to also terminate.
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;

      case GLFW_KEY_R:
        resetCamera();
        break;
      case GLFW_KEY_TAB:
        _debugmode = !_debugmode;
        break;
      case GLFW_KEY_C:
        Cam->Fixed = !(Cam->Fixed);
        break;
      case GLFW_KEY_X:
        gameScene->sceneResources->sounds["test"]->play();  // temporary
        break;
      default:
        break;
    }
  }
}

void Window::charCallback(GLFWwindow* window, unsigned int codepoint) {
  // ImGui::WantCaptureChar???
  // if (TwEventCharGLFW(codepoint, GLFW_PRESS)) return;
}

void Window::mouse_callback(GLFWwindow* window, int button, int action,
                            int mods) {
  if (_debugmode && ImGui::GetIO().WantCaptureMouse) {
    LeftDown = RightDown = false;
    return;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    LeftDown = (action == GLFW_PRESS);
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    RightDown = (action == GLFW_PRESS);
  }
}

void Window::scroll_callback(GLFWwindow* window, double xoffset,
                             double yoffset) {
  if (_debugmode && ImGui::GetIO().WantCaptureMouse) return;

  // Zoom camera
  if (yoffset) {
    Cam->CamZoom(yoffset);
  }
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
  int maxDelta = 100;
  int dx = glm::clamp(static_cast<int>(currX) - MouseX, -maxDelta, maxDelta);
  int dy = glm::clamp(-(static_cast<int>(currY) - MouseY), -maxDelta, maxDelta);

  MouseX = static_cast<int>(currX);
  MouseY = static_cast<int>(currY);

  if (_debugmode && ImGui::GetIO().WantCaptureMouse) {
    LeftDown = RightDown = false;
    return;
  }

  // Rotate camera
  if (RightDown || LeftDown) {
    Cam->CamDrag(dx, dy);
  }
}

////////////////////////////////////////////////////////////////////////////////
