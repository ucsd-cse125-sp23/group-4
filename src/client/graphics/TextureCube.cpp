#include "TextureCube.h"

#include "client/graphics/imported/stb_image.h"

void TextureCube::init() {
  /// order of sides:
  /// 0 right
  /// 1 left
  /// 2 top
  /// 3 bottom
  /// 4 front
  /// 5 back
  glGenTextures(1, &textureID);
  glBindTexture(target, textureID);
  // set the texture wrapping/filtering options (on the currently bound
  // texture object)
  glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapping);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapping);
  glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapping);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filtering);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filtering);

  // load and generate each texture tile
  for (unsigned int i = 0; i < 6; i++) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(cubeTileFilenames[i].c_str(), &width,
                                    &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(targetImage + i, 0, GL_RGB, width, height, 0, format,
                   GL_UNSIGNED_BYTE, data);
    } else {
      std::cerr << "Cannot open file: " << cubeTileFilenames[i].c_str()
                << std::endl;
    }

    stbi_image_free(data);
  }

  glBindTexture(target, 0);
}
