#include "client/graphics/HUD.h"

void HUD::draw(GLFWwindow* window) {
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  std::map<std::string, Timer> player_times;

  float scale = float(width) / float(800);

  for (GameThing* e : scene->gamethings) {
    if (dynamic_cast<client::Player*>(e) != nullptr) {
      client::Player* player = dynamic_cast<client::Player*>(e);
      std::string name = player->name;
      player_times[name] = player->time;
      const unsigned char* cname =
          reinterpret_cast<const unsigned char*>(name.c_str());
      glm::vec4 position = glm::vec4(player->transform.position, 1.0f);
      glm::vec4 postProjectivePosition = scene->camera->GetViewProjectMtx() * position;
      glm::vec3 normalizedSpace =
          glm::vec3(postProjectivePosition) / postProjectivePosition[3];
      glm::vec2 windowSpace = ((glm::vec2(normalizedSpace) + 1.0f) / 2.0f) *
                                  glm::vec2(width, height) +
                              glm::vec2(0.0f, 0.0f);
      fr.RenderText(window, name, windowSpace[0], windowSpace[1], 0.5f * scale,
                    glm::vec3(0.0f, 0.0f, 1.0f));
    }
  }

  float y_pos = float(height) - (48 * 0.5f * scale);
  for (auto times : player_times) {
    std::string str = times.first;
    Timer time = times.second;
    str += " " + time.ToString();
    fr.RenderText(window, str, 10.0f, y_pos, 0.5f * scale,
                  glm::vec3(1.0f, 1.0f, 1.0f));
    y_pos -= 48 * 0.5f * scale;
  }

  std::string game_time = scene->time.ToString();
  float w = fr.TextWidth(game_time, scale);
  fr.RenderText(window, game_time, (width / 2.0f) - (w / 2.0f), height - (48 * scale),
                1.0f * scale,
                glm::vec3(1.0f, 0.0f, 0.0f));

  // minimap stuff
  int map_size = (width / 4 > 250) ? 250 : width / 4;
  glViewport(10, 10, map_size, map_size);
  glScissor(10, 10, map_size, map_size);
  glEnable(GL_SCISSOR_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_SCISSOR_TEST);

  for (GameThing* e : scene->gamethings) {
    if (dynamic_cast<client::Player*>(e) != nullptr) {
      client::Player* player = dynamic_cast<client::Player*>(e);
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

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}