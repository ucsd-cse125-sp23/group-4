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
#include "network/item.hpp"

using glm::mat4x4;
using glm::vec3;

int Scene::_myPlayerId = -1;

bool Scene::_freecam = false;
bool Scene::_gizmos = false;
SceneResourceMap Scene::_globalSceneResources = SceneResourceMap();

bool cmp(const std::pair<int, float>& a, const std::pair<int, float>& b) {
  return a.second < b.second;
}

Player* Scene::createPlayer(int id, std::string skin) {
  bool isUser = false;
  if (_myPlayerId >= 0 && _myPlayerId == id) isUser = true;

  // creating a player to be rendered
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
  if (dynamic_cast<AssimpModel*>(sceneResources->models[skins[id]])) {
    AssimpModel* amRef =
        dynamic_cast<AssimpModel*>(sceneResources->models[skins[id]]);
    AssimpModel* am = new AssimpModel(*amRef);
    myModel = am;
    player->pmodel = am;
  } else {
    if (sceneResources->models[skins[id]]) {
      myModel = new Model(*sceneResources->models[skins[id]]);
    } else {
      myModel = new Model(*sceneResources->models["PREFAB_player.model"]);
    }
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

  // sound effects
  SoundEffect* sfxRef =
      dynamic_cast<SoundEffect*>(sceneResources->sounds["sfx_jump"]);

  auto sfx = new SoundEffect(*sfxRef);
  player->sfx_jump = sfx;
  sfxRef = dynamic_cast<SoundEffect*>(sceneResources->sounds["sfx_item"]);
  sfx = new SoundEffect(*sfxRef);
  player->sfx_item = sfx;
  sfxRef = dynamic_cast<SoundEffect*>(sceneResources->sounds["sfx_tag"]);
  sfx = new SoundEffect(*sfxRef);
  player->sfx_tag = sfx;
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
  fx->transform.position = glm::vec3(0, 0.5f, 0);
  fx->transform.updateMtx(&fx->transformMtx);
  player->childnodes.push_back(fx);
  player->fx_item = fx;

  fx = new ParticleSystem(*ptclRef);
  fx->Reset(false);  // important!!!
  fx->name += "." + playername;
  fx->transform.position = glm::vec3(0, 0.5f, 0);
  fx->transform.updateMtx(&fx->transformMtx);
  player->childnodes.push_back(fx);
  player->fx_tag = fx;

  ParticleSystem* ptclRef2 =
      dynamic_cast<ParticleSystem*>(sceneResources->prefabs["ptcl_isTagged"]);
  fx = new ParticleSystem(*ptclRef2);
  fx->Reset(false);  // important!!!
  fx->name += "." + playername;
  fx->transform.position = glm::vec3(0, 1.0f, 0);
  fx->transform.updateMtx(&fx->transformMtx);
  player->childnodes.push_back(fx);
  player->fx_tagStatus = fx;

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

ItemBox* Scene::createItemBox(int id, Item iEnum) {
  // creating a player to be rendered
  std::string name = "item " + std::to_string(id);

  ItemBox* itemBox = new ItemBox();
  itemBox->name = name;
  itemBox->id = id;

  // Set model (TODO use asset)
  itemBox->model = sceneResources->models["cubeBoxTest"];

  ParticleSystem* ptclRef2 =
      dynamic_cast<ParticleSystem*>(sceneResources->prefabs["ptcl_isTagged"]);
  auto fx = new ParticleSystem(*ptclRef2);
  fx->Reset(false);  // important!!!
  fx->name += "." + name;
  fx->transform.position = glm::vec3(0, 0.5f, 0);
  fx->transform.updateMtx(&fx->transformMtx);
  itemBox->childnodes.push_back(fx);
  itemBox->fx = fx;

  // position is set by server message

  networkGameThings.insert({id, itemBox});
  node["world"]->childnodes.push_back(itemBox);

  return itemBox;
}

void Scene::removeItemBox(int id) {
  auto i = networkGameThings.at(id);

  // remove from world
  auto& nodes = node["world"]->childnodes;
  auto it = std::find(nodes.begin(), nodes.end(), i);
  nodes.erase(it);

  networkGameThings.erase(id);
  delete i;
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

void Scene::reset() {
  time.time = 5.0f;
  time.countdown = true;
  gameStart = false;
  timeOver = 0;

  networkGameThings.clear();
}

void Scene::animate(float delta) {
  // 2nd level animation process frame cap
  double fpsMin = (1.0 / fpsCapParam);
  num_updates_to_send += delta / fpsMin;

  std::vector<GameThing*> animators;
  GameThing* userAnim = nullptr;
  for (auto& thing : localGameThings) animators.push_back(thing);
  for (auto& [_, thing] : networkGameThings) {
    if (thing->isUser)
      userAnim = thing;
    else
      animators.push_back(thing);
  }

  if (userAnim) userAnim->animate(delta);  // always update user animations

  if (num_updates_to_send < 1) return;
  delta = fpsMin * num_updates_to_send;

  for (auto& thing : animators) thing->animate(delta);

  num_updates_to_send = 0;
}

void Scene::update(float delta) {
  if (gameStart) {
    for (auto& thing : localGameThings) thing->update(delta);
    for (auto& [_, thing] : networkGameThings) thing->update(delta);
    time.Update(delta);
  }

  if (Window::phase == GamePhase::GameOver) {
    // TODO: build new scene graph based on player rankings
    node["world"]->childnodes.clear();
    rankings = rankPlayers();
  }

  if (music) {
    music->setEffectVolume();
  }
}

message::UserStateUpdate Scene::pollUpdate() {
  if (!networkGameThings.count(_myPlayerId)) return {};

  return networkGameThings.at(_myPlayerId)->pollInput();
}

void Scene::receiveState(message::GameStateUpdate newState) {
  // update existing items, create new item if it doesn't exist
  for (auto& [id, player] : newState.players) {
    std::string skin = "bee";
    if (skins.count(id)) skin = skins[id];
    if (!networkGameThings.count(id)) createPlayer(id, skin);

    auto thing = networkGameThings.at(id);
    thing->updateFromState(player);
  }

  for (auto& [id, item] : newState.items) {
    if (!networkGameThings.count(id)) createItemBox(id, item.item);

    auto thing = networkGameThings.at(id);
    thing->updateFromState(item);
  }

  // remove items that don't exist on the server anymore
  std::vector<int> removedIds;
  for (auto& [id, _] : networkGameThings)
    if (!newState.players.count(id)) removedIds.push_back(id);

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

  auto t2 = networkGameThings.at(e.tagger);
  if (dynamic_cast<Player*>(t2) != nullptr) {
    Player* player = dynamic_cast<Player*>(t2);

    player->eventTag();
  }
}

#pragma endregion

std::vector<std::string> Scene::rankPlayers() {
  std::vector<std::pair<int, float>> player_times;
  for (auto& [i, g] : networkGameThings) {
    if (dynamic_cast<Player*>(g) != nullptr) {
      Player* player = dynamic_cast<Player*>(g);
      int p_id = player->id;
      float time = player->time.time;
      player_times.push_back(std::make_pair(p_id, time));
    }
  }
  std::sort(player_times.begin(), player_times.end(), cmp);

  std::vector<std::string> rankings;
  for (auto& [i, _] : player_times) {
    rankings.push_back(skins[i]);
  }
  return rankings;
}

void Scene::draw() {
  if (Window::phase == GamePhase::GameOver) {
    glDisable(GL_DEPTH_TEST);
    leaderboard.draw();
    leaderboard.drawPlayers(rankings);
    glEnable(GL_DEPTH_TEST);
  }
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
  settings.gui(&camera->FOV);

  ImGui::Begin("scene debug +++");

  ImGui::Checkbox("free camera", &camera->Fixed);
  ImGui::Checkbox("show gizmos", &_gizmos);

  ImGui::Separator();

  ImGui::SliderFloat("anim FPS cap (!!!)", &fpsCapParam, 5.0f, 30.0);

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
