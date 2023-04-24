#include "AssimpNode.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

AssimpNode::AssimpNode(unsigned int id)
        : id(id), parent(nullptr), localTransform(1.0f) {
    name = std::to_string(id);
}

void AssimpNode::imGui() {
    ImGui::Text("id    : %u %s", id, name.c_str());
    ImGui::Text("parent: %u %s",
        parent ? parent->id : 0,
        parent ? parent->name.c_str() : "none");
    if(meshes.size() > 0) {
        if (ImGui::TreeNode((void*)(intptr_t)0, "Meshes (%lu)", meshes.size())) {
            for(int i = 0; i < meshes.size(); i++) {
                if (ImGui::TreeNode((void*)(intptr_t)i, "Mesh %d", i)) {
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }
    if(children.size() > 0) {
        if (ImGui::TreeNode((void*)(intptr_t)0, "Children (%lu)", children.size())) {
            for(int i = 0; i < children.size(); i++) {
                if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i)) {
                    children[i]->imGui();
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }
}
