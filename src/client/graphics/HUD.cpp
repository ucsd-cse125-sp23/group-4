#include "HUD.h"

#include "Window.h"

void HUD::init() {
  for (int i = 0; i < 40; i++) {
    Texture frame;
    std::string filename =
        "assets/image/countdown/frame_" + std::to_string(i) + "_delay-0.1s.png";
    frame.init(filename.c_str());
    frames.push_back(frame);
  }
  timer.init("assets/UI/IMG_2658.PNG");

  player_bars["trash panda"].init("assets/UI/bar-racoon.png");
  player_bars["bee"].init("assets/UI/bar-bee.png");
  player_bars["avocado"].init("assets/UI/bar-avocado.png");
  player_bars["duck"].init("assets/UI/bar-duck.png");
  player_bars["cat"].init("assets/UI/bar-cat.png");
  player_bars["unicorn"].init("assets/UI/bar-unicorn.png");
  player_bars["waffle"].init("assets/UI/bar-waffle.png");
  player_bars["bear"].init("assets/UI/Group 6 (1).png");
}

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
                       size, color);
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
  if (Window::phase == GamePhase::GameOver) {
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
