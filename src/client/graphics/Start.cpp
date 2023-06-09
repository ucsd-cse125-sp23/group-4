#include "Start.h"

#include "Window.h"

void Start::update(float delta) {
  timeOnFrame += delta;
  if (timeOnFrame >= 0.1 && index < frames.size() - 1) {
    index++;
    timeOnFrame = 0;
  }

  camera->SetAzimuth(camera->GetAzimuth() + (5 * delta));

  camera->update(delta);

  if (index == frames.size() - 1 && timeOnFrame >= 1.25) {
    renderMain = true;
  }

  if (renderMain) {
    alpha += (0.75 * delta);
    if (alpha > 1) {
      alpha = 1;
    }
  }

  if (renderMain &&
      Input::GetInputState(InputAction::Enter) == InputState::Press) {
    Window::phase = GamePhase::Lobby;
  }

  if (alpha == 1 &&
      Input::GetInputState(InputAction::Tab) == InputState::Press) {
    alpha2 = 1;
  }

  if (alpha == 1 &&
      Input::GetInputState(InputAction::Tab) == InputState::Release) {
    alpha2 = 0;
  }

  if (alpha == 1 &&
      Input::GetInputState(InputAction::MoveJump) == InputState::Hold) {
    offset += (3 * delta);
    if (offset > 2) {
      offset = 2;
    }
  }

  if (alpha == 1 && (Input::GetInputState(InputAction::MoveJump) == InputState::Release || Input::GetInputState(InputAction::MoveJump) == InputState::None)) {
    offset -= (3 * delta);
    if (offset < 0) {
      offset = 0;
    }
  }
}

void Start::init() {
  Scene::init();
  for (int i = 1; i < 33; i++) {
    Texture frame;
    std::string filename = "assets/image/tagguys_open/frame_" +
                           std::to_string(i) + "_delay-0.1s.png";
    frame.init(filename.c_str());
    frames.push_back(frame);
  }

  main.init("assets/UI/IMG_2510 2.PNG");
  credits.init("assets/UI/IMG_2509 2.PNG");
}

void Start::draw() {
  Scene::draw();
  if (renderMain) {
    drawMain();
  }
  drawName();
  glDisable(GL_DEPTH_TEST);
  drawCredits();
  glEnable(GL_DEPTH_TEST);
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
  glVertex2f(-1, -1 + offset);

  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1, -1 + offset);

  glTexCoord2f(1, 0);
  glVertex2f(1, 1 + offset);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1, 1 + offset);

  glEnd();

  glDisable(GL_BLEND);

  glDisable(GL_TEXTURE_2D);
}

void Start::drawCredits() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  credits.bindgl();

  glEnable(GL_TEXTURE_2D);

  glColor4f(1.0f, 1.0f, 1.0f, alpha2);
  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-1, -1 + offset);

  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1, -1 + offset);

  glTexCoord2f(1, 0);
  glVertex2f(1, 1 + offset);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1, 1 + offset);

  glEnd();

  glDisable(GL_BLEND);

  glDisable(GL_TEXTURE_2D);
}
