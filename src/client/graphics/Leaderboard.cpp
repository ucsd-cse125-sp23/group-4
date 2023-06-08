#include "Leaderboard.h"

Leaderboard::Leaderboard() {
  background.init("assets/UI/Group 14.png");
}

void Leaderboard::draw() {
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float scale_x = static_cast<float>(width) / static_cast<float>(800);
  float scale_y = static_cast<float>(height) / static_cast<float>(600);

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

  glDisable(GL_DEPTH_TEST);
  glViewport(0, height - (150 * scale_y), width, 90 * scale_y);

  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);

  float leaderboard_width = fr->TextWidth("Leaderboard", 1.5 * scale_y);
  fr->RenderText(width, 90 * scale_y, "Leaderboard",
                 (static_cast<float>(width) / 2.0) - (leaderboard_width / 2.0),
                 25 * scale_y, 1.5 * scale_y,
                 glm::vec3(50.0 / 256.0, 65.0 / 256.0, 68.0 / 256.0));
  glViewport(0, 0, width, height);

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}
