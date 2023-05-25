#pragma once

#include <string>
#include <vector>

#include "client/graphics/Texture.h"
#include "client/graphics/UserState.h"

class Load {
 public:
  std::vector<Texture> frames;
  int index;
  float timeOnFrame;
  bool forward;
  double lastTime;

  explicit Load() {
    index = 0;
    timeOnFrame = 0;
    forward = true;
    lastTime = glfwGetTime();

    for (int i = 0; i < 24; i++) {
      Texture frame;
      std::string filename = "assets/image/tagguys/frame_" + std::to_string(i) +
                             "_delay-0.08s.png";
      frame.init(filename.c_str());
      frames.push_back(frame);
    }
  }

  void update() {
    double nowTime = glfwGetTime();
    float delta = nowTime - lastTime;
    lastTime = nowTime;
    
    timeOnFrame += delta;
    if (timeOnFrame >= 0.05) {
      if (forward) {
        index++;
      } else {
        index--;
      }

      if (index == frames.size()) {
        forward = false;
        index--;
      } else if (index == -1) {
        forward = true;
        index++;
      }

      timeOnFrame = 0;
    }
  }

  void draw() {
    GLFWwindow* window = glfwGetCurrentContext();
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glColor3f(213.0f / 256.0f, 236.0f / 256.0f, 241.0f / 256.0f);
    glBegin(GL_QUADS);

    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);

    glEnd();

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
  }
};
