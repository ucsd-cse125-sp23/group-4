#include "HUD.h"

#include "Window.h"

void HUD::draw(GLFWwindow* window) {
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  std::map<std::string, Player*> players;

  float scale = static_cast<float>(width) / static_cast<float>(800);

  for (auto& [_, e] : scene->networkGameThings) {
    if (dynamic_cast<Player*>(e) != nullptr) {
      Player* player = dynamic_cast<Player*>(e);
      glm::vec3 color;
      if (ind == scene->_myPlayerId) {
        color = glm::vec3(255.0 / 256.0, 0.0, 115.0 / 256.0);
      } else if (player->tagged) {
        color = glm::vec3(147.0 / 256.0, 0.0, 232.0 / 256.0);
      } else {
        color = glm::vec3(0.0, 0.0, 0.0);
      }
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
          size = (0.5 * scale) / (0.5 * abs(deltaD));
          if (size < 0.1 * scale)
            size = 0.1 * scale;
          else if (size > 0.5 * scale)
            size = 0.5 * scale;
        } else {
          size = 0.5 * scale;
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

  glDisable(GL_DEPTH_TEST);
  fr->RenderText(width, height, game_time, width - (w + (15 * scale)),
                 height - (48 * 0.75 * scale), 0.75f * scale,
                 glm::vec3(1.0f, 0.0f, 0.0f));

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
        glColor3f(255.0 / 256.0, 0.0f, 115.0 / 256.0);
      } else if (player->tagged) {
        glColor3f(147.0 / 256.0, 0.0, 232.0 / 256.0);
      } else {
        glColor3f(0.0, 0.0, 0.0);
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

  gameOver();

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}

void HUD::drawLeaderboard(GLFWwindow* window, float scale,
                          std::map<std::string, Player*> players) {
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

  x = bar_width / 2.5;
  y = bar_height / 2;
  for (auto it = players.rbegin(); it != players.rend(); it++) {
    str = it->first;
    player = it->second;

    glViewport(x, y, bar_width, bar_height);

    str = player->name;

    int score_s = player->score / 20.0;
    str += " " + std::to_string(score_s) + "s";

    glDisable(GL_DEPTH_TEST);

    // change text color of tagged player
    if (it->first == scene->_myPlayerId)
      fr->RenderText(bar_width, bar_height, str, bar_width / 2.5,
                     bar_height / 2, 0.3 * scale,
                     glm::vec3(255.0 / 256.0, 0.0, 115.0 / 256.0));
    else if (player->tagged)
      fr->RenderText(bar_width, bar_height, str, bar_width / 2.5,
                     bar_height / 2, 0.3 * scale,
                     glm::vec3(147.0 / 256.0, 0.0, 232.0 / 256.0));
    else
      fr->RenderText(bar_width, bar_height, str, bar_width / 2.5,
                     bar_height / 2, 0.3 * scale, glm::vec3(0.0, 0.0, 0.0));
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
    glDisable(GL_DEPTH_TEST);
    fr->RenderText(width, height, "Time's Up!", (width / 2) - (w / 2),
                   height / 2, offset * scale, glm::vec3(1, 0, 0));
    glEnable(GL_DEPTH_TEST);
    offset--;
    if (offset < 1) offset = 1;
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
  }
}
