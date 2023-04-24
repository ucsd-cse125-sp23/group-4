#include "AssimpMesh.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

glm::mat4 AssimpJoint::update() {
    return glm::mat4(1.0f);
}

AssimpMesh::AssimpMesh() 
        : node(nullptr), isLoaded(false) { }

void AssimpMesh::update() {
    for(int i = 0; i < joints.size(); i++) {
        AssimpJoint* joint = joints[i];
        matBindingInvs[i] = joint->update();
    }
}

void AssimpMesh::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    if(!isLoaded) {
        gl_load();
    }
    glm::mat4 m4(1.0f);
    glm::vec3 color(1.0f, 1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&m4);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    if(joints.size() > 0) {
        glUniformMatrix4fv(glGetUniformLocation(shader, "bones"), joints.size(), GL_FALSE, (float*)&matBindingInvs);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void AssimpMesh::gl_load() {
    if(isLoaded) {
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // bone index
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneInds));
    // bone weights
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));

    glBindVertexArray(0);
    isLoaded = true;
}

void AssimpMesh::gl_delete() {
    if(!isLoaded) {
        return;
    }

    // TODO:
}

void AssimpMesh::imGui() {
    ImGui::Text("nodeId: %u %s", node->id, node->name.c_str());
    ImGui::Text("vertex (%lu)", joints.size());
    ImGui::SameLine();
    ImGui::Text("index  (%lu)", indices.size());
    ImGui::Text("joints (%lu)", joints.size());
}
