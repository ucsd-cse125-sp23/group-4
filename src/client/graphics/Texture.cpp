#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "client/graphics/imported/stb_image.h"

void Texture::init(const char* filename) {
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  // set the texture wrapping/filtering options (on the currently bound
  // texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // load and generate the texture
  int width, height, nrChannels;
  unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

  if (data) {
    if (nrChannels == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
    } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    }
    // glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Cannot open file: " << filename << std::endl;
  }

  stbi_image_free(data);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindgl() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
}
