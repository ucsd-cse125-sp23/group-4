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
}

void Lobby::drawBackground() {
  background.bindgl();
  glEnable(GL_TEXTURE_2D);

  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(1, -1);

  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1, 1);

  glTexCoord2f(1, 0);
  glVertex2f(-1, 1);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1, -1);

  glEnd();

  glDisable(GL_TEXTURE_2D);
}
