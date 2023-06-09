#include "HUD.h"

#include "Window.h"

void HUD::draw(GLFWwindow* window) {
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  std::map<int, Player*> players;

  float scale_x = static_cast<float>(width) / static_cast<float>(800);
  float scale_y = static_cast<float>(height) / static_cast<float>(600);

  for (auto& [ind, e] : scene->networkGameThings) {
    if (dynamic_cast<Player*>(e) != nullptr) {
      Player* player = dynamic_cast<Player*>(e);
      auto name = player->name;
      players[ind] = player;
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

      float view = 1;
      if (normalizedSpace[2] >= -view && normalizedSpace[2] <= view) {
        // Scale the size of the text based on camera distance
        Camera* cam = scene->camera;
        float deltaD = 10 - cam->GetDistance();
        float size;
        if (deltaD < 0) {
          size = (0.5 * scale_y) / (0.5 * abs(deltaD));
          if (size < 0.1 * scale_y)
            size = 0.1 * scale_y;
          else if (size > 0.5 * scale_y)
            size = 0.5 * scale_y;
        } else {
          size = 0.5 * scale_y;
        }
        glDisable(GL_DEPTH_TEST);
        fr->RenderText(width, height, name, windowSpace[0], windowSpace[1],
                       size, glm::vec3(0.0f, 0.0f, 1.0f));
        glEnable(GL_DEPTH_TEST);
      }
    }
  }

  if (Window::_debugmode) {
    // draw FPS (probably can be shown in imgui instead tbh)
    int top = 65;
    std::string fps_text = std::to_string(Window::fps) + " FPS";
    std::string ups_text = std::to_string(Window::ups) + " UPS";

    glm::vec2 wpos = glm::vec2(width - 40, height - top);
    float size = 0.2f;

    glDisable(GL_DEPTH_TEST);
    fr_dev->RenderText(width, height, fps_text, wpos.x, wpos.y, size,
                       glm::vec3(1.0f, 0.0f, 0.0f));
    fr_dev->RenderText(width, height, ups_text, wpos.x, wpos.y - 12, size,
                       glm::vec3(1.0f, 0.0f, 0.0f));
    glEnable(GL_DEPTH_TEST);
  }

  drawLeaderboard(window, scale_x, players);

  if (scene->gameStart) drawTime();

  // minimap stuff
  int map_height = (width / 5 > 250) ? 250 : width / 4;
  int map_width = (map_height * 1920) / 1080;
  glViewport(-(map_width / 4) + 10, height - map_height - 10,
             (map_height * 1920) / 1080, map_height);

  drawMinimap();

  int world_height = 130;
  int world_width = (world_height * 1920) / 1080;
  for (auto& [i, e] : scene->networkGameThings) {
    if (dynamic_cast<Player*>(e) != nullptr) {
      Player* player = dynamic_cast<Player*>(e);
      glm::vec3 position = player->transform.position;

      if (i == scene->_myPlayerId) {
        glColor3f(0.0f, 1.0f, 0.0f);
      } else if (player->tagged) {
        glColor3f(251.0 / 256.0, 133.0 / 256.0, 0.0 / 256.0);
      } else {
        glColor3f(137.0 / 256.0, 177.0 / 256.0, 185.0 / 256.0);
      }
      glPointSize(10);
      glBegin(GL_POINTS);
      glVertex2f(-(position[2] - 23) / world_width,
                 -((position[0] - 33)) / world_height);
      glEnd();
    }
  }
  glEnable(GL_DEPTH_TEST);

  glViewport(0, 0, width, height);

  drawCountdown();

  if (scene->gameStart) gameOver();

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}

void HUD::drawTime() {
  glDisable(GL_DEPTH_TEST);
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float scale_x = static_cast<float>(width) / static_cast<float>(800);
  float scale_y = static_cast<float>(height) / static_cast<float>(600);

  int timer_x = 0.7 * width;
  int timer_y = 0.75 * height;
  int size_x = width - timer_x;
  int size_y = height - timer_y;
  glViewport(timer_x, timer_y, size_x, size_y);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  timer.bindgl();
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

  std::string game_time = scene->time.ToString();

  fr->RenderText(size_x, size_y, game_time, size_x / 6, size_y / 2.25,
                 0.65f * scale_y, glm::vec3(1.0f, 0.0f, 0.0f));

  glViewport(0, 0, width, height);
}

void HUD::drawLeaderboard(GLFWwindow* window, float scale,
                          std::map<int, Player*> players) {
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  int bar_width = (width / 4 > 800) ? 800 : width / 4;
  int bar_height = (height / 5 > 200) ? 200 : height / 5;
  int x = 0;
  int y = 0;

  Player* player;
  std::string str;
  for (auto it = scene->skins.rbegin(); it != scene->skins.rend(); it++) {
    glViewport(x, y, bar_width, bar_height);

    drawBar(it->second);

    glViewport(0, 0, width, height);

    y += (bar_height / 1.5);
  }

  x = 0;
  y = 0;
  for (auto it = players.rbegin(); it != players.rend(); it++) {
    player = it->second;
    glViewport(x, y, bar_width, bar_height);

    int i, steps = 36;
    float a = 0.0, b = 0.0, r = 1.0, phi, dphi = 2. * M_PI / (float)(steps);
    if (it->first == scene->_myPlayerId) {
      glViewport(x + bar_width / 5, y + bar_height / 3, bar_width / 5,
                 bar_width / 5);
      glColor3f(0.0f, 1.0f, 0.0f);
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0xff);
      glLineWidth(5);

      glBegin(GL_LINE_LOOP);
      for (i = 0, phi = 0.0; i < steps; i++, phi += dphi)
        glVertex3f(a + r * cos(phi), b + r * sin(phi), 0.0);

      glEnd();

      glDisable(GL_LINE_STIPPLE);

      glViewport(x, y, bar_width, bar_height);
    }

    str = player->name;
    int score_s = player->score / 20.0;
    str += " " + std::to_string(score_s) + "s";

    glDisable(GL_DEPTH_TEST);

    // change text color of tagged player
    if (it->first == scene->_myPlayerId)
      fr->RenderText(
          bar_width, bar_height, str, bar_width / 2.5, bar_height / 2,
          0.3 * scale,
          glm::vec3(0.0, 1.0, 0.0)); 
    else if (player->tagged)
      fr->RenderText(
          bar_width, bar_height, str, bar_width / 2.5, bar_height / 2,
          0.3 * scale,
          glm::vec3(251.0 / 256.0, 133.0 / 256.0, 0.0 / 256.0));  // orange
    else
      fr->RenderText(
          bar_width, bar_height, str, bar_width / 2.5, bar_height / 2,
          0.3 * scale,
          glm::vec3(137.0 / 256.0, 177.0 / 256.0, 185.0 / 256.0));  // gray
    glEnable(GL_DEPTH_TEST);

    y += (bar_height / 1.5);

    glViewport(0, 0, width, height);
  }
}

void HUD::drawBar(std::string skin) {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  player_bars[skin].bindgl();
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
  glEnable(GL_DEPTH_TEST);
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
  glDisable(GL_DEPTH_TEST);
  if (!scene->gameStart && index < frames.size()) {
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
  glEnable(GL_DEPTH_TEST);
}

void HUD::gameOver() {
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float scale_x = static_cast<float>(width) / static_cast<float>(800);
  float scale_y = static_cast<float>(height) / static_cast<float>(600);
  if (scene->time.time <= 0.1) {
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    float w = fr->TextWidth("Time's Up!", offset * scale_y);
    glDisable(GL_DEPTH_TEST);
    fr->RenderText(width, height, "Time's Up!", (width / 2) - (w / 2),
                   height / 2, offset * scale_y, glm::vec3(1, 0, 0));
    glEnable(GL_DEPTH_TEST);
    offset--;
    if (offset < 1) offset = 1;
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
  }
}

void HUD::reset() {
  offset = 10;
  index = 0;
  lastTime = glfwGetTime();
  index = 0;
  timeOnFrame = 0;

  for (auto& thing : scene->localGameThings) thing->update(3);
  for (auto& [_, thing] : scene->networkGameThings) thing->update(3);
}
