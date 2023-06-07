#include "AssimpModel.h"

#include <deque>
#include <glm/gtc/matrix_transform.hpp>

// #define STB_IMAGE_IMPLEMENTATION
#include "client/graphics/AssimpMath.h"
#include "client/graphics/imported/parallel.h"
#include "client/graphics/imported/stb_image.h"

AssimpModel::AssimpModel()
    : name("N/A"), path(""), rootNode(nullptr), numNode(0) {}

AssimpModel::AssimpModel(const AssimpModel& am)
    : rootNode(nullptr), numNode(0) {
  if (!loadAssimp(am.path.c_str())) {
    printf(
        "Assimp: [ERROR] loading from reference model failed: cannot find path "
        "%s\n",
        am.path.c_str());
  }

  material = am.material;
}

AssimpModel::~AssimpModel() {
  for (auto& kv : nodeMap) {
    AssimpNode* node = kv.second;
    delete node;
  }

  name = "destructed";
  path = name;
  numNode = 0;
  rootNode = nullptr;
  nodeMap.clear();
  meshes.clear();
}

bool AssimpModel::loadAssimp(const char* path) {
  if (rootNode) {
    return false;
  }
  printf("Assimp: [LOG] loading file: %s\n", path);

  /* Load with assimp importer, check if loading failed */
  Assimp::Importer import;
  const aiScene* scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    printf("Assimp: [ERROR] assimp loader err msg: %s\n",
           import.GetErrorString());
    return false;
  }

  name = std::string(path);
  name = name.substr(name.find_last_of('/') + 1);
  this->path = std::string(path);

  prepareNodes(scene);
  if (numNode != nodeMap.size()) {
    printf(
        "Assimp: [ERROR] duplicate node: processed %u nodes, has %u unique "
        "name nodes\n",
        numNode, nodeMap.size());
    printf("Assimp: Aborting loading %s\n", name.c_str());
    return false;
  }

  std::vector<std::pair<aiMesh*, AssimpMesh*>> meshQueue;
  if (!populateNode(scene, scene->mRootNode, glm::mat4(1.0f), meshQueue)) {
    printf("Assimp: Aborting loading %s\n", name.c_str());
    return false;
  }
  if (scene->mNumMeshes != meshQueue.size()) {
    printf(
        "Assimp: [ERROR] unmatched mesh count: should have %u meshes, "
        "processed %u meshes\n",
        scene->mNumMeshes, meshQueue.size());
    printf("Assimp: Aborting loading %s\n", name.c_str());
    return false;
  }
  for (unsigned int i = 0; i < meshQueue.size(); i++) {
    if (!populateMesh(scene, meshQueue[i].first, meshQueue[i].second)) {
      printf("Assimp: Aborting loading %s\n", name.c_str());
      return false;
    }
  }

  bool animIsPlayer = false;
  if (!animation.init(scene, nodeMap, animIsPlayer)) {
    printf("Assimp: Aborting loading %s\n", name.c_str());
    return false;
  }
  printf("Assimp: [LOG] %s %s\n", name.c_str(),
         animIsPlayer ? "matches a player model"
                      : "does not match a player model");

  betterView = glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(0.01f)),
                              glm::vec3(0, 0, 0));
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
      mesh->outPos.push_back(mesh->pos[i]);
      mesh->norm.push_back(glm::vec4(aiMesh->mNormals[i].x,
                                     aiMesh->mNormals[i].y,
                                     aiMesh->mNormals[i].z, 0.0f));
      mesh->outNorm.push_back(mesh->norm[i]);
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
      mesh->outPos.push_back(mesh->pos[i]);
      mesh->norm.push_back(glm::vec4(aiMesh->mNormals[i].x,
                                     aiMesh->mNormals[i].y,
                                     aiMesh->mNormals[i].z, 0.0f));
      mesh->outNorm.push_back(mesh->norm[i]);
      mesh->uvs.push_back(glm::vec2(0.0f));
    }
  }
  if (mesh->pos.size() != mesh->outPos.size() ||
      mesh->norm.size() != mesh->outNorm.size() ||
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

void AssimpModel::setAnimation(std::string animName) {
  animation.useAnimation(animName);
}

void AssimpModel::update(float dt) {
  if (!rootNode) return;

  animation.update(dt);

  for (unsigned int i = 0; i < meshes.size(); i++) {
    for (unsigned int v = 0; v < meshes[i]->pos.size(); v++) {
      meshes[i]->outPos[v] = glm::vec4(0.0f);
      meshes[i]->outNorm[v] = glm::vec4(0.0f);
    }
  }
  rootNode->update(glm::mat4(1.0f));
  for (unsigned int i = 0; i < meshes.size(); i++) {
    for (unsigned int v = 0; v < meshes[i]->pos.size(); v++) {
      meshes[i]->outPos[v] /= meshes[i]->outPos[v][3];
      meshes[i]->outNorm[v] = glm::normalize(meshes[i]->outNorm[v]);
    }
  }
}

void AssimpModel::draw(const glm::mat4& viewProjMtx, const glm::mat4& viewMtx,
                       const glm::mat4& transformMtx) {
  if (!material) {
    return;
  }

  GLuint shader = material->shader;
  glUseProgram(shader);

  glm::vec4 tempDiffuse = material->diffuse;
  glm::vec4 tempSpecular = material->specular;
  /*glm::vec4 tempAmbient = material->ambient;
  glm::vec4 tempEmission = material->emission;
  float tempShininess = material->shininess;*/

  for (int i = 0; i < meshes.size(); i++) {
    material->diffuse = meshes[i]->diffuse;
    material->specular = meshes[i]->specular;
    // material->ambient = meshes[i]->ambient;
    // material->emission = meshes[i]->emissive;
    // material->shininess = meshes[i]->shininess;
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

  material->diffuse = tempDiffuse;
  material->specular = tempSpecular;
  /*material->ambient = tempAmbient;
  material->emission = tempEmission;
  material->shininess = tempShininess;*/

  glUseProgram(0);
}

void AssimpModel::draw() {
  for (int i = 0; i < meshes.size(); i++) {
    meshes[i]->draw();
  }
}
