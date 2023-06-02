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
#include <ios>
#include <iostream>
#include <network/message.hpp>
#include <network/tcp_client.hpp>
#include <string>
#include <thread>

#include "Window.h"

bool net_assigned = false;
bool is_game_ready = false;
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

    auto lobby_update_handler = [](const message::LobbyUpdate& body) {};

    auto game_ready_handler = [](const message::GameStart& body) {
      is_game_ready = true;
    };

    auto any_handler = [](const message::Message::Body&) {};

    auto message_handler = boost::make_overloaded_function(
        assign_handler, game_state_update_handler, lobby_update_handler,
        game_ready_handler, any_handler);
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

  // Check for user ready
  if (!game_exit) {
    std::cout << "Press Enter when you are ready to start..." << std::endl;

    while (!Window::readyInput) {  // press enter check
      if (glfwWindowShouldClose(window)) {
        game_exit = true;
        break;
      }

      Window::draw(window);  // TODO: this should be lobby draw
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    if (!game_exit) {
      client->write<message::LobbyPlayerUpdate>(Window::gameScene->_myPlayerId,
                                                "", true);
      std::cout << "Ready!" << std::endl;
    }
  }

  // After ready, wait until server GameReady message
  while (!game_exit && !is_game_ready) {
    if (glfwWindowShouldClose(window)) {
      game_exit = true;
      break;
    }

    client->poll();
    Window::draw(window);  // TODO: this should be lobby draw
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }

  // rate limit sending updates to network, credit:
  // https://stackoverflow.com/questions/20390028/c-using-glfwgettime-for-a-fixed-time-step
  const double min_time_between_updates = 1.0 / 30;
  double prev_time = glfwGetTime();
  double num_updates_to_send = 0;

  double time_elapsed = 0;
  int frame_count = 0;
  int update_count = 0;

  // Main game loop
  while (!game_exit && !glfwWindowShouldClose(window)) {
    // check for updates from server
    client->poll();

    // update stats
    frame_count++;
    double curr_time = glfwGetTime();
    double time_since_prev_frame = curr_time - prev_time;
    prev_time = curr_time;

    // handle updates to server
    num_updates_to_send += time_since_prev_frame / min_time_between_updates;
    while (num_updates_to_send >= 1.0) {
      message::UserStateUpdate user_update = Window::gameScene->pollUpdate();

      // check if update if valid
      // TODO: in the future, remove this check since the player will definitely
      // be initialized once we enter the game render loop
      if (user_update.id == Window::gameScene->_myPlayerId)
        client->write<message::UserStateUpdate>(user_update);

      update_count++;
      num_updates_to_send--;
    }

    // calculate fps/ups
    time_elapsed += time_since_prev_frame;
    if (time_elapsed > 1.0) {
      Window::fps = frame_count / time_elapsed;
      Window::ups = update_count / time_elapsed;

      time_elapsed = 0;
      update_count = 0;
      frame_count = 0;
    }

    // update and render scene
    Window::update(window, time_since_prev_frame);
    Window::draw(window);
  }

  Window::cleanUp();
  glfwDestroyWindow(window);
  glfwTerminate();
}
