#include "AssimpMesh.h"

AssimpMesh::AssimpMesh()
    : isUpdated(false), isLoaded(false), isLoaded2(false) {}

void AssimpMesh::gl_load() {
  // if (isLoaded) {
  //   return;
  // }

  // glGenVertexArrays(1, &VAO);
  // glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);

  // glBindVertexArray(VAO);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
  // &vertices[0],
  //              GL_STATIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned
  // int),
  //              &indices[0], GL_STATIC_DRAW);

  // // vertex positions
  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       reinterpret_cast<void*>(0));
  // // vertex normals
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       reinterpret_cast<void*>(offsetof(Vertex, normal)));
  // // bone index
  // glEnableVertexAttribArray(2);
  // glVertexAttribPointer(2, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex),
  //                       reinterpret_cast<void*>(offsetof(Vertex, boneInds)));
  // // bone weights
  // glEnableVertexAttribArray(3);
  // glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
  //                       reinterpret_cast<void*>(offsetof(Vertex,
  //                       boneWeights)));

  // glBindVertexArray(0);
  // isLoaded = true;
}

void AssimpMesh::gl_delete() {
  // if (!isLoaded) {
  //   return;
  // }

  // // TODO:
}

void AssimpMesh::draw() {
  if (!isLoaded2) {
    gl_load2();
  } else if (isUpdated) {
    gl_update2();
  }
  isUpdated = false;

  glBindVertexArray(VAO2);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void AssimpMesh::gl_load2() {
  glGenVertexArrays(1, &VAO2);
  glGenBuffers(1, &VBO2P);
  glGenBuffers(1, &VBO2N);
  glGenBuffers(1, &VBO2UV);
  glGenBuffers(1, &EBO2);
  isLoaded2 = true;

  glBindVertexArray(VAO2);

  glBindBuffer(GL_ARRAY_BUFFER, VBO2P);
  glBufferData(GL_ARRAY_BUFFER, outPos.size() * sizeof(glm::vec3), &outPos[0],
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

  glBindBuffer(GL_ARRAY_BUFFER, VBO2N);
  glBufferData(GL_ARRAY_BUFFER, outNorm.size() * sizeof(glm::vec3), &outNorm[0],
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

  glBindBuffer(GL_ARRAY_BUFFER, VBO2UV);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0],
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void AssimpMesh::gl_update2() {
  glBindVertexArray(VAO2);

  glBindBuffer(GL_ARRAY_BUFFER, VBO2P);
  glBufferData(GL_ARRAY_BUFFER, outPos.size() * sizeof(glm::vec3), &outPos[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, VBO2N);
  glBufferData(GL_ARRAY_BUFFER, outNorm.size() * sizeof(glm::vec3), &outNorm[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void AssimpMesh::gl_delete2() {
  glDeleteBuffers(1, &VAO2);
  glDeleteBuffers(1, &VBO2P);
  glDeleteBuffers(1, &VBO2N);
  glDeleteBuffers(1, &VBO2UV);
}
