#include "AssimpNode.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtx/string_cast.hpp>

#include "client/graphics/AssimpMath.h"
#include "client/graphics/shader.h"

AssimpNode::AssimpNode(unsigned int id)
    : id(id), parent(nullptr), localTransform(1.0f) {
  name = std::to_string(id);
}

void AssimpNode::update(const glm::mat4& parentWorldTransform) {
  matWorldTransform = parentWorldTransform * this->animationTransform;
  for (int i = 0; i < joints.size(); i++) {
    joints[i]->matWorldTransform = matWorldTransform;
  }
  for (int i = 0; i < children.size(); i++) {
    children[i]->update(matWorldTransform);
  }
}

GLuint* AssimpNode::shader;
void AssimpNode::draw(const glm::mat4& viewProjMtx) {
  if (!shader) {
    GLuint shaderTemp =
        LoadShaders("assets/shaders/shader.vert", "assets/shaders/shader.frag");
    shader = new GLuint(shaderTemp);
  }

  glUseProgram(*shader);
  glm::vec3 color;
  if (!isMarked) {
    color = glm::vec3(1.0f, 1.0f, 1.0f);
  } else {
    color = glm::vec3(1.0f, 1.0f, 0.0f);
  }
  glUniformMatrix4fv(glGetUniformLocation(*shader, "viewProj"), 1, false,
                     reinterpret_cast<const float*>(&viewProjMtx));
  glUniformMatrix4fv(glGetUniformLocation(*shader, "model"), 1, GL_FALSE,
                     reinterpret_cast<const float*>(&matWorldTransform));
  glUniform3fv(glGetUniformLocation(*shader, "DiffuseColor"), 1, &color[0]);
  geometry.draw();
  for (auto c : children) {
    c->draw(viewProjMtx);
  }
}

void AssimpNode::imGui() {
  unsigned int numTreeNode = 0;
  ImGui::Text("id    : %u %s", id, name.c_str());
  ImGui::Text("parent: %u %s", parent ? parent->id : 0,
              parent ? parent->name.c_str() : "none");
  ImGui::Text("joints: %lu", joints.size());
  if (meshes.size() > 0) {
    if (ImGui::TreeNode((reinterpret_cast<void*>((intptr_t)numTreeNode++), "Meshes (%lu)",
                        meshes.size())) {
      for (int i = 0; i < meshes.size(); i++) {
        if (ImGui::TreeNode((void*)(intptr_t)i, "Mesh %d", i)) {
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  }
  if (children.size() > 0) {
    if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)numTreeNode++), "Children (%lu)",
                        children.size())) {
      for (int i = 0; i < children.size(); i++) {
        if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i)) {
          children[i]->imGui();
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  }

  ImGui::Text("MatLocal %s", glm::to_string(localTransform).c_str());
  ImGui::Text("MatAnim  %s", glm::to_string(animationTransform).c_str());
  ImGui::Text("MatAcc   %s", glm::to_string(accTransform).c_str());
  ImGui::Text("MatWorld %s", glm::to_string(matWorldTransform).c_str());
}
