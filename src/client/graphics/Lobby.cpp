#include "Lobby.h"

#include <network/message.hpp>

#include "Lobby.inl"
#include "Window.h"

Lobby::Lobby(Camera* camFromWindow) : Scene(camFromWindow) {
  is_ready = false;
  index = 0;

  receiveState(Window::lobby_state);

  background.init("assets/UI/Group 8.png");
  highlight.init("assets/UI/Vector 15 (1).png");
  point.init("assets/UI/Polygon 6 (1).png");

  ready_icons["trash panda"].init("assets/UI/IMG_2421.PNG");
  icons["trash panda"].init("assets/UI/IMG_2422.PNG");
  icons["bee"].init("assets/UI/IMG_2423.PNG");
  ready_icons["bee"].init("assets/UI/IMG_2424.PNG");
  icons["avocado"].init("assets/UI/IMG_2425.PNG");
  ready_icons["avocado"].init("assets/UI/IMG_2426.PNG");
  icons["duck"].init("assets/UI/IMG_2427.PNG");
  ready_icons["duck"].init("assets/UI/IMG_2428.PNG");
  icons["cat"].init("assets/UI/IMG_2429.PNG");
  ready_icons["cat"].init("assets/UI/IMG_2430.PNG");
  icons["unicorn"].init("assets/UI/IMG_2431.PNG");
  ready_icons["unicorn"].init("assets/UI/IMG_2432.PNG");
  icons["waffle"].init("assets/UI/IMG_2436.PNG");
  ready_icons["waffle"].init("assets/UI/IMG_2437.PNG");
  icons["bear"].init("assets/UI/IMG_2491.PNG");
  ready_icons["bear"].init("assets/UI/IMG_2492.PNG");
}

Lobby::~Lobby() {
  for (std::pair<std::string, Node*> entry : node) {
    delete entry.second;
  }
  delete sceneResources;
  delete fr;
}

void Lobby::reset() {
  is_ready = false;
  index = 0;

  receiveState(Window::lobby_state);

  buildSceneTree();
}

void Lobby::update(float delta) {
  GameThing* display = models[index];
  display->update(delta);
  if (auto* am = dynamic_cast<AssimpModel*>(display->model)) {
    am->update(delta);
  }

  if (Input::GetInputState(InputAction::Enter) == InputState::Press) {
    is_ready = true;
    Window::client->write<message::LobbyPlayerUpdate>(
        Window::my_pid, skin_names[index], is_ready);
  }
  if (Input::GetInputState(InputAction::MoveRight) == InputState::Press) {
    is_ready = false;
    index = (index + 1) % models.size();
    buildSceneTree();
    Window::client->write<message::LobbyPlayerUpdate>(
        Window::my_pid, skin_names[index], is_ready);
  }
  if (Input::GetInputState(InputAction::MoveLeft) == InputState::Press) {
    is_ready = false;
    index = (index - 1 + models.size()) % models.size();
    buildSceneTree();
    Window::client->write<message::LobbyPlayerUpdate>(
        Window::my_pid, skin_names[index], is_ready);
  }
}

void Lobby::receiveState(message::LobbyUpdate newState) {
  for (auto player : newState.players) {
    int ind = player.first;
    message::LobbyPlayer p = player.second;
    players[ind] = p;
  }
}

void Lobby::buildSceneTree() {
  GameThing* display = models[index];
  node["world"]->childnodes.clear();
  node["world"]->childnodes.push_back(node[display->name]);
}

void Lobby::draw() {
  glDisable(GL_DEPTH_TEST);
  drawBackground();
  drawPlayers();
  glEnable(GL_DEPTH_TEST);
  Scene::draw();
}

void Lobby::drawBackground() {
  background.bindgl();
  glEnable(GL_TEXTURE_2D);

  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-1, -1);

  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1, -1);

  glTexCoord2f(1, 0);
  glVertex2f(1, 1);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1, 1);

  glEnd();

}

void Lobby::drawPlayers() {
  glDisable(GL_DEPTH_TEST);
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  float scale_x = static_cast<float>(width) / static_cast<float>(800);
  float scale_y = static_cast<float>(height) / static_cast<float>(600);

  int size = height/3;
  
  int num_players = players.size();
  float spacing = 10.0;
  if (num_players > 4) {
    spacing = 10 * pow(0.25 * scale_x, (num_players - 4));
  }
  int x = spacing * scale_x;
  int y = 20 * scale_y;

  int count = 0;
  for (auto player : players) {
    count++;
    message::LobbyPlayer play = player.second;
    int id = play.id;
    std::string name = play.skin;

    glViewport(x, y, size, size);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (id == _myPlayerId) {
      highlight.bindgl();
      glColor3f(1.0, 1.0, 1.0);
      glBegin(GL_QUADS);

      glTexCoord2f(1, 0);
      glVertex2f(1.0f, 0.75f);
      glTexCoord2f(0, 0);
      glVertex2f(-1.0f, 0.75f);
      glTexCoord2f(0, 1);
      glVertex2f(-1.0f, -0.85f);
      glTexCoord2f(1, 1);
      glVertex2f(1.0f, -0.85f);

      glEnd();

      glViewport(0, 0, width, height);
      glViewport(x + (size / 2), y + (size * 0.85), size / 6, size / 6);

      point.bindgl();
      glBegin(GL_QUADS);

      glTexCoord2f(1, 0);
      glVertex2f(1.0f, 1.0f);
      glTexCoord2f(0, 0);
      glVertex2f(-1.0f, 1.0f);
      glTexCoord2f(0, 1);
      glVertex2f(-1.0f, -1.0f);
      glTexCoord2f(1, 1);
      glVertex2f(1.0f, -1.0f);

      glEnd();

      glViewport(x, y, size, size);
    }

    if (!play.is_ready) {
      icons[name].bindgl();
    } else {
      ready_icons[name].bindgl();
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);

    glTexCoord2f(1, 0);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0, 0);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glTexCoord2f(0, 1);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1, 1);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    x += (size / 1.5);

    if (count == ceil(players.size() / 2)) {
      int num_left = count;
      if (players.size() % 2 == 1) {
        num_left++;
      }
      x = (width - (spacing * scale_x)) - (num_left * size / 1.5) - (size / 3.0);
    }
  }
  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);
}
