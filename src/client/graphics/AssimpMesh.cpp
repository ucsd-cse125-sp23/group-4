#include "AssimpMesh.h"

AssimpMesh::AssimpMesh() : isUpdated(false), isLoaded(false) {}

AssimpMesh::~AssimpMesh() { delete texture; }

void AssimpMesh::draw() {
  if (!isLoaded) {
    gl_load();
  } else if (isUpdated) {
    gl_update();
  }
  isUpdated = false;

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void AssimpMesh::gl_load() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBOP);
  glGenBuffers(1, &VBON);
  glGenBuffers(1, &VBOUV);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBOP);
  glBufferData(GL_ARRAY_BUFFER, outPos.size() * sizeof(glm::vec4), &outPos[0],
               GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 16, static_cast<void*>(0));

  glBindBuffer(GL_ARRAY_BUFFER, VBON);
  glBufferData(GL_ARRAY_BUFFER, outNorm.size() * sizeof(glm::vec4), &outNorm[0],
               GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 16, static_cast<void*>(0));

  glBindBuffer(GL_ARRAY_BUFFER, VBOUV);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0],
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  isLoaded = true;
}

void AssimpMesh::gl_update() {
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBOP);
  glBufferData(GL_ARRAY_BUFFER, outPos.size() * sizeof(glm::vec4), &outPos[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, VBON);
  glBufferData(GL_ARRAY_BUFFER, outNorm.size() * sizeof(glm::vec4), &outNorm[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void AssimpMesh::gl_delete() {
  glDeleteBuffers(1, &VAO);
  glDeleteBuffers(1, &VBOP);
  glDeleteBuffers(1, &VBON);
  glDeleteBuffers(1, &VBOUV);
  isLoaded = false;
}
