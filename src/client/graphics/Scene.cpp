/**************************************************
Scene.cpp contains the implementation of the draw command
Renders all objects with DFS tree traversal
adapted from CSE 167 - Matthew
*****************************************************/
#include "Scene.h"

// The scene init definition 
#include "Scene.inl"


using namespace glm;
/*
GLuint textureID;

void LoadTexture() {
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  // set the texture wrapping/filtering options (on the currently bound
  // texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // load and generate the texture
  int width, height, nrChannels;
  unsigned char* data = stbi_load("assets/image/test_uv.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Cannot open file: " << filename << std::endl;
  }

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
}*/

Scene::Scene() {
  camera = new Camera;
  sceneResources = new SceneResourceMap();
  time.time = 300.0f;
  time.countdown = true;

  // the default scene graph already has one node named "world."
  node["world"] = new Node("world");
}

void Scene::update(GLFWwindow* window, Camera* camera, float delta, float step) {
    //if (player) player->update(deltaTime);

    for (auto e : gamethings) {
        e->update(window, camera, delta, step);
    }

    time.Update(delta);
}

void Scene::drawHUD(GLFWwindow* window) { 
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
  
    std::map<std::string, Timer> player_times;

    for (GameThing* e : gamethings) {
      if (dynamic_cast<Player*>(e) != nullptr) {
        Player* player = dynamic_cast<Player*>(e);
        std::string name = player->name;
        PlayerModel* mod = player->mod;
        Skeleton* skel = mod->skel;
        glm::vec3 position = skel->getPos();
        player_times[name] = player->time;
        const unsigned char* cname =
            reinterpret_cast<const unsigned char*>(name.c_str());
        fr.RenderText(name, (width / 2.0f) - 30.0f, (height / 2.0f) + 25.0f, 0.5f,
                   glm::vec3(0.0f, 0.0f, 0.0f)); //TODO: get screen coordinates based off player position
      }
    }
    
    for (auto times : player_times) {
      std::string str = times.first;
      Timer time = times.second;
      str += " " + time.ToString();
      fr.RenderText(str, 10.0f, float(height) - 25, 0.5f,
                     glm::vec3(1.0f, 1.0f, 1.0f));
    }

    std::string game_time = time.ToString();
    fr.RenderText(game_time, (width / 2.0f) - 75.0f, height - 35.0f, 1.0f,
                   glm::vec3(1.0f, 0.0f, 0.0f));

    // minimap stuff
    int map_size = width / 4;
    glViewport(10, 10, map_size, map_size);
    glScissor(10, 10, map_size, map_size);
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); 
    glDisable(GL_SCISSOR_TEST);

    for (GameThing* e : gamethings) {
      if (dynamic_cast<Player*>(e) != nullptr) {
        Player* player = dynamic_cast<Player*>(e);
        std::string name = player->name;
        PlayerModel* mod = player->mod;
        Skeleton* skel = mod->skel;
        glm::vec3 position = skel->getPos();
        glColor3f(0.0f, 0.0f, 1.0f);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex3f(position[0] / map_size, -position[2] / map_size, 0.0f);
        glEnd();
      }
    }
    

    glViewport(0, 0, width, height);

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
 }
void Scene::draw(GLFWwindow* window, const glm::mat4& viewProjMtx) {
    // Pre-draw sequence:
    //camera->computeMatrices();

    // Define stacks for depth-first search (DFS)
    std::stack < Node* > dfs_stack;
    std::stack < mat4 >  matrix_stack;

    // Initialize the current state variable for DFS
    Node* cur = node["world"]; // root of the tree
    //mat4 cur_VM = camera->view;   // 167 implementation uses VM, we can use model matrix
    mat4 cur_MMtx = cur->transformMtx;

    dfs_stack.push(cur);
    matrix_stack.push(cur_MMtx);

    while (!dfs_stack.empty()) {
        // Detect whether the search runs into infinite loop by checking whether the stack is longer than the size of the graph.
        // Note that, at any time, the stack does not contain repeated element.
        if (dfs_stack.size() > node.size()) {
            std::cerr << "Error: The scene graph has a closed loop." << std::endl;
            exit(-1);
        }

        // top-pop the stacks
        cur = dfs_stack.top();
        dfs_stack.pop();
        cur_MMtx = matrix_stack.top();
        matrix_stack.pop();

        // draw the model of our current node
        if(cur->model)
            cur->model->draw(viewProjMtx, cur_MMtx);

        if (_gizmos && cur->_renderGizmo)
            sceneResources->models["_gz-xyz"]->draw(viewProjMtx, cur_MMtx, true);

        // Continue the DFS: put all the child nodes of the current node in the stack
        for (unsigned int i = 0; i < cur->childnodes.size(); i++) {
            dfs_stack.push(cur->childnodes[i]);
            matrix_stack.push(cur_MMtx * (cur->childnodes[i]->transformMtx));
        }

    } // End of DFS while loop.

    drawHUD(window);
}


#include "imgui/imgui.h"
void treeChildren(ImGuiTreeNodeFlags node_flags, bool isOpen, Node* par)
{
    if (isOpen)
    {
        for (auto child : par->childnodes)
        {
            if (child->childnodes.empty())
            {
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                //ImGui::TreeNodeEx((void*)(intptr_t)par, node_flags, child.name.c_str());
            }
            else
            {
                //bool o = ImGui::TreeNodeEx((void*)(intptr_t)par, node_flags, child.name.c_str());
                ImGui::TreePop();
                //treeChildren(node_flags, o, p);
            }
        }
        ImGui::TreePop();
    }
}

void Scene::gui() {
    ImGui::Begin("scene debug +++");

    ImGui::Checkbox("free camera (TODO)", &_freecam);
    ImGui::Checkbox("show gizmos", &_gizmos);

    ImGui::Separator();

    //show a list of game objects
    //ImGui::LabelText("", "gfx nodes");

    if (ImGui::TreeNode("--- node[] tree ---"))
    {
        std::stack < Node* > dfs_stack_gui;
        Node* cur = node["world"]; // root of the tree
        dfs_stack_gui.push(cur);

        int gui_id = 0;
        while (!dfs_stack_gui.empty()) {
            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);

            // top-pop the stacks
            cur = dfs_stack_gui.top();
            dfs_stack_gui.pop();

            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
            if (cur->childnodes.empty()) node_flags |= ImGuiTreeNodeFlags_Leaf;
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)gui_id, node_flags, cur->name.c_str());

            if (node_open) {
                for (unsigned int i = 0; i < cur->childnodes.size(); i++) {
                    dfs_stack_gui.push(cur->childnodes[i]);
                }
                ImGui::TreePop();
            }
            gui_id++;

            ImGui::PopStyleVar();

        } // End of DFS while loop

        ImGui::TreePop();
    }

    ImGui::End();
}
