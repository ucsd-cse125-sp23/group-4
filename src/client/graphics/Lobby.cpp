#include "Lobby.h"

#include <network/message.hpp>

#include "Lobby.inl"
#include "Window.h"

Lobby::Lobby(Camera* camFromWindow) : Scene(camFromWindow) {
  is_ready = false;
  index = 0;

  background.init("assets/image/character_select.png");
  flag.init("assets/image/flag.png");

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
}

void Lobby::update(float delta) {
  GameThing* display = models[index];
  display->update(delta);

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
  glEnable(GL_DEPTH_TEST);
  Scene::draw();
  drawPlayers();
}

void Lobby::drawBackground() {
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float scale = static_cast<float>(width) / static_cast<float>(800);

  glColor3f(213.0f / 256.0f, 236.0f / 256.0f, 241.0f / 256.0f);
  glBegin(GL_QUADS);

  glVertex2f(1, 1);
  glVertex2f(-1, 1);
  glVertex2f(-1, -1);
  glVertex2f(1, -1);

  glEnd();

  glViewport(0, height - (100 * scale), width, 75 * scale);
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

  float flag_size = (width / 2.5) > 800 ? 800 : width / 2.5f;
  glViewport(0, height - (200 * scale), flag_size, 75 * scale);
  flag.bindgl();

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

  glDisable(GL_TEXTURE_2D);

  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);

  fr->RenderText(flag_size, 75 * scale, "Use arrow keys to make selection", 10,
                 75 * scale - (48 * 0.35 * scale) - 25, 0.35 * scale,
                 glm::vec3(50.0 / 256.0, 65.0 / 256.0, 68.0 / 256.0));
  fr->RenderText(flag_size, 75 * scale, "Press enter to lock in", 10,
                 75 * scale - (2 * 48 * 0.35 * scale) - 25, 0.35 * scale,
                 glm::vec3(50.0 / 256.0, 65.0 / 256.0, 68.0 / 256.0));
  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);

  glViewport(0, 0, width, height);

  glColor3f(206.0f / 256.0f, 228.0f / 256.0f, 233.0f / 256.0f);

  glBegin(GL_QUADS);

  glVertex2f(1, -0.5);
  glVertex2f(-1, -0.5);
  glVertex2f(-1, -1);
  glVertex2f(1, -1);

  glEnd();
}

void Lobby::drawPlayers() {
  glDisable(GL_DEPTH_TEST);
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float scale = static_cast<float>(width) / static_cast<float>(800);

  // TODO: fix size and scaling
  int size = width / 4;
  float midpoint = width / 2.0f;
  int x = midpoint - (1.25 * size);
  int y = -size / 6;
  for (auto player : players) {
    message::LobbyPlayer play = player.second;
    int id = play.id;
    std::string name = play.skin;

    glViewport(x, y, size, size);

    if (id == _myPlayerId) {
      glColor3f(1, 0, 0);
      glLineStipple(1, 0xF00F);
      glEnable(GL_LINE_STIPPLE);
      glLineWidth(4);
      glBegin(GL_LINES);
      glVertex2f(-0.75, -0.65);
      glVertex2f(-0.32, 0.6);

      glVertex2f(-0.32, 0.6);
      glVertex2f(0.65, 0.6);

      glVertex2f(0.65, 0.6);
      glVertex2f(0.2, -0.65);

      glVertex2f(-0.75, -0.65);
      glVertex2f(0.2, -0.65);

      glEnd();
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (!play.is_ready) {
      icons[name].bindgl();
    } else {
      ready_icons[name].bindgl();
    }
    glEnable(GL_TEXTURE_2D);

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

    x += (size / 2);
  }
  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);
}
