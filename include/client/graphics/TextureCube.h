/**************************************************
 * TextureCube.h
 * Cubemap texture for skybox, uses 6 sides
 * https://www.youtube.com/watch?v=8sVvxeKI9Pk
 *****************************************************/

#pragma once

#include <stdio.h>

#include <glm/glm.hpp>

#include "client/graphics/Texture.h"

class TextureCube : public Texture {
 private:
  const std::string skyboxDir = "assets/image/skybox/";
  const std::string filetype = ".png";

  std::string cubeTileFilenames[6] = {skyboxDir + "cube_tile_0001" + filetype,
                                      skyboxDir + "cube_tile_0002" + filetype,
                                      skyboxDir + "cube_tile_0003" + filetype,
                                      skyboxDir + "cube_tile_0004" + filetype,
                                      skyboxDir + "cube_tile_0005" + filetype,
                                      skyboxDir + "cube_tile_0006" + filetype};

 public:
  TextureCube() {
    target = GL_TEXTURE_CUBE_MAP;
    targetImage = GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    filtering = GL_LINEAR;
    wrapping = GL_CLAMP_TO_EDGE;

    format = GL_RGB;
  }

  void init();
};
