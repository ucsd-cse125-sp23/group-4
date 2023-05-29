#include "AssimpMesh.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtx/string_cast.hpp>

AssimpMesh::AssimpMesh()
    : isUpdated(false), isLoaded(false), isLoaded2(false) {}

void AssimpMesh::draw(const glm::mat4& viewProjMtx, GLuint shader) {
  // if (!isLoaded) {
  //   gl_load();
  // }
  // glm::mat4 m4(1.0f);
  // glm::vec3 color(1.0f, 1.0f, 1.0f);
  // glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false,
  //                    reinterpret_cast<const float*>(&viewProjMtx));
  // glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,
  //                    reinterpret_cast<const float*>(&m4));
  // glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

  // glUniformMatrix4fv(glGetUniformLocation(shader, "bones"), joints.size(),
  //                    GL_FALSE, reinterpret_cast<const
  //                    float*>(&matBindingInvs));

  // glBindVertexArray(VAO);
  // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  // glBindVertexArray(0);
}

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

void AssimpMesh::imGui() {
  // unsigned int numTreeNode = 0;

  // ImGui::Text("nodeId: %u %s", node->id, node->name.c_str());
  // ImGui::Text("vertex (%lu)", vertices.size());
  // ImGui::SameLine();
  // ImGui::Text("index (%lu)", indices.size());
  // ImGui::Text("joints (%lu)", joints.size());
  // if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)numTreeNode++),
  //                     "Verticies (%lu)", vertices.size())) {
  //   for (int i = 0; i < vertices.size(); i++) {
  //     if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)i), "Vertex %d",
  //                         i)) {
  //       ImGui::Text("Pos (%f,%f,%f)", vertices[i].position.x,
  //                   vertices[i].position.y, vertices[i].position.z);
  //       ImGui::Text("Norm(%f,%f,%f)", vertices[i].normal.x,
  //                   vertices[i].normal.y, vertices[i].normal.z);
  //       ImGui::Text("Bone Indices: %u,%u,%u,%u", vertices[i].boneInds.x,
  //                   vertices[i].boneInds.y, vertices[i].boneInds.z,
  //                   vertices[i].boneInds.w);
  //       ImGui::Text("Bone Weights: %f,%f,%f,%f", vertices[i].boneWeights.x,
  //                   vertices[i].boneWeights.y, vertices[i].boneWeights.z,
  //                   vertices[i].boneWeights.w);
  //       ImGui::TreePop();
  //     }
  //   }
  //   ImGui::TreePop();
  // }
  // if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)numTreeNode++),
  //                     "Joints (%lu)", joints.size())) {
  //   for (int i = 0; i < joints.size(); i++) {
  //     if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)i),
  //                         "Joint %5d (%5lu) %s", i,
  //                         joints[i]->weights.size(),
  //                         joints[i]->node->name.c_str())) {
  //       AssimpJoint* joint = joints[i];
  //       for (int j = 0; j < joint->weights.size(); j++) {
  //         ImGui::Text("MatBInv %s",
  //                     glm::to_string(joints[i]->matBindingInv).c_str());
  //         ImGui::Text("MatW    %s",
  //                     glm::to_string(joints[i]->matWorldTransform).c_str());
  //         if (ImGui::TreeNode("Vertex - Weight")) {
  //           ImGui::Text("vInd: %u", joint->weights[j].vertexInd);
  //           ImGui::SameLine();
  //           ImGui::Text("vWgt: %f", joint->weights[j].weight);
  //           ImGui::TreePop();
  //         }
  //       }
  //       ImGui::TreePop();
  //     }
  //   }
  //   ImGui::TreePop();
  // }
}

void AssimpMesh::draw() {
  if (!isLoaded2 || isUpdated) {
    gl_load2();
    isUpdated = false;
  }

  glBindVertexArray(VAO2);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void AssimpMesh::gl_load2() {
  if (!isLoaded2) {
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2P);
    glGenBuffers(1, &VBO2N);
    glGenBuffers(1, &VBO2UV);
    glGenBuffers(1, &EBO2);
    isLoaded2 = true;
  }

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
