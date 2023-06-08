#include "Start.h"

#include "Window.h"

void Start::update(float delta) {
  timeOnFrame += delta;
  if (timeOnFrame >= 0.1 && index < frames.size() - 1) {
    index++;
    timeOnFrame = 0;
  }

  camera->SetAzimuth(camera->GetAzimuth() + (5 * delta));

  if (index == frames.size() - 1 && timeOnFrame >= 1.25) {
    renderMain = true;
  }

  if (renderMain) {
    alpha += (0.5 * delta);
    if (alpha > 1) {
      alpha = 1;
    }
  }

  if (renderMain && Input::GetInputState(InputAction::Enter) == InputState::Press) {
    Window::phase = GamePhase::Lobby;
  }
}

void Start::draw() {
  Scene::draw();
  if (renderMain) {
    drawMain();
  }
  drawName();
  
}

void Start::drawName() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Texture curFrame = frames[index];
  curFrame.bindgl();
  glEnable(GL_TEXTURE_2D);

  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);

  glTexCoord2f(1, 0);
  glVertex3f(0.75f, 1.0f, 0.0f);
  glTexCoord2f(0, 0);
  glVertex3f(-0.75f, 1.0f, 0.0f);
  glTexCoord2f(0, 1);
  glVertex3f(-0.75f, -0.5f, 0.0f);
  glTexCoord2f(1, 1);
  glVertex3f(0.75f, -0.5f, 0.0f);

  glEnd();

  glDisable(GL_TEXTURE_2D);
}

void Start::drawMain() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  main.bindgl();

  glEnable(GL_TEXTURE_2D);

  glColor4f(1.0f, 1.0f, 1.0f, alpha);
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

  glDisable(GL_BLEND);
  
  glDisable(GL_TEXTURE_2D);
}

void Start::drawCredits(){
}
