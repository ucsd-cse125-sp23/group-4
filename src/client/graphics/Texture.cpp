#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "client/graphics/imported/stb_image.h"

void Texture::init(const char* filename) {
  glGenTextures(1, &textureID);
  glBindTexture(target, textureID);
  // set the texture wrapping/filtering options (on the currently bound
  // texture object)
  glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapping);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapping);
  glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapping);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filtering);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filtering);

  // load and generate the texture
  int width, height, nrChannels;
  unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(targetImage, 0, GL_RGB, width, height, 0,
                 format, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D); // no mipmaps rn
  } else {
    std::cerr << "Cannot open file: " << filename << std::endl;
  }

  stbi_image_free(data);
  glBindTexture(target, 0);
}

void Texture::bindgl() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(target, textureID);
}
