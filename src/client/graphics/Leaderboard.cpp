#include "Leaderboard.h"

Leaderboard::Leaderboard() {
  background.init("assets/UI/Group 14.png");
  icons["trash panda"].init("assets/UI/IMG_2376.PNG");
  icons["unicorn"].init("assets/UI/IMG_2372.PNG");
  icons["cat"].init("assets/UI/IMG_2373.PNG");
  icons["duck"].init("assets/UI/IMG_2374.PNG");
  icons["avocado"].init("assets/UI/IMG_2375.PNG");
  icons["bee"].init("assets/UI/IMG_2377.PNG");

  name_tags[1].init("assets/UI/Group 21.png");
  name_tags[2].init("assets/UI/Group 20.png");
  name_tags[3].init("assets/UI/Group 22.png");
  name_tags[4].init("assets/UI/Group 19.png");
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

void Leaderboard::drawPlayers(std::vector<std::pair<int, std::string>> rankings) {
  glDisable(GL_DEPTH_TEST);
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  std::vector<std::pair<int, int>> viewports{
      std::make_pair(0.4 * width, 0.2 * height),
      std::make_pair(0.65 * width, 0.13 * height),
      std::make_pair(0.25 * width, 0.1 * height)};

  std::vector<int> scale{static_cast<int>(0.68 * height),
                         static_cast<int>(0.58 * height),
                         static_cast<int>(0.48 * height)};
  int count = 0;
  for (auto rank : rankings) {
    auto id = rank.first;
    auto skin = rank.second;
    std::pair<int, int> position = viewports[count];
    glViewport(position.first, position.second, scale[count], scale[count]);

    icons[skin].bindgl();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1 * flip, -1);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1 * flip, -1);

    glTexCoord2f(1, 0);
    glVertex2f(1 * flip, 1);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1 * flip, 1);

    glEnd();

    name_tags[id].bindgl();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-0.5, 0.35);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(0.5, 0.35);

    glTexCoord2f(1, 0);
    glVertex2f(0.5, 0.8);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-0.5, 0.8);

    glEnd();

    glViewport(0, 0, width, height);
    count++;
    if (count == 3) {
      break;
    }
  }

  frames++;
  if (frames == 100) {
    flip *= -1;
    frames = 0;
  }

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}
