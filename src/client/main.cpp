// clang-format off
#include <GL/glew.h>
// clang-format on
#include <GL/freeglut_std.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/functional/overloaded_function.hpp>
#include <config/lib.hpp>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <network/message.hpp>
#include <network/tcp_client.hpp>
#include <string>

#include "Window.h"

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

void network_init() {
  auto config = get_config();
  Addr server_addr{config["server_address"], config["server_port"]};

  boost::asio::io_context io_context;
  std::string player_id;
  auto connect_handler = [&](tcp::endpoint endpoint, Client& client) {
    std::cout << "Connected to " << endpoint.address() << ":" << endpoint.port()
              << std::endl;
  };
  auto read_handler = [&](const message::Message& m, Client& client) {
    std::cout << "Received " << m << std::endl;
    auto assign_handler = [&](const message::Assign& body) {
      player_id = m.metadata.player_id;
      message::Message new_m{message::Type::Greeting,
                             {player_id, std::time(nullptr)},
                             message::Greeting{"Hello, server!"}};
      client.write(new_m);
    };
    auto greeting_handler = [&](const message::Greeting& body) {};

    auto message_handler = boost::bind<void>(
        boost::make_overloaded_function(assign_handler, greeting_handler),
        boost::placeholders::_1);

    boost::apply_visitor(message_handler, m.body);
  };
  auto write_handler = [&](std::size_t bytes_transferred, Client& client) {
    std::cout << "Successfully wrote " << bytes_transferred
              << " bytes to server" << std::endl;
  };
  Client client(io_context, server_addr, connect_handler, read_handler,
                write_handler);
  io_context.run();
}

int main(int argc, char* argv[]) {
  network_init();
  return 0;

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

  // Loop while GLFW window should stay open.
  while (!glfwWindowShouldClose(window)) {
    // - Measure time
    double nowTime = glfwGetTime();
    double deltaTime = nowTime - lastTime;
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
