// clang-format off
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
// clang-format on
#include <GL/freeglut_std.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/functional/overloaded_function.hpp>
#include <chrono>
#include <config/lib.hpp>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <network/message.hpp>
#include <network/tcp_client.hpp>
#include <string>
#include <thread>

#include "Window.h"

bool net_assigned = false;

bool game_exit = false;

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

std::unique_ptr<Client> network_init() {
  auto config = get_config();
  Addr server_addr{config["server_address"], config["server_port"]};

  auto connect_handler = [](tcp::endpoint endpoint, Client& client) {};

  auto read_handler = [](const message::Message& m, Client& client) {
    auto assign_handler = [&client](const message::Assign& body) {
      Window::gameScene->initFromServer(body.pid);  // need a unique int id
      client.write<message::Greeting>("Hello, server!");

      net_assigned = true;
    };
    auto game_state_update_handler = [](const message::GameStateUpdate& body) {
      Window::gameScene->receiveState(body);
    };
    auto any_handler = [](const message::Message::Body&) {};

    auto message_handler = boost::make_overloaded_function(
        assign_handler, game_state_update_handler, any_handler);
    boost::apply_visitor(message_handler, m.body);
  };

  auto write_handler = [](std::size_t bytes_transferred,
                          const message::Message& m, Client& client) {};

  auto client = std::make_unique<Client>(server_addr, connect_handler,
                                         read_handler, write_handler);
  return client;
}

int main(int argc, char* argv[]) {
  auto client = network_init();

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

  // Initialize game scene for rendering; exit if initialization fails.
  if (!Window::initializeObjects()) {
    std::cout << "Press enter...";
    char i = std::getchar();
    exit(EXIT_FAILURE);
  }

  // wait for server assignment, TODO: replace with start screen UI
  while (!net_assigned) {
    if (glfwWindowShouldClose(window)) {
      game_exit = true;
      break;
    }

    std::cout << "(net_assigned: " << net_assigned << ") ";
    std::cout << "Waiting for server to assign pid..." << std::endl;
    client->poll();

    Window::draw(window);  // TODO: this should be lobby draw
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }

  // Delta time logic (see
  // https://stackoverflow.com/questions/20390028/c-using-glfwgettime-for-a-fixed-time-step)
  const double limitTPS = 1.0 / 60.0;
  double lastTime = glfwGetTime();
  double timer = lastTime;
  double deltaTimer = 0;
  int frames = 0, updates = 0;

  // Loop while GLFW window should stay open.
  while (!game_exit && !glfwWindowShouldClose(window)) {
    // - Measure time
    double nowTime = glfwGetTime();
    double deltaTime = nowTime - lastTime;
    deltaTimer += (nowTime - lastTime) / limitTPS;  // for network ticks
    lastTime = nowTime;

    // POLL FROM SERVER
    client->poll();

    // - Only update network at X frames / sec
    while (deltaTimer >= 1.0) {
      // Get a message back of input state
      message::UserStateUpdate mout = Window::gameScene->pollInput();

      // OUTPUT TO SERVER
      if (net_assigned && mout.id >= 0)
        client->write<message::UserStateUpdate>(mout);

      updates++;
      deltaTimer--;
    }

    frames++;

    // - Reset after one second
    if (glfwGetTime() - timer > 1.0) {
      timer++;
      Window::fps = frames;
      Window::ups = updates;
      updates = 0, frames = 0;
    }
    // update and render scene
    Window::update(window, time_since_prev_frame);
    Window::draw(window);

    // prevent drawing too fast...
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }

  Window::cleanUp();
  // Destroy the window.
  glfwDestroyWindow(window);
  // Terminate GLFW.
  glfwTerminate();

  exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
