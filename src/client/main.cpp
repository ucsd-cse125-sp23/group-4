#include "client/graphics/main.h"

#include <core/lib.hpp>
#include <iostream>
#include <network/message.hpp>
#include <network/tcp_client.hpp>

void error_callback(int error, const char* description) {
  std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window) {
  // Set the error callback.
  glfwSetErrorCallback(error_callback);
  // Set the window resize callback.
  glfwSetWindowSizeCallback(window, Window::resizeCallback);

  // Set the key callback.
  glfwSetKeyCallback(window, Window::keyCallback);

  glfwSetCharCallback(window, Window::charCallback);

  // Set the mouse and cursor callbacks
  glfwSetMouseButtonCallback(window, Window::mouse_callback);
  glfwSetScrollCallback(window, Window::scroll_callback);
  glfwSetCursorPosCallback(window, Window::cursor_callback);
}

void setup_opengl_settings() {
  // Enable depth buffering.
  glEnable(GL_DEPTH_TEST);
  // Related to shaders and z value comparisons for the depth buffer.
  glDepthFunc(GL_LEQUAL);
  // Set polygon drawing mode to fill front and back of each polygon.
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // Set clear color to black.
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions() {
  // Get info of GPU and supported OpenGL version.
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
            << std::endl;

  // If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
  std::cout << "Supported GLSL version is: "
            << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(int argc, char* argv[]) {
  char* arg_host;
  char* arg_port;

  if (argc != 3) {
    std::cout << "usage: client <host> <port>" << std::endl;

    const unsigned int kMaxArgLength = 64;
    char argbuff_1[kMaxArgLength];
    char argbuff_2[kMaxArgLength];

    std::cout << "enter <host>:";
    std::cin.getline(argbuff_1, kMaxArgLength);

    std::cout << "enter <port>:";
    std::cin.getline(argbuff_2, kMaxArgLength);

    arg_host = argbuff_1;
    arg_port = argbuff_2;
  } else {
    arg_host = argv[1];
    arg_port = argv[2];
  }

  std::cout << "connecting to: " << arg_host << ":" << arg_port << std::endl;

  // NETWORK CODE
  boost::asio::io_context io_context;
  TCPClient client(io_context, argv[1], argv[2]);
  struct message::Greeting g = {"Hello!"};
  message::Message m = {message::Type::Greeting, {0, std::time(nullptr)}, g};

  for (;;) {
    client.write(m);
    message::Message response = client.read();
    std::cout << "Server response: " << response << std::endl;
  }

  // Create the GLFW window.
  GLFWwindow* window = Window::createWindow(800, 600);
  if (!window) exit(EXIT_FAILURE);

  glutInit(&argc, argv);

  // Print OpenGL and GLSL versions.
  print_versions();
  // Setup callbacks.
  setup_callbacks(window);
  // Setup OpenGL settings.
  setup_opengl_settings();

  // Initialize the shader program; exit if initialization fails.
  if (!Window::initializeProgram(window)) {
    std::cout << "Press enter...";
    char i = std::getchar();
    exit(EXIT_FAILURE);
  }
  // Initialize objects/pointers for rendering; exit if initialization fails.
  if (!Window::initializeObjects()) {
    std::cout << "Press enter...";
    char i = std::getchar();
    exit(EXIT_FAILURE);
  }

  // Delta time logic (see
  // https://stackoverflow.com/questions/20390028/c-using-glfwgettime-for-a-fixed-time-step)
  double lastTime = glfwGetTime();
  double deltaTime = 0, nowTime = 0;

  // Loop while GLFW window should stay open.
  while (!glfwWindowShouldClose(window)) {
    // - Measure time
    nowTime = glfwGetTime();
    deltaTime = nowTime - lastTime;
    lastTime = nowTime;

    // Idle callback. Updating objects, etc. can be done here.
    Window::idleCallback(window, deltaTime);

    // Main render display callback. Rendering of objects is done here.
    Window::displayCallback(window);
  }

  Window::cleanUp();
  // Destroy the window.
  glfwDestroyWindow(window);
  // Terminate GLFW.
  glfwTerminate();

  exit(EXIT_SUCCESS);
}
