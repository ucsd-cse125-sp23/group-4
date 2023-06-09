#pragma once

#include <string>
#include <vector>

#include "Window.h"
#include "client/graphics/Texture.h"

extern std::atomic<bool> loading_resources;

class Load {
 public:
  std::vector<Texture> frames;
  int index;
  float timeOnFrame;
  bool forward;
  double lastTime;
  float timeElapsed;

  Load() {
    index = 0;
    timeOnFrame = 0;
    forward = true;
    lastTime = glfwGetTime();
    timeElapsed = 0;

    loadFrames();
  }

  void loadFrames() {
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
    timeElapsed += delta;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

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
    glDisable(GL_DEPTH_TEST);

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

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);
  }
};
