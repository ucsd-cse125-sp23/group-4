/**************************************************
 * Texture.h
 * representation of an image to be rendered
 * interacts with gl methods directly
 * should be composed into a Material.h
 * and the shader needs to have a gSampler uniform
 * see here for reference https://learnopengl.com/Getting-started/Textures
 * also here https://ogldev.org/www/tutorial16/tutorial16.html
 *****************************************************/

#pragma once

#include <stdio.h>

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "client/graphics/core.h"

class Texture {
 public:
  GLuint textureID;

  void init(const char* filename);

  void bindgl();
};
