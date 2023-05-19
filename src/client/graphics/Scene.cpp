/**************************************************
Scene.cpp contains the implementation of the draw command
Renders all objects with DFS tree traversal
adapted from CSE 167 - Matthew
*****************************************************/
#include "Scene.h"

#include <GL/freeglut.h>
#include <Player.h>
#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <stack>

#include "Scene.inl"  // The scene init definition

using glm::mat4x4;
using glm::vec3;

bool Scene::_freecam = false;
bool Scene::_gizmos = false;
SceneResourceMap Scene::_globalSceneResources = SceneResourceMap();

void Scene::initFromServer(int myid) {
  // TODO(matthew) make this better
  for (auto e : gamethings) {
    if (e->netId == myid) {
      setToUserFocus(e);
      return;
    }
  }
}

void Scene::setToUserFocus(GameThing* t) {
  t->isUser = true;
  if (dynamic_cast<Player*>(t) != nullptr) {
    Player* player = dynamic_cast<Player*>(t);
    player->camera = camera;  // give a reference to the game camera
  }
  t->childnodes.push_back(camera);
}

UserState Scene::update(float delta) {
  UserState ourPlayerUpdates;

  for (auto e : gamethings) {
    UserState currUpdate = e->update(delta);

    if (e->isUser) ourPlayerUpdates = currUpdate;
  }

  return ourPlayerUpdates;
}

void Scene::updateState(SceneState newState) {
  // loop through GameThings, send newest state data
  for (auto e : gamethings) {
    int currId = e->netId;

    if (currId == -1) continue;  // skip thing

    SceneGameThingState currState = newState.GetUpdateFor(currId);
    // please check for non-null too!
    e->updateFromState(currState);
  }
}

void Scene::drawHUD(GLFWwindow* window) {
  if (camera->Fixed) return;

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  std::map<std::string, float> player_times;

  for (GameThing* e : gamethings) {
    if (dynamic_cast<Player*>(e) != nullptr) {
      Player* player = dynamic_cast<Player*>(e);
      std::string name = player->name;
      glm::vec3 position = player->transform.position;
      player_times[name] = player->time;
      const unsigned char* cname =
          reinterpret_cast<const unsigned char*>(name.c_str());
      glColor3f(1.0f, 1.0f, 1.0f);
      // glRasterPos2f(-0.1f, position[1] + 0.1);
      // TODO(matthew): implement world to screen-space positions
      glRasterPos2f(-0.1f, 0.1f);
      glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, cname);
    }
  }

  for (auto times : player_times) {
    std::string str = times.first;
    float time = times.second;
    std::string num_text = std::to_string(time);
    std::string rounded = num_text.substr(0, num_text.find(".") + 3);
    str += " " + rounded;
    const unsigned char* string =
        reinterpret_cast<const unsigned char*>(str.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    glWindowPos2f(10.0f, static_cast<float>(height) - 25);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, string);
  }
}
void Scene::draw() {
  // Pre-draw sequence:
  glm::mat4 viewProjMtx = camera->GetViewProjectMtx();
  glm::mat4 viewMtx = camera->GetViewMtx();  // required for certain lighting

  // Define stacks for depth-first search (DFS)
  std::stack<Node*> dfs_stack;
  std::stack<mat4> matrix_stack;

  // Initialize the current state variable for DFS
  Node* cur = node["world"];  // root of the tree
  // mat4 cur_VM = camera->view;   // 167 implementation uses VM, we can use
  // model matrix
  mat4 cur_MMtx = glm::mat4(1);

  dfs_stack.push(cur);
  matrix_stack.push(cur_MMtx);

  while (!dfs_stack.empty()) {
    // Detect whether the search runs into infinite loop by checking whether the
    // stack is longer than the size of the graph. Note that, at any time, the
    // stack does not contain repeated element.
    if (dfs_stack.size() > node.size()) {
      std::cerr << "Error: The scene graph has a closed loop." << std::endl;
      exit(-1);
    }

    // top-pop the stacks
    cur = dfs_stack.top();
    dfs_stack.pop();
    cur_MMtx = matrix_stack.top();
    matrix_stack.pop();

    // draw the visuals of our current node
    cur->draw(viewProjMtx, viewMtx, cur_MMtx);

    cur->draw_debug(viewProjMtx, cur_MMtx, Scene::_gizmos,
                    _globalSceneResources.models["_gz-xyz"],
                    _globalSceneResources.models["_gz-cube"]);

    // Continue the DFS: put all the child nodes of the current node in the
    // stack
    for (unsigned int i = 0; i < cur->childnodes.size(); i++) {
      dfs_stack.push(cur->childnodes[i]);
      matrix_stack.push(cur_MMtx * (cur->transformMtx));
    }
  }  // End of DFS while loop.
}

#include <imgui.h>

void Scene::gui() {
  ImGui::Begin("scene debug +++");

  ImGui::Checkbox("free camera", &camera->Fixed);
  ImGui::Checkbox("show gizmos", &_gizmos);

  ImGui::Separator();

  // show a list of game objects
  // ImGui::LabelText("", "gfx nodes");

  if (ImGui::TreeNode("--- node[] tree ---")) {
    std::stack<Node*> dfs_stack_gui;
    Node* cur = node["world"];  // root of the tree
    dfs_stack_gui.push(cur);

    int gui_id = 0;
    while (!dfs_stack_gui.empty()) {
      ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing,
                          ImGui::GetFontSize() * 3);

      // top-pop the stacks
      cur = dfs_stack_gui.top();
      dfs_stack_gui.pop();

      ImGuiTreeNodeFlags node_flags =
          ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
      if (cur->childnodes.empty()) node_flags |= ImGuiTreeNodeFlags_Leaf;
      bool node_open =
          ImGui::TreeNodeEx(reinterpret_cast<void*>((intptr_t)gui_id),
                            node_flags, cur->name.c_str());

      if (node_open) {
        for (unsigned int i = 0; i < cur->childnodes.size(); i++) {
          dfs_stack_gui.push(cur->childnodes[i]);
        }
        ImGui::TreePop();
      }
      gui_id++;

      ImGui::PopStyleVar();
    }  // End of DFS while loop
    ImGui::TreePop();
  }

  ImGui::End();
}
