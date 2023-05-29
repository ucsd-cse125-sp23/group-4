#include "AssimpModel.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <deque>
#include <glm/gtc/matrix_transform.hpp>

// #define STB_IMAGE_IMPLEMENTATION
#include "client/graphics/AssimpMath.h"
#include "client/graphics/imported/stb_image.h"

AssimpModel::AssimpModel()
    : name("N/A"),
      rootNode(nullptr),
      numNode(0),
      isAnimated(false),
      isPaused(true),
      currentAnimation(-1),
      animModes(nullptr) {}

bool AssimpModel::loadAssimp(const char* path) {
  if (rootNode) {
    return false;
  }

  /* Load with assimp importer, check if loading failed */
  Assimp::Importer import;
  const aiScene* scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    printf("ERR::ASSIMP::%s\n", import.GetErrorString());
    return false;
  }

  name = std::string(path);
  name = name.substr(name.find_last_of('/') + 1);

  prepareNodes(scene);
  if (numNode != nodeMap.size()) {
    printf(
        "Assimp: [ERROR] duplicate node: processed %u nodes, has %u unique "
        "name nodes\n",
        numNode, nodeMap.size());
    printf("Assimp: Aborting loading %s\n", name);
    return false;
  }

  std::vector<std::pair<aiMesh*, AssimpMesh*>> meshQueue;
  if (!populateNode(scene, scene->mRootNode, glm::mat4(1.0f), meshQueue)) {
    printf("Assimp: Aborting loading %s\n", name);
    return false;
  }
  if (scene->mNumMeshes != meshQueue.size()) {
    printf(
        "Assimp: [ERROR] unmatched mesh count: should have %u meshes, "
        "processed %u meshes\n",
        scene->mNumMeshes, meshQueue.size());
    printf("Assimp: Aborting loading %s\n", name);
    return false;
  }
  for (unsigned int i = 0; i < meshQueue.size(); i++) {
    if (!populateMesh(scene, meshQueue[i].first, meshQueue[i].second)) {
      printf("Assimp: Aborting loading %s\n", name);
      return false;
    }
  }

  loadAssimpHelperAnim(scene);
  loadAssimpHelperImgui();
  useMesh();
  betterView = glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(0.01f)),
                              glm::vec3(0, -120, 0));
  return true;
}

void AssimpModel::prepareNodes(const aiScene* const scene) {
  // bfs
  numNode = 0;
  nodeMap.clear();
  std::deque<aiNode*> queue;
  queue.push_back(scene->mRootNode);

  while (!queue.empty()) {
    unsigned int lvlSize = queue.size();
    for (unsigned int i = 0; i < lvlSize; i++) {
      aiNode* curr = queue.front();
      queue.pop_front();

      nodeMap[curr->mName.C_Str()] = new AssimpNode(numNode++);
      for (unsigned int c = 0; c < curr->mNumChildren; c++) {
        queue.push_back(curr->mChildren[c]);
      }
    }
  }

  rootNode = nodeMap[scene->mRootNode->mName.C_Str()];
}

bool AssimpModel::populateNode(
    const aiScene* const scene, const aiNode* const aiNode,
    const glm::mat4 accTransform,
    std::vector<std::pair<aiMesh*, AssimpMesh*>>& meshQueue) {
  if (nodeMap.find(aiNode->mName.C_Str()) == nodeMap.end()) {
    printf("Assimp: [ERROR] cannot find node: %s\n", aiNode->mName.C_Str());
    return false;
  }

  AssimpNode* const node = nodeMap.find(aiNode->mName.C_Str())->second;
  node->name = aiNode->mName.C_Str();
  node->localTransform = aiMatToMat4x4(aiNode->mTransformation);
  node->animationTransform = node->localTransform;
  node->matWorldTransform = accTransform * node->localTransform;

  // prepare meshes in this node
  for (unsigned int i = 0; i < aiNode->mNumMeshes; i++) {
    AssimpMesh* const mesh = new AssimpMesh();
    meshes.push_back(mesh);
    node->meshes.push_back(mesh);
    meshQueue.push_back(
        std::make_pair(scene->mMeshes[aiNode->mMeshes[i]], mesh));
  }

  // populate child nodes
  for (unsigned int i = 0; i < aiNode->mNumChildren; i++) {
    node->children.push_back(
        nodeMap.find(aiNode->mChildren[i]->mName.C_Str())->second);
    populateNode(scene, aiNode->mChildren[i], node->matWorldTransform,
                 meshQueue);
  }

  return true;
}

bool AssimpModel::populateMesh(const aiScene* const scene,
                               const aiMesh* const aiMesh,
                               AssimpMesh* const mesh) {
  mesh->name = aiMesh->mName.C_Str();

  // process vertex data
  if (aiMesh->HasTextureCoords(0) && aiMesh->mNumUVComponents[0] == 2) {
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
      mesh->pos.push_back(glm::vec4(aiMesh->mVertices[i].x,
                                    aiMesh->mVertices[i].y,
                                    aiMesh->mVertices[i].z, 1.0f));
      mesh->outPos.push_back(
          glm::vec3(mesh->pos[i].x, mesh->pos[i].y, mesh->pos[i].z));
      mesh->animPos.push_back(glm::vec4(0.0f));
      mesh->norm.push_back(glm::vec4(aiMesh->mNormals[i].x,
                                     aiMesh->mNormals[i].y,
                                     aiMesh->mNormals[i].z, 0.0f));
      mesh->outNorm.push_back(
          glm::vec3(mesh->norm[i].x, mesh->norm[i].y, mesh->norm[i].z));
      mesh->animNorm.push_back(glm::vec4(0.0f));
      mesh->uvs.push_back(glm::vec2(aiMesh->mTextureCoords[0][i].x,
                                    aiMesh->mTextureCoords[0][i].y));
    }
  } else {
    printf("Assimp: ignoring UV of mesh '%s' - length is not 2 but %u.\n",
           aiMesh->mName.C_Str(), aiMesh->mNumUVComponents[0]);
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
      mesh->pos.push_back(glm::vec4(aiMesh->mVertices[i].x,
                                    aiMesh->mVertices[i].y,
                                    aiMesh->mVertices[i].z, 1.0f));
      mesh->outPos.push_back(
          glm::vec3(mesh->pos[i].x, mesh->pos[i].y, mesh->pos[i].z));
      mesh->animPos.push_back(glm::vec4(0.0f));
      mesh->norm.push_back(glm::vec4(aiMesh->mNormals[i].x,
                                     aiMesh->mNormals[i].y,
                                     aiMesh->mNormals[i].z, 0.0f));
      mesh->outNorm.push_back(
          glm::vec3(mesh->norm[i].x, mesh->norm[i].y, mesh->norm[i].z));
      mesh->animNorm.push_back(glm::vec4(0.0f));
      mesh->uvs.push_back(glm::vec2(0.0f));
    }
  }
  if (mesh->pos.size() != mesh->outPos.size() ||
      mesh->pos.size() != mesh->animPos.size() ||
      mesh->norm.size() != mesh->outNorm.size() ||
      mesh->norm.size() != mesh->animNorm.size() ||
      mesh->pos.size() != mesh->norm.size() ||
      mesh->pos.size() != mesh->uvs.size()) {
    return false;
  }
  for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
    aiFace& aiFace = aiMesh->mFaces[i];
    for (unsigned int j = 0; j < aiFace.mNumIndices; j++) {
      mesh->indices.push_back(aiFace.mIndices[j]);
    }
  }

  // bind joints to nodes
  for (unsigned int i = 0; i < aiMesh->mNumBones; i++) {
    if (!bindJoint(mesh, aiMesh->mBones[i])) {
      return false;
    }
  }

  // process material
  aiColor3D color(0.f, 0.f, 0.f);
  float f = 0.0f;
  aiMaterial* aiMaterial = scene->mMaterials[aiMesh->mMaterialIndex];
  aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
  mesh->diffuse = glm::vec4(color.r, color.g, color.b, 1.0f);
  aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
  mesh->specular = glm::vec4(color.r, color.g, color.b, 1.0f);
  aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
  mesh->ambient = glm::vec4(color.r, color.g, color.b, 1.0f);
  aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
  mesh->emissive = glm::vec4(color.r, color.g, color.b, 1.0f);
  aiMaterial->Get(AI_MATKEY_SHININESS, f);
  mesh->shininess = f;

  // process texture
  if (aiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
    aiString texFile;
    aiMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texFile);
    printf(
        "Assimp: mesh \"%s\" has %u textures, loading the first one: "
        "\"%s\"\n",
        aiMesh->mName.C_Str(),
        aiMaterial->GetTextureCount(aiTextureType_DIFFUSE),
        aiMaterial->GetName().C_Str());

    if (const aiTexture* aiTexture =
            scene->GetEmbeddedTexture(texFile.C_Str())) {
      // embedded file
      printf("Assimp: loading texture of \"%s\" - embedded W:H = %u:%u\n",
             aiMaterial->GetName().C_Str(), aiTexture->mWidth,
             aiTexture->mHeight);
      if (aiTexture->mHeight == 0) {
        // compressed file
        printf("        (Compressed: %s file)\n", aiTexture->achFormatHint);
        int ok, width, height, numOfChannels;
        ok = stbi_info_from_memory(
            reinterpret_cast<const unsigned char*>(aiTexture->pcData),
            aiTexture->mWidth, &width, &height, &numOfChannels);
        printf(
            "Assimp: Loading \"%s\" texture info %d - W:H = "
            "%d:%d[%d]\n",
            aiMaterial->GetName().C_Str(), ok, width, height, numOfChannels);

        if (ok == 0) {
          printf("Assimp: load texture of \"%s\" failed\n",
                 aiMaterial->GetName().C_Str());
        } else {
          printf("Assimp: load texture of \"%s\" done - W:H = %d:%d[%d]\n",
                 aiMaterial->GetName().C_Str(), width, height, numOfChannels);

          mesh->texture = new Texture();
          mesh->texture->init(
              reinterpret_cast<const unsigned char*>(aiTexture->pcData),
              aiTexture->mWidth);
        }
      } else {
        // uncompressed file
        printf(
            "        (Not compressed, not implemented so nothing will "
            "happen)\n");
      }
    } else {
      // external file
      printf("Assimp: loading texture of \"%s\" - external.\n",
             aiMaterial->GetName().C_Str());
      printf("        (Not implemented so nothing will happen)\n");
    }
  }

  return true;
}

bool AssimpModel::bindJoint(AssimpMesh* const mesh,
                            const aiBone* const aiBone) {
  if (nodeMap.find(aiBone->mName.C_Str()) == nodeMap.end()) {
    printf("Assimp: [ERROR] cannot bind node: %s\n", aiBone->mName.C_Str());
    return false;
  }

  AssimpNode* node = nodeMap.find(aiBone->mName.C_Str())->second;
  NodeJoint nodeJoint(mesh, aiMatToMat4x4(aiBone->mOffsetMatrix));
  for (unsigned int i = 0; i < aiBone->mNumWeights; i++) {
    nodeJoint.vertexWeights.push_back(VertexWeight{
        aiBone->mWeights[i].mVertexId, aiBone->mWeights[i].mWeight});
  }
  node->joints.push_back(nodeJoint);

  return true;
}

void AssimpModel::loadAssimpHelperAnim(const aiScene* scene) {
  for (int i = 0; i < scene->mNumAnimations; i++) {
    aiAnimation* aiAnimation = scene->mAnimations[i];
    AssimpAnimation animation;
    animation.name = aiAnimation->mName.C_Str();
    animation.duration = aiAnimation->mDuration;
    animation.tps = aiAnimation->mTicksPerSecond;
    for (int j = 0; j < aiAnimation->mNumChannels; j++) {
      aiNodeAnim* aiChannel = aiAnimation->mChannels[j];
      AssimpChannel channel;
      channel.name = aiChannel->mNodeName.C_Str();
      channel.node = nodeMap[channel.name];
      for (int k = 0; k < aiChannel->mNumPositionKeys; k++) {
        channel.positions.push_back(
            std::make_pair(aiChannel->mPositionKeys[k].mTime,
                           aiVecToVec3(aiChannel->mPositionKeys[k].mValue)));
      }
      for (int k = 0; k < aiChannel->mNumRotationKeys; k++) {
        channel.rotations.push_back(std::make_pair(
            aiChannel->mRotationKeys[k].mTime,
            aiQuaternionToVec4(aiChannel->mRotationKeys[k].mValue)));
      }
      for (int k = 0; k < aiChannel->mNumScalingKeys; k++) {
        channel.scalings.push_back(
            std::make_pair(aiChannel->mScalingKeys[k].mTime,
                           aiVecToVec3(aiChannel->mScalingKeys[k].mValue)));
      }
      channel.extrapPre = AssimpChannel::loadExtrapMode(aiChannel->mPreState);
      channel.extrapPost = AssimpChannel::loadExtrapMode(aiChannel->mPostState);
      animation.channels.push_back(channel);
    }
    animations.push_back(animation);
  }
}

void AssimpModel::draw(const glm::mat4& viewProjMtx, GLuint shader) {
  if (!rootNode) {
    return;
  }

  glUseProgram(shader);
  // TODO: remove this matrix when done with integration
  glm::mat4 betterViewMat = glm::scale(
      glm::translate(glm::mat4(1.0f), glm::vec3(0, -2, -0)), glm::vec3(0.025f));
  for (int i = 0; i < meshes.size(); i++) {
    if (meshVisibilities[i]) {
      meshes[i]->draw(viewProjMtx, shader);
    }
  }

  if (drawNode) {
    glDepthFunc(GL_ALWAYS);
    rootNode->draw(viewProjMtx);
    glDepthFunc(GL_LEQUAL);
  }
}

void AssimpModel::loadAssimpHelperImgui() {
  animModes = new char*[1 + animations.size()];
  for (int i = 0; i < animations.size() + 1; i++) {
    animModes[i] = new char[256];
  }
  snprintf(animModes[0], sizeof(animModes[0]), "Mesh");
  for (int i = 0; i < animations.size(); i++) {
    snprintf(animModes[1 + i], sizeof(animModes[1 + i]), "Animation %d", i + 1);
  }

  for (auto& n : nodeMap) {
    nodeControlMap[n.first] = ControlInfo();
  }
}

void AssimpModel::imGui() {
  ImGui::Begin("Assimp Model Info");

  ImGui::Text("File: %s", name.c_str());
  unsigned int numTreeNode = 0;

  // Node Tree
  ImGui::SeparatorText("Node Tree");
  ImGui::Checkbox("draw node", &drawNode);
  if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)numTreeNode++),
                      "Node Highlight (%lu)", nodeMap.size())) {
    for (auto it = nodeMap.begin(); it != nodeMap.end(); it++) {
      std::string name = it->first;
      ImGui::Checkbox(name.c_str(), &it->second->isMarked);
    }
    ImGui::TreePop();
  }
  if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)numTreeNode++),
                      "Node Tree (%lu)", nodeMap.size())) {
    rootNode->imGui();
    ImGui::TreePop();
  }

  // Meshes
  ImGui::SeparatorText("Meshes");
  if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)numTreeNode++),
                      "Meshes (%lu)", meshes.size())) {
    for (int i = 0; i < meshes.size(); i++) {
      if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)i), "Mesh %d", i)) {
        bool visibility = meshVisibilities[i];
        ImGui::Checkbox("visible", &visibility);
        meshVisibilities[i] = visibility;
        meshes[i]->imGui();
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }

  // Animations
  ImGui::SeparatorText("Animations");
  if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)numTreeNode++),
                      "Animations (%lu)", animations.size())) {
    for (int i = 0; i < animations.size(); i++) {
      if (ImGui::TreeNode(reinterpret_cast<void*>((intptr_t)i), "Animation %d",
                          i)) {
        animations[i].imGui();
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
  ImGui::SeparatorText("Current Animation");
  if (ImGui::Button(isPaused ? "Start" : "Pause")) {
    if (isAnimated) {
      isPaused = !isPaused;
    }
  }
  ImGui::Text("Animation: %lu", animations.size());
  if (ImGui::BeginListBox("Current Anim")) {
    for (int i = 0; i < animations.size() + 1; i++) {
      const bool isSelected = (currentAnimation + 1 == i);
      if (ImGui::Selectable(animModes[i], isSelected)) {
        if (i == 0) {
          useMesh();
        } else {
          useAnimation(i - 1);
        }
      }
      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndListBox();
  }

  // Joint control
  ImGui::SeparatorText("Joint Control");
  imGuiJointMenu();

  ImGui::End();
}

void AssimpModel::useMesh() { useAnimation(-1); }

bool AssimpModel::useAnimation(int animationInd) {
  printf("Assimp: Animation - Using %d\t: %s\n", animationInd,
         animationInd < 0 || animationInd >= animations.size()
             ? "Mesh | Unknown"
             : animations[animationInd].name.c_str());

  if (animationInd < 0) {
    currentAnimation = -1;
    isAnimated = false;
    isPaused = true;
    for (auto const& x : nodeMap) {
      x.second->animationTransform = x.second->localTransform;
    }
    for (unsigned int i = 0; i < meshes.size(); i++) {
      for (unsigned int v = 0; v < meshes[i]->pos.size(); v++) {
        meshes[i]->animPos[v] = glm::vec4(0.0f);
        meshes[i]->animNorm[v] = glm::vec4(0.0f);
      }
    }
    rootNode->update(glm::mat4(1.0f));
    for (unsigned int i = 0; i < meshes.size(); i++) {
      for (unsigned int v = 0; v < meshes[i]->pos.size(); v++) {
        meshes[i]->animPos[v] /= meshes[i]->animPos[v].w;
        meshes[i]->animNorm[v] = glm::normalize(meshes[i]->animNorm[v]);
        meshes[i]->outPos[v] =
            glm::vec3(meshes[i]->animPos[v].x, meshes[i]->animPos[v].y,
                      meshes[i]->animPos[v].z);
        meshes[i]->outNorm[v] =
            glm::vec3(meshes[i]->animNorm[v].x, meshes[i]->animNorm[v].y,
                      meshes[i]->animNorm[v].z);
      }
    }
    return true;
  }
  if (animationInd >= animations.size()) {
    return false;
  }

  isAnimated = true;
  isPaused = true;
  for (auto const& x : nodeMap) {
    x.second->animationTransform = glm::mat4(1.0f);
  }
  currentAnimation = animationInd;
  animations[animationInd].restart();
  return true;
}

void AssimpModel::update(float deltaTimeInMs) {
  if (!isAnimated || isPaused) {
    return;
  }

  animations[currentAnimation].update(deltaTimeInMs);

  for (unsigned int i = 0; i < meshes.size(); i++) {
    for (unsigned int v = 0; v < meshes[i]->pos.size(); v++) {
      meshes[i]->animPos[v] = glm::vec4(0.0f);
      meshes[i]->animNorm[v] = glm::vec4(0.0f);
    }
  }
  rootNode->update(glm::mat4(1.0f));
  for (unsigned int i = 0; i < meshes.size(); i++) {
    for (unsigned int v = 0; v < meshes[i]->pos.size(); v++) {
      meshes[i]->animPos[v] /= meshes[i]->animPos[v][3];
      meshes[i]->animNorm[v] = glm::normalize(meshes[i]->animNorm[v]);
      meshes[i]->outPos[v] =
          glm::vec3(meshes[i]->animPos[v].x, meshes[i]->animPos[v].y,
                    meshes[i]->animPos[v].z);
      meshes[i]->outNorm[v] =
          glm::vec3(meshes[i]->animNorm[v].x, meshes[i]->animNorm[v].y,
                    meshes[i]->animNorm[v].z);
    }
  }
}

void AssimpModel::imGuiJointMenu() {
  // for (auto& x : nodeMap) {
  //   AssimpNode* node = x.second;
  //   const std::string name = node->name;
  //   if (ImGui::TreeNode(name.c_str())) {
  //     bool& isControlled = nodeControlMap[x.first].useControl;
  //     glm::vec3& pose = nodeControlMap[x.first].pose;
  //     glm::vec3& scale = nodeControlMap[x.first].scale;
  //     glm::vec3& offset = nodeControlMap[x.first].offset;
  //     ImGui::Checkbox("Control", &isControlled);
  //     ImGui::SliderFloat("Pos X", &offset.x, -5.0f, 5.0f, "%.2f");
  //     ImGui::SliderFloat("Pos Y", &offset.y, -5.0f, 5.0f, "%.2f");
  //     ImGui::SliderFloat("Pos Z", &offset.z, -5.0f, 5.0f, "%.2f");
  //     ImGui::SliderFloat("Rot X", &pose.x, -360.0f, 360.0f, "%.2f");
  //     ImGui::SliderFloat("Rot Y", &pose.y, -360.0f, 360.0f, "%.2f");
  //     ImGui::SliderFloat("Rot Z", &pose.z, -360.0f, 360.0f, "%.2f");
  //     ImGui::SliderFloat("Sca X", &scale.x, 0.1f, 2.0f, "%.2f");
  //     ImGui::SliderFloat("Sca Y", &scale.y, 0.1f, 2.0f, "%.2f");
  //     ImGui::SliderFloat("Sca Z", &scale.z, 0.1f, 2.0f, "%.2f");
  //     ImGui::TreePop();
  //     if (isControlled) {
  //       glm::vec3 poseR = glm::radians(pose);
  //       glm::mat4 result(1.0f);
  //       result = glm::translate(result, offset);
  //       result = glm::rotate(result, poseR.x, glm::vec3(1.0f, 0.0f, 0.0f));
  //       result = glm::rotate(result, poseR.y, glm::vec3(0.0f, 1.0f, 0.0f));
  //       result = glm::rotate(result, poseR.z, glm::vec3(0.0f, 0.0f, 1.0f));
  //       result = glm::scale(result, scale);
  //       node->animationTransform = result;
  //       rootNode->update(glm::mat4(1.0f));
  //       for (int i = 0; i < meshes.size(); i++) {
  //         meshes[i]->update();
  //       }
  //     }
  //   }
  // }
}

void AssimpModel::setAnimation(std::string name) {
  for (int i = 0; i < animations.size(); i++) {
    if (animations[i].name.compare(name) == 0) {
      useAnimation(i);
      isPaused = false;
      return;
    }
  }

  // use default pose
  std::cout << "Assimp: Animation - cannot find " << name.c_str()
            << " using Mesh instead" << std::endl;
  useAnimation(-1);
}

void AssimpModel::draw(const glm::mat4& viewProjMtx, const glm::mat4& viewMtx,
                       const glm::mat4& transformMtx, const bool ignoreDepth) {
  if (!material) {
    return;
  }

  GLuint shader = material->shader;
  glUseProgram(shader);

  if (ignoreDepth) glDisable(GL_DEPTH_TEST);

  for (int i = 0; i < meshes.size(); i++) {
    material->diffuse = meshes[i]->diffuse;
    material->specular = meshes[i]->specular;
    //material->ambient = meshes[i]->ambient;
    //material->emission = meshes[i]->emissive;
    //material->shininess = meshes[i]->shininess;
    material->setUniforms(viewProjMtx, viewMtx,
                          transformMtx * modelMtx * betterView);
    if (meshes[i]->texture) {
      meshes[i]->texture->bindgl();
      glUniform1i(glGetUniformLocation(shader, "gSampler"), 0);
      glUniform1i(glGetUniformLocation(shader, "renderMode"), 1);
    } else {
      glUniform1i(glGetUniformLocation(shader, "renderMode"), 0);
    }

    meshes[i]->draw();
  }
  if (ignoreDepth) glEnable(GL_DEPTH_TEST);

  glUseProgram(0);
}

void AssimpModel::draw() {
  // if (!isAnimated) {
  //   useMesh();
  // }

  for (int i = 0; i < meshes.size(); i++) {
    meshes[i]->draw();
  }
}
