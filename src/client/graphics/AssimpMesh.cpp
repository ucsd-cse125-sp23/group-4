#include "AssimpMesh.h"

#include <glm/gtx/string_cast.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

glm::mat4 AssimpJoint::update() {
    return matWorldTransform * matBindingInv;
}

AssimpMesh::AssimpMesh() 
        : node(nullptr), isLoaded(false), isDrawMesh(true) {
    matBindingInvs[0] = glm::mat4(1.0f);
}

void AssimpMesh::setDraw(bool useMesh) {
    if(!useMesh) { 
        isDrawMesh = false;
        return;
    }

    for(int i = 0; i < joints.size(); i++) {
        matBindingInvs[i] = glm::mat4(1.0f);
    }
    isDrawMesh = true;
}

void AssimpMesh::update() {
    if(isDrawMesh) {
        return;
    }

    for(int i = 0; i < joints.size(); i++) {
        matBindingInvs[i] = joints[i]->update();
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
    long numTreeNode = 0;

    ImGui::Text("nodeId: %u %s", node->id, node->name.c_str());
    ImGui::Text("vertex (%lu)", vertices.size());
    ImGui::SameLine();
    ImGui::Text("index (%lu)", indices.size());
    ImGui::Text("joints (%lu)", joints.size());
    if (ImGui::TreeNode((void*)(intptr_t)numTreeNode++, "Verticies (%lu)", vertices.size())) {
        for(int i = 0; i < vertices.size(); i++) {
            if (ImGui::TreeNode((void*)(intptr_t)i, "Vertex %d", i)) {
                ImGui::Text("Pos (%f,%f,%f)", 
                    vertices[i].position.x,
                    vertices[i].position.y,
                    vertices[i].position.z);
                ImGui::Text("Norm(%f,%f,%f)",
                    vertices[i].normal.x,
                    vertices[i].normal.y,
                    vertices[i].normal.z);
                ImGui::Text("Bone Indices: %u,%u,%u,%u", 
                    vertices[i].boneInds.x,
                    vertices[i].boneInds.y,
                    vertices[i].boneInds.z,
                    vertices[i].boneInds.w);
                ImGui::Text("Bone Weights: %f,%f,%f,%f", 
                    vertices[i].boneWeights.x,
                    vertices[i].boneWeights.y,
                    vertices[i].boneWeights.z,
                    vertices[i].boneWeights.w);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode((void*)(intptr_t)numTreeNode++, "Joints (%lu)", joints.size())) {
        for(int i = 0; i < joints.size(); i++) {
            if (ImGui::TreeNode((void*)(intptr_t)i, "Joint %5d (%5lu) %s", i, joints[i]->weights.size(), joints[i]->node->name.c_str())) {
                AssimpJoint* joint = joints[i];
                for(int j = 0; j < joint->weights.size(); j++) {
                    ImGui::Text("MatBInv %s", glm::to_string(joints[i]->matBindingInv).c_str());
                    ImGui::Text("MatW    %s", glm::to_string(joints[i]->matWorldTransform).c_str());
                    if(ImGui::TreeNode("Vertex - Weight")) {
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
