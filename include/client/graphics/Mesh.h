/**************************************************
Mesh is an abstract class for a 3D rendered object.
*****************************************************/

#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "client/graphics/shader.h"

class Mesh {
 public:
  GLenum mode = GL_TRIANGLES;     // the cookboook for glDrawElements
  int count;                      // number of elements (indices) to draw
  GLenum type = GL_UNSIGNED_INT;  // type of the index array

  GLuint vao;  // vertex array object a.k.a. geometry spreadsheet
  std::vector<GLuint> buffers;  // data storage
                                // position,
                                // normal,
                                // uv,
                                // indices

  virtual void init() {}
  void init(std::string str) { init(str.c_str()); }
  virtual void init(const char* s) {}

  virtual void draw(void) {
    if (!vao) {
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      // 0th attribute: position
      glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

      // 1st attribute: normal
      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

      // 2nd attribute: uv (textures)
      glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

      // indices
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
    } else {
      glBindVertexArray(vao);
    }
    // unbind the buffers, vao
    glDrawElements(mode, count, type, 0);  // uses indices
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void creategl() {
    buffers.resize(4);
    glGenBuffers(4, &buffers[0]);
  }

  void bindgl(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
              std::vector<glm::vec2> uvs, std::vector<unsigned int> indices) {
    unsigned int n = indices.size();  // #(triangles)*3

    count = n;

    if (count == 0) return;

    // 0th attribute: position
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
                 &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

    // 1st attribute: normal
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
                 &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

    // 2nd attribute: uv (textures)
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), &uvs[0],
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * n, &indices[0],
                 GL_STATIC_DRAW);

    // unbind the buffers, vao
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // don't unbuffer me!
  }

  void bindgl(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
              std::vector<unsigned int> indices) {
    std::vector<glm::vec2> uvsDefault;
    for (auto v : vertices) {
      uvsDefault.push_back(glm::vec2(0));
    }

    bindgl(vertices, normals, uvsDefault, indices);
  }

  void cleargl() {
    // Delete the VBOs and the VAO.
    if(vao) glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &buffers[0]);
    glDeleteBuffers(1, &buffers[1]);
    glDeleteBuffers(1, &buffers[2]);
    glDeleteBuffers(1, &buffers[3]);
  }
};
