#include "AssimpMesh.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtx/string_cast.hpp>

glm::mat4 AssimpJoint::update() { return matWorldTransform * matBindingInv; }

AssimpMesh::AssimpMesh()
    : node(nullptr), isLoaded(false), isLoaded2(false) {
  for (int i = 0; i < MAX_BONES; i++) {
    matBindingInvs[i] = glm::mat4(1.0f);
  }
}

void AssimpMesh::update() {
  for (int i = 0; i < joints.size(); i++) {
    matBindingInvs[i] = joints[i]->update();
  }

  for (int i = 0; i < vertices.size(); i++) {
    Vertex& v = vertices[i];
    glm::vec4 animPos = glm::vec4(0.0);
    glm::vec4 animNorm = glm::vec4(0.0);
    glm::vec4 vPos4 = glm::vec4(v.position.x, v.position.y, v.position.z, 1.0);
    glm::vec4 vNorm4 = glm::vec4(v.normal.x, v.normal.y, v.normal.z, 0.0);

    for (int b = 0; b < 4; b++) {
      glm::vec4 bPos4 =
          matBindingInvs[v.boneInds[b]] * vPos4 * v.boneWeights[b];
      glm::vec4 bNorm4 =
          matBindingInvs[v.boneInds[b]] * vNorm4 * v.boneWeights[b];
      animPos = animPos + bPos4;
      animNorm = animNorm + bNorm4;
    }
    animPos = animPos / animPos[3];
    animNorm = glm::normalize(animNorm);

    worldVerticies[i].position = glm::vec3(animPos.x, animPos.y, animPos.z);
    worldVerticies[i].normal = glm::vec3(animNorm.x, animNorm.y, animNorm.z);
  }
}

void AssimpMesh::draw(const glm::mat4& viewProjMtx, GLuint shader) {
  if (!isLoaded) {
    gl_load();
  }
  glm::mat4 m4(1.0f);
  glm::vec3 color(1.0f, 1.0f, 1.0f);
  glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false,
                     reinterpret_cast<const float*>(&viewProjMtx));
  glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,
                     reinterpret_cast<const float*>(&m4));
  glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

  glUniformMatrix4fv(glGetUniformLocation(shader, "bones"), joints.size(),
                     GL_FALSE, reinterpret_cast<const float*>(&matBindingInvs));

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void AssimpMesh::gl_load() {
  if (isLoaded) {
    return;
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(0));
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, normal)));
  // bone index
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, boneInds)));
  // bone weights
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, boneWeights)));

  glBindVertexArray(0);
  isLoaded = true;
}

void AssimpMesh::gl_delete() {
  if (!isLoaded) {
    return;
  }

  // TODO:
}

void AssimpMesh::imGui() {
  unsigned int numTreeNode = 0;

  ImGui::Text("nodeId: %u %s", node->id, node->name.c_str());
  ImGui::Text("vertex (%lu)", vertices.size());
  ImGui::SameLine();
  ImGui::Text("index (%lu)", indices.size());
  ImGui::Text("joints (%lu)", joints.size());
  if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)numTreeNode++),
                      "Verticies (%lu)", vertices.size())) {
    for (int i = 0; i < vertices.size(); i++) {
      if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)i), "Vertex %d",
                          i)) {
        ImGui::Text("Pos (%f,%f,%f)", vertices[i].position.x,
                    vertices[i].position.y, vertices[i].position.z);
        ImGui::Text("Norm(%f,%f,%f)", vertices[i].normal.x,
                    vertices[i].normal.y, vertices[i].normal.z);
        ImGui::Text("Bone Indices: %u,%u,%u,%u", vertices[i].boneInds.x,
                    vertices[i].boneInds.y, vertices[i].boneInds.z,
                    vertices[i].boneInds.w);
        ImGui::Text("Bone Weights: %f,%f,%f,%f", vertices[i].boneWeights.x,
                    vertices[i].boneWeights.y, vertices[i].boneWeights.z,
                    vertices[i].boneWeights.w);
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
  if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)numTreeNode++),
                      "Joints (%lu)", joints.size())) {
    for (int i = 0; i < joints.size(); i++) {
      if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)i),
                          "Joint %5d (%5lu) %s", i, joints[i]->weights.size(),
                          joints[i]->node->name.c_str())) {
        AssimpJoint* joint = joints[i];
        for (int j = 0; j < joint->weights.size(); j++) {
          ImGui::Text("MatBInv %s",
                      glm::to_string(joints[i]->matBindingInv).c_str());
          ImGui::Text("MatW    %s",
                      glm::to_string(joints[i]->matWorldTransform).c_str());
          if (ImGui::TreeNode("Vertex - Weight")) {
            ImGui::Text("vInd: %u", joint->weights[j].vertexInd);
            ImGui::SameLine();
            ImGui::Text("vWgt: %f", joint->weights[j].weight);
            ImGui::TreePop();
          }
        }
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
}

void AssimpMesh::draw() {
  gl_load2();
  glBindVertexArray(VAO2);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void AssimpMesh::gl_load2() {
  if (!isLoaded2) {
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);
    isLoaded2 = true;
  }

  glBindVertexArray(VAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);

  glBufferData(GL_ARRAY_BUFFER, worldVerticies.size() * sizeof(Vertex),
               &worldVerticies[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(0));
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, normal)));
  // uvs
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, uv)));

  glBindVertexArray(0);
}
