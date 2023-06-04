#include "Leaderboard.h"

Leaderboard::Leaderboard(Camera* camFromWindow) : Scene(camFromWindow) {
  background.init("assets/image/backgrounds/blue.png");
}

void Leaderboard::init() {}

void Leaderboard::update(float delta) {
  if (Input::GetInputState(InputAction::Enter) == InputState::Press) {
    // switch to lobby
  }
}

void Leaderboard::draw() {
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float scale = static_cast<float>(width) / static_cast<float>(800);

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

  glViewport(0, height - (100 * scale), width, 90 * scale);
  glColor3f(213.0f / 256.0f, 236.0f / 256.0f, 241.0f / 256.0f);
  glBegin(GL_QUADS);

  glVertex2f(1, 1);
  glVertex2f(-1, 1);
  glVertex2f(-1, -1);
  glVertex2f(1, -1);

  glEnd();

  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);

  float leaderboard_width = fr->TextWidth("Leaderboard", 1.5 * scale);
  fr->RenderText(width, 90 * scale, "Leaderboard",
                 (static_cast<float>(width) / 2.0) - (leaderboard_width / 2.0),
                 25 * scale, 1.5 * scale,
                 glm::vec3(50.0 / 256.0, 65.0 / 256.0, 68.0 / 256.0));
  glViewport(0, 0, width, height);

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}
