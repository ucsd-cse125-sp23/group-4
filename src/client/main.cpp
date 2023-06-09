// clang-format off
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
// clang-format on
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

void network_init() {
  auto config = get_config();
  Addr server_addr{config["server_address"], config["server_port"]};

  auto connect_handler = [](tcp::endpoint endpoint, Client& client) {};

  auto read_handler = [](const message::Message& m, Client& client) {
    auto assign_handler = [&client](const message::Assign& body) {
      Window::gameScene->initFromServer(body.pid);  // need a unique int id
      Window::my_pid = body.pid;
      client.write<message::Greeting>("Hello, server!");
      net_assigned = true;
    };

    auto game_state_update_handler = [](const message::GameStateUpdate& body) {
      Window::gameScene->receiveState(body);
    };

    auto lobby_update_handler = [](const message::LobbyUpdate& body) {
      if (Window::phase == GamePhase::Lobby) {
        dynamic_cast<Lobby*>(Window::gameScene)->receiveState(body);
      } else {
        Window::lobby_state = body;
      }
    };

    auto game_start_handler = [](const message::GameStart& body) {
      Window::phase = GamePhase::Game;
    };

    // event messages
    auto jump_event_handler = [](const message::JumpEvent& body) {
      Window::gameScene->receiveEvent_jump(body);
    };

    auto land_event_handler = [](const message::LandEvent& body) {
      Window::gameScene->receiveEvent_land(body);  // bug: fires cont.
    };

    auto item_pickup_event_handler = [](const message::ItemPickupEvent& body) {
      Window::gameScene->receiveEvent_item(body);  // bug: fires cont.
    };

    auto tag_event_handler = [](const message::TagEvent& body) {
      Window::gameScene->receiveEvent_tag(body);
    };

    auto any_handler = [](const message::Message::Body&) {};

    auto message_handler = boost::make_overloaded_function(
        assign_handler, game_state_update_handler, lobby_update_handler,
        game_start_handler, jump_event_handler, land_event_handler,
        item_pickup_event_handler, tag_event_handler, any_handler);
    boost::apply_visitor(message_handler, m.body);
  };

  auto write_handler = [](std::size_t bytes_transferred,
                          const message::Message& m, Client& client) {};

  Window::client = std::make_unique<Client>(server_addr, connect_handler,
                                            read_handler, write_handler);
}

int main(int argc, char* argv[]) {
  network_init();

  // Create the GLFW window.
  GLFWwindow* window = Window::createWindow(800, 600);
  if (!window) exit(EXIT_FAILURE);

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
    Window::client->poll();
    Window::draw(window);  // necessary otherwise window will hang

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
    Window::client->poll();

    // update stats
    frame_count++;
    double curr_time = glfwGetTime();
    double time_since_prev_frame = curr_time - prev_time;
    prev_time = curr_time;

    if (Window::phase == GamePhase::Game) {
      // handle updates to server
      num_updates_to_send += time_since_prev_frame / min_time_between_updates;
      if (num_updates_to_send >= 1.0) {
        message::UserStateUpdate user_update = Window::gameScene->pollUpdate();

        // check if update if valid
        // TODO: in the future, remove this check since the player will
        // definitely be initialized once we enter the game render loop
        if (user_update.id == Window::gameScene->_myPlayerId)
          Window::client->write<message::UserStateUpdate>(user_update);

        // update player anims (SLOW)
        Window::animate(min_time_between_updates * num_updates_to_send);

        update_count++;
        num_updates_to_send = 0;
      }
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
