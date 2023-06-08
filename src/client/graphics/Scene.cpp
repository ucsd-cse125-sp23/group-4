/**************************************************
Scene.cpp contains the implementation of the draw command
Renders all objects with DFS tree traversal
adapted from CSE 167 - Matthew
*****************************************************/
#include "Scene.h"

#include "Settings.h"

// The scene init definition
#include <Player.h>
#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <stack>

#include "Scene.inl"  // The scene init definition
#include "client/graphics/Window.h"

using glm::mat4x4;
using glm::vec3;

int Scene::_myPlayerId = -1;

bool Scene::_freecam = false;
bool Scene::_gizmos = false;
SceneResourceMap Scene::_globalSceneResources = SceneResourceMap();

Player* Scene::createPlayer(int id) {
  bool isUser = false;
  if (_myPlayerId >= 0 && _myPlayerId == id) isUser = true;

  // creating a player to be rendered... TODO call this from state update!
  std::string playername = "player " + std::to_string(id);

  Player* player = new Player();
  player->name = playername;
  player->id = id;
  if (isUser) {
    setToUserFocus(player);
  }

  // LOAD PLAYER ASSETS ---
  // copy into a new model object
  Model* myModel;
  if (dynamic_cast<AssimpModel*>(
          sceneResources->models["PREFAB_player.model"])) {
    AssimpModel* amRef = dynamic_cast<AssimpModel*>(
        sceneResources->models["PREFAB_player.model"]);
    AssimpModel* am = new AssimpModel(*amRef);
    myModel = am;
    player->pmodel = am;
  } else {
    myModel = new Model(*sceneResources->models[skins[id]]);
  }
  player->model = myModel;
  // TODO(matthew) set material here! if needed
  sceneResources->models[playername + ".model"] = myModel;

  // skin/costume
  if (false) {
    Model* myCostume =
        new Model(*sceneResources->models["PREFAB_player.costume1"]);
    // player->model = myModel; TODO: support multiple models on player
    sceneResources->models[playername + ".costume"] = myCostume;
  }

  // animations TODO(?)
  // player->pmodel->setAnimation("walk");  // TODO: make this automated

  // particle emitters
  ParticleSystem* ptclRef =
      dynamic_cast<ParticleSystem*>(sceneResources->prefabs["ptcl_jump"]);
  auto fx = new ParticleSystem(*ptclRef);
  fx->Reset(false);  // important!!!
  fx->name += "." + playername;
  fx->transform.position = glm::vec3(0, 0, 0);
  fx->transform.updateMtx(&fx->transformMtx);
  player->childnodes.push_back(fx);
  player->fx_jump = fx;

  fx = new ParticleSystem(*ptclRef);
  fx->Reset(false);
  fx->name += "." + playername;
  fx->transform.position = glm::vec3(0, 0, 0);
  fx->transform.updateMtx(&fx->transformMtx);
  player->childnodes.push_back(fx);
  player->fx_land = fx;

  fx = new ParticleSystem(*ptclRef);
  fx->Reset(false);
  fx->name += "." + playername;
  fx->transform.position = glm::vec3(0, 0, 0);
  fx->transform.updateMtx(&fx->transformMtx);
  player->childnodes.push_back(fx);
  player->fx_item = fx;

  fx = new ParticleSystem(*ptclRef);
  fx->Reset(false);  // important!!!
  fx->name += "." + playername;
  fx->transform.position = glm::vec3(0, 0, 0);
  fx->transform.updateMtx(&fx->transformMtx);
  player->childnodes.push_back(fx);
  player->fx_tag = fx;

  // ---

  // position is set by server message

  networkGameThings.insert({id, player});
  node["world"]->childnodes.push_back(player);

  return player;
}

void Scene::removePlayer(int id) {
  auto player = networkGameThings.at(id);

  // remove player from world
  auto& nodes = node["world"]->childnodes;
  auto it = std::find(nodes.begin(), nodes.end(), player);
  nodes.erase(it);

  networkGameThings.erase(id);
  delete player;
}

void Scene::initFromServer(int myid) { _myPlayerId = myid; }

void Scene::setToUserFocus(GameThing* t) {
  myPlayer = nullptr;

  t->isUser = true;
  if (dynamic_cast<Player*>(t) != nullptr) {
    Player* player = dynamic_cast<Player*>(t);
    player->camera = camera;  // give a reference to the game camera
    myPlayer = player;
  }
  t->childnodes.push_back(camera);  // parent camera to player
}

void Scene::animate(float delta) {
  for (auto& thing : localGameThings) thing->animate(delta);
  for (auto& [_, thing] : networkGameThings) thing->animate(delta);
}

void Scene::update(float delta) {
  for (auto& thing : localGameThings) thing->update(delta);
  for (auto& [_, thing] : networkGameThings) thing->update(delta);
  if (gameStart) {
    time.Update(delta);
  }
}

message::UserStateUpdate Scene::pollUpdate() {
  if (!networkGameThings.count(_myPlayerId)) return {};

  return networkGameThings.at(_myPlayerId)->pollInput();
}

void Scene::receiveState(message::GameStateUpdate newState) {
  // update existing items, create new item if it doesn't exist
  for (auto& [id, state] : newState.things) {
    // TODO: handle items besides Player as well
    if (!networkGameThings.count(id)) createPlayer(id);

    auto thing = networkGameThings.at(id);
    thing->updateFromState(state);
  }

  // remove items that don't exist on the server anymore
  std::vector<int> removedIds;
  for (auto& [id, _] : networkGameThings)
    if (!newState.things.count(id)) removedIds.push_back(id);

  for (int id : removedIds) removePlayer(id);
}

#pragma region receiveEvent

void Scene::receiveEvent_jump(message::JumpEvent e) {
  if (!networkGameThings.count(e.pid)) return;

  auto t = networkGameThings.at(e.pid);
  if (dynamic_cast<Player*>(t) != nullptr) {
    Player* player = dynamic_cast<Player*>(t);

    player->eventJump();
  }
}
void Scene::receiveEvent_land(message::LandEvent e) {
  if (!networkGameThings.count(e.pid)) return;

  auto t = networkGameThings.at(e.pid);
  if (dynamic_cast<Player*>(t) != nullptr) {
    Player* player = dynamic_cast<Player*>(t);

    player->eventLand();
  }
}
void Scene::receiveEvent_item(message::ItemPickupEvent e) {
  if (!networkGameThings.count(e.pid)) return;

  auto t = networkGameThings.at(e.pid);
  if (dynamic_cast<Player*>(t) != nullptr) {
    Player* player = dynamic_cast<Player*>(t);

    player->eventItem(/*TODO(matthew) add param?*/);
  }
}
void Scene::receiveEvent_tag(message::TagEvent e) {
  if (!networkGameThings.count(e.taggee)) return;

  auto t1 = networkGameThings.at(e.taggee);
  if (dynamic_cast<Player*>(t1) != nullptr) {
    Player* player = dynamic_cast<Player*>(t1);

    player->eventTagged();
  }
}

#pragma endregion

void Scene::draw() {
  // Pre-draw sequence:
  if (myPlayer) camera->SetPositionTarget(myPlayer->transform.position);
  camera->UpdateView();

  DrawInfo drawInfo = DrawInfo();
  drawInfo.viewMtx = camera->GetViewMtx();  // required for certain lighting
  drawInfo.viewProjMtx = camera->GetViewProjectMtx();
  drawInfo.viewProjOrigMtx = camera->GetViewProjectMtx(true);

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
    // Detect whether the search runs into infinite loop
    if (dfs_stack.size() > std::max(static_cast<int>(node.size()), 100)) {
      // std::cerr << "Error: The scene graph probably has a closed loop."
      //           << std::endl;
      // exit(-1);
    }

    // top-pop the stacks
    cur = dfs_stack.top();
    dfs_stack.pop();
    cur_MMtx = matrix_stack.top();
    matrix_stack.pop();

    // draw the visuals of our current node
    cur->draw(drawInfo, cur_MMtx);

    cur->draw_debug(drawInfo, cur_MMtx, Scene::_gizmos,
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

Settings settings;  // define extern var

void Scene::gui() {
  settings.gui();

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
