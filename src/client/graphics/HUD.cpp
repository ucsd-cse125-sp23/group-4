#include "HUD.h"

void HUD::draw(GLFWwindow* window) {
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  std::map<std::string, Player*> players;

  float scale = static_cast<float>(width) / static_cast<float>(800);

  for (auto e : scene->networkGameThings) {
    auto play = e.second;
    if (dynamic_cast<Player*>(play) != nullptr) {
      Player* player = dynamic_cast<Player*>(play);
      std::string name = player->name;
      players[name] = player;
      const unsigned char* cname =
          reinterpret_cast<const unsigned char*>(name.c_str());
      glm::vec4 position = glm::vec4(player->transform.position, 1.0f);
      glm::vec4 postProjectivePosition =
          scene->camera->GetViewProjectMtx() * position;
      glm::vec3 normalizedSpace =
          glm::vec3(postProjectivePosition) / postProjectivePosition[3];
      glm::vec2 windowSpace = ((glm::vec2(normalizedSpace) + 1.0f) / 2.0f) *
                                  glm::vec2(width, height) +
                              glm::vec2(0.0f, 0.0f);

      // Scale the size of the text based on camera distance
      Camera* cam = scene->camera;
      float deltaD = 10 - cam->GetDistance();
      float size;
      if (deltaD < 0) {
        size = (0.5 * scale) / (0.5 * abs(deltaD));
        if (size < 0.1 * scale)
          size = 0.1 * scale;
        else if (size > 0.5 * scale)
          size = 0.5 * scale;
      } else {
        size = 0.5 * scale;
      }
      fr->RenderText(width, height, name, windowSpace[0], windowSpace[1], size,
                     glm::vec3(0.0f, 0.0f, 1.0f));
    }
  }

  drawLeaderboard(window, scale, players);

  glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
  std::string game_time = scene->time.ToString();
  float w = fr->TextWidth(game_time, 0.75 * scale);

  float x_left = width - (130 * scale);
  float x_leftNDC = (x_left / width * 2) - 1;

  float y_bot = height - (60 * 0.75 * scale);
  float y_botNDC = (y_bot / height * 2) - 1;

  glBegin(GL_QUADS);

  glVertex2f(1, 1);
  glVertex2f(x_leftNDC, 1);
  glVertex2f(x_leftNDC, y_botNDC);
  glVertex2f(1, y_botNDC);

  glEnd();

  fr->RenderText(width, height, game_time, width - (w + (15 * scale)),
                 height - (48 * 0.75 * scale), 0.75f * scale,
                 glm::vec3(1.0f, 0.0f, 0.0f));

  // minimap stuff
  int map_size = (width / 5 > 250) ? 250 : width / 4;
  glViewport(10, height - map_size + 10, map_size, map_size);

  drawMinimap();

  for (auto e : scene->networkGameThings) {
    auto play = e.second;
    if (dynamic_cast<Player*>(play) != nullptr) {
      Player* player = dynamic_cast<Player*>(play);
      glm::vec3 position = player->transform.position;
      glColor3f(0.0f, 0.0f, 1.0f);
      glPointSize(10);
      glBegin(GL_POINTS);
      glVertex3f(
          position[0] / map_size, -position[2] / map_size,
          0.0f);  // TODO: get minimap coordinates based off player position
      glEnd();
    }
  }

  glViewport(0, 0, width, height);

  drawCountdown();

  gameOver();

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}

void HUD::drawLeaderboard(GLFWwindow* window, float scale,
                          std::map<std::string, Player*> players) {
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  int size = (width / 10 > 250) ? 250 : width / 10;
  int x = 10;
  int y = 50;

  Player* player;
  std::string str;
  for (auto p : players) {
    str = p.first;
    player = p.second;
    Timer time = player->time;
    str += " " + time.ToString();

    glViewport(x, y, size, size);

    // Camera viewport;

    glBegin(GL_TRIANGLES);
    glColor4f(0.0, 0.0, 0.0, 0.5);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(0, 1);
    glEnd();

    glViewport(x - (2 * scale), y - (10 * scale), size, size);
    drawIcon("neutral");
    
    glViewport(0, 0, width, height);

    // Draw highlight
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

    // screen coordinates
    float x_left = x;
    float x_right = x + size;
    float y_top = y - 27 + (60 * 0.2 * scale);
    float y_bot = y - 27;

    // convert to normalized device coordinate
    float x_leftNDC = (x_left / width * 2) - 1;
    float x_rightNDC = (x_right / width * 2) - 1;
    float y_topNDC = (y_top / height * 2) - 1;
    float y_botNDC = (y_bot / height * 2) - 1;

    // render the text background
    glBegin(GL_QUADS);

    glVertex2f(x_rightNDC, y_topNDC);
    glVertex2f(x_leftNDC, y_topNDC);
    glVertex2f(x_leftNDC, y_botNDC);
    glVertex2f(x_rightNDC, y_botNDC);

    glEnd();

    fr->RenderText(width, height, str, x + (4 * scale), y - 20, 0.2 * scale,
                   glm::vec3(1.0, 1.0, 1.0));

    x += (size + (25 * scale));
  }
}

void HUD::drawIcon(std::string icon) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  icons[icon].bindgl();
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

}

void HUD::drawMinimap() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  map.bindgl();
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

  glDisable(GL_BLEND);
}

void HUD::update() {
  double nowTime = glfwGetTime();
  float delta = nowTime - lastTime;
  lastTime = nowTime;

  timeOnFrame += delta;
  if (timeOnFrame >= 0.08) {
    index++;
    timeOnFrame = 0;
  }

  if (index == frames.size()) {
    scene->gameStart = true;
  }
}

void HUD::drawCountdown() {
  if (!scene->gameStart) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Texture curFrame = frames[index];
    curFrame.bindgl();
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

    glDisable(GL_BLEND);

    update();
  }
}

void HUD::gameOver() {
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float scale = static_cast<float>(width) / static_cast<float>(800);
  if (scene->time.time <= 0.1) {
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    float w = fr->TextWidth("Time's Up!", offset * scale);
    fr->RenderText(width, height, "Time's Up!", (width / 2) - (w / 2),
                   height / 2, offset * scale, glm::vec3(1, 0, 0));
    offset--;
    if (offset < 1) offset = 1;
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
  }
}
