#include "Start.h"

void Start::update(float delta, message::UserStateUpdate& ourPlayerUpdates) {
  Scene::update(delta, ourPlayerUpdates);
  timeElapsed += delta;
  offset += (0.25 * delta);
  if (timeElapsed >= 1.0 && renderText) {
    timeElapsed = 0.0f;
    renderText = false;
  } else if (timeElapsed >= 0.5 && !renderText) {
    timeElapsed = 0.0f;
    renderText = true;
  }

  timeOnFrame += delta;
  if (timeOnFrame >= 0.1 && index < frames.size() - 1) {
    index++;
    timeOnFrame = 0;
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
  float scale = static_cast<float>(width) / static_cast<float>(800);
  drawName();
  if (renderText) {
    std::string prompt = "Press enter to start game";
    float p_width = fr->TextWidth(prompt, 0.5 * scale);
    fr->RenderText(width, height, prompt,
                   (width / 2.0f) - (p_width / 2.0f) - 25.0f,
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

void Start::drawName() {
  GLFWwindow* window = glfwGetCurrentContext();
  int width, height;
  glfwGetWindowSize(window, &width, &height);

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
