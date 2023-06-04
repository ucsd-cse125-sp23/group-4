#include "Lobby.h"

#include <network/message.hpp>

#include "Lobby.inl"
#include "Window.h"

Lobby::Lobby(Camera* camFromWindow) : Scene(camFromWindow) {
  localGameThings.clear();
  is_ready = false;
  index = 0;

  receiveState(Window::lobby_state);

  background.init("assets/image/character_select.png");
  flag.init("assets/image/flag.png");

  icons["neutral"].init("assets/icons/neutral.png");
  icons["angry"].init("assets/icons/angry.png");
  icons["blushing"].init("assets/icons/blushing.png");
  icons["crying"].init("assets/icons/crying.png");
  icons["heart"].init("assets/icons/heart.png");
  icons["side_eye"].init("assets/icons/side_eye.png");

  skin_names.push_back("neutral");
  skin_names.push_back("blushing");
  skin_names.push_back("crying");
  skin_names.push_back("side_eye");

  wait.time = 5;
  wait.countdown = true;
  gameStart = false;
  offset = 0;
}

Lobby::~Lobby() {
  for (std::pair<std::string, Node*> entry : node) {
    delete entry.second;
  }
  delete sceneResources;
}

void Lobby::update(float delta) {
  GameThing* display = models[index];
  display->update(delta);

  if (Input::GetInputState(InputAction::Enter) == InputState::Press) {
    is_ready = true;
    selectedModel = player_models[index];
    Window::client->write<message::LobbyPlayerUpdate>(
        Window::my_pid, skin_names[index], is_ready);
  }
  if (Input::GetInputState(InputAction::MoveRight) == InputState::Press) {
    index = (index + 1) % models.size();
    buildSceneTree();
    Window::client->write<message::LobbyPlayerUpdate>(
        Window::my_pid, skin_names[index], is_ready);
  }
  if (Input::GetInputState(InputAction::MoveLeft) == InputState::Press) {
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
  lockIn();
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
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float scale = static_cast<float>(width) / static_cast<float>(800);

  int size = (width / 8 > 200) ? 200 : width / 8;
  float midpoint = width / 2.0f;
  int x = midpoint - (4 * (size - (30 * scale))) + 10;  // TODO: fix spacing
  int y = 30;
  for (auto player : players) {
    message::LobbyPlayer play = player.second;
    int id = play.id;
    std::string name = play.skin;
    // get index
    // draw
    glViewport(x, y, size, size);
    if (id != _myPlayerId) {
      glColor3f(255.0 / 256.0, 243.0 / 256.0, 201 / 256.0);
    } else {
      glColor3f(1.0, 0.0, 0.0);
    }
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 1);
    glVertex2f(1, 0);
    glVertex2f(-1, 0);

    glVertex2f(0, -1);
    glVertex2f(1, 0);
    glVertex2f(-1, 0);

    glEnd();

    glColor3f(1, 1, 1);
    glLineStipple(1, 0xF00F);
    glEnable(GL_LINE_STIPPLE);
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(0, 1);
    glVertex2f(1, 0);

    glVertex2f(0, 1);
    glVertex2f(-1, 0);

    glVertex2f(0, -1);
    glVertex2f(1, 0);

    glVertex2f(0, -1);
    glVertex2f(-1, 0);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    icons[name].bindgl();
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

    x += (size + (30 * scale));
  }
  glViewport(0, 0, width, height);
}

void Lobby::lockIn() {
  if (is_ready) {
    GLFWwindow* window = glfwGetCurrentContext();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float scale = static_cast<float>(width) / static_cast<float>(800);
    glColor3f(255.0 / 256.0, 243.0 / 256.0, 201 / 256.0);

    float x = -1 + offset;
    if (x > 1) x = 1;
    glBegin(GL_QUADS);

    glVertex2f(x, 0.1);
    glVertex2f(-1, 0.1);
    glVertex2f(-1, -0.1);
    glVertex2f(x, -0.1);

    glEnd();

    glColor3f(1, 1, 1);
    glLineWidth(5);
    glBegin(GL_LINES);

    glVertex2f(-1, 0.1);
    glVertex2f(x, 0.1);

    glVertex2f(-1, -0.1);
    glVertex2f(x, -0.1);
    glEnd();

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    float text_Xndc = -1.5 + offset;
    if (text_Xndc > -0.2) text_Xndc = -0.2;
    float text_Yndc = -0.07;
    float textX = ((text_Xndc + 1) / 2) * width;
    float textY = ((text_Yndc + 1) / 2) * height;
    fr->RenderText(width, height, "Ready!", textX, textY, scale,
                   glm::vec3(137.0 / 256.0, 177.0 / 256.0, 185.0 / 256.0));

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
  }
}