#include "client/graphics/Start.h"

void Start::update(float delta) {
  timeElapsed += delta;
  offset += 1.0f / float(15000);
  if (timeElapsed >= 1.0 && renderText) {
    timeElapsed = 0.0f;
    renderText = false;
  } else if (timeElapsed >= 0.5 && !renderText) {
    timeElapsed = 0.0f;
    renderText = true;
  }

  if (Input::GetInputState(InputAction::Enter) == InputState::Press) {
    gameStart = true;
  }
}

void Start::draw() { 
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    drawBackground(); 
    GLFWwindow* window = glfwGetCurrentContext();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float scale = float(width) / float(800);
    std::string name = "Tag Guys";
    float n_width = fr.TextWidth(name, 2 * scale);
    fr.RenderText(window, name, (width / 2.0f) - (n_width / 2.0f) - 25.0f,
                  (height) - (48 * 4 * scale), 2*scale, glm::vec3(0.0f, 0.0f, 0.0f));
    if (renderText) {
        std::string prompt = "Press enter to start game";
        float p_width = fr.TextWidth(prompt, 0.5 * scale);
        fr.RenderText(window, prompt, (width / 2.0f) - (p_width / 2.0f) - 25.0f,
                      (height / 2.0f) - (48 * 0.5 * scale), 0.5 * scale,
                      glm::vec3(0.0f, 0.0f, 0.0f));
    }
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

void Start::drawBackground() {
  background.bindgl();
  glEnable(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);

  glTexCoord2f(1 + offset, 0);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glTexCoord2f(0 + offset, 0);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glTexCoord2f(0 + offset, 1);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glTexCoord2f(1 + offset, 1);
  glVertex3f(1.0f, -1.0f, 0.0f);

  glEnd();

  glDisable(GL_TEXTURE_2D);
}