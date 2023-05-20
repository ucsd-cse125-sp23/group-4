#include "client/graphics/Lobby.h"

#include "Lobby.inl"

void Lobby::update(float delta) {
  GameThing* display = gamethings[index];
  display->update(delta);
  if (Input::GetInputState(InputAction::Enter) == InputState::Press) {
    ready = true;
    selectedModel = player_models[index];
  }
  if (Input::GetInputState(InputAction::MoveRight) == InputState::Press) {
    index++;
    if (index >= gamethings.size()) index = 0;
    buildSceneTree();
  }
  if (Input::GetInputState(InputAction::MoveLeft) == InputState::Press) {
    index--;
    if (index < 0) index = gamethings.size() - 1;
    buildSceneTree();
  }
}

void Lobby::buildSceneTree() {
  GameThing* display = gamethings[index];
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
  float scale = float(width) / float(800);

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
  glScissor(0, height - (200 * scale), flag_size, 75 * scale);
  glEnable(GL_SCISSOR_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_SCISSOR_TEST);
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
                 75 * scale - (48 * 0.35 * scale) - 25,
                 0.35 * scale, glm::vec3(50.0 / 256.0, 65.0 / 256.0, 68.0 / 256.0));
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
  float scale = float(width) / float(800);

  int size = (width / 8 > 200) ? 200 : width / 8;
  float midpoint = width / 2.0f;
  int x = midpoint - (4 * (size - (30 * scale))) + 10; // TODO: fix spacing
  int y = 30;
  for (int i = 0; i < 4; i++) {
    glViewport(x, y, size, size);

    glColor3f(255.0  / 256.0, 243.0 / 256.0, 201 / 256.0);
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

    x += (size + (30 * scale));
  }
  glViewport(0, 0, width, height);
}
