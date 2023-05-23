#include "AssimpModel.h"

#include "AssimpMath.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>

AssimpModel::AssimpModel() 
        : name("N/A"), rootNode(nullptr), numNode(0), isAnimated(false), 
          isPaused(true), currentAnimation(-1), animModes(nullptr) { }

bool AssimpModel::loadAssimp(const char* path) {
    if(rootNode) {
        return false;
    }

    /* Load with assimp importer, check if loading failed */
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(
        path, 
        aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || 
       scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || 
       !scene->mRootNode) {
        printf("ERR::ASSIMP::%s\n", import.GetErrorString());
        return false;
    }

    name = std::string(path);
    name = name.substr(name.find_last_of('/')+1);

    nodeMap.clear();
    rootNode = new AssimpNode(numNode++);
    loadAssimpHelperNode(rootNode, glm::mat4(1.0f), scene->mRootNode, scene);
    loadAssimpHelperAnim(scene);
    loadAssimpHelperImgui();
    useMesh();
    betterView = glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(0.01f)), glm::vec3(0, -120, 0));
    return true;
}

void AssimpModel::loadAssimpHelperNode(AssimpNode* node, glm::mat4 accTransform, aiNode *aiNode, const aiScene *scene) {
    node->name = aiNode->mName.C_Str();
    nodeMap[node->name] = node;
    node->localTransform = aiMatToMat4x4(aiNode->mTransformation);
    node->accTransform = accTransform * node->localTransform;
    node->animationTransform = glm::mat4(1.0f);
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < aiNode->mNumMeshes; i++) {
        // TODO: this implmentation will create duplicate
        //       AssimpMesh if mesh has multiple reference aiNode
        aiMesh* aiMesh = scene->mMeshes[aiNode->mMeshes[i]];
        AssimpMesh* childMesh = new AssimpMesh();
        childMesh->node = node;
        loadAssimpHelperMesh(childMesh, aiMesh, scene);
        for(int j = 0; j < childMesh->joints.size(); j++) {
            childMesh->joints[j]->meshNode = node;
        }
        node->meshes.push_back(childMesh);
        meshes.push_back(childMesh);
        meshVisibilities.push_back(true);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < aiNode->mNumChildren; i++) {
        AssimpNode* childNode;
        if(nodeMap.find(aiNode->mChildren[i]->mName.C_Str()) != nodeMap.end()) {
            childNode = nodeMap.find(aiNode->mChildren[i]->mName.C_Str())->second;
        } else {
            childNode = new AssimpNode(numNode++);
        }
        childNode->parent = node;
        loadAssimpHelperNode(childNode, node->accTransform, aiNode->mChildren[i], scene);
        node->children.push_back(childNode);
    }
}

void AssimpModel::loadAssimpHelperMesh(AssimpMesh* mesh, aiMesh *aiMesh, const aiScene *scene) {
    // Is it possible to use vector.resize() & index?
    mesh->name = aiMesh->mName.C_Str();

    for(unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = glm::vec3(aiMesh->mVertices[i].x,
                                    aiMesh->mVertices[i].y,
                                    aiMesh->mVertices[i].z);
        vertex.normal = glm::vec3(aiMesh->mNormals[i].x,
                                  aiMesh->mNormals[i].y,
                                  aiMesh->mNormals[i].z);
        mesh->vertices.push_back(vertex);
        mesh->worldVerticies.push_back(vertex);
    }

    for(unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
        aiFace f = aiMesh->mFaces[i];
        for(unsigned int j = 0; j < f.mNumIndices; j++) {
            mesh->indices.push_back(f.mIndices[j]);
        }
    }

    if(aiMesh->mNumBones > 0) {
        loadAssimpHelperSkel(mesh, aiMesh, scene);
    }
}

void AssimpModel::loadAssimpHelperSkel(AssimpMesh* mesh, aiMesh *aiMesh, const aiScene *scene) {
    for(unsigned int i = 0; i < aiMesh->mNumBones; i++) {
        aiBone* bone = aiMesh->mBones[i];
        AssimpJoint* joint = new AssimpJoint();

        joint->ind = i;
        joint->name = bone->mName.C_Str();
        joint->matBindingInv = aiMatToMat4x4(bone->mOffsetMatrix);
        joint->matWorldTransform = glm::mat4(1.0f);
        for(int i = 0; i < bone->mNumWeights; i++) {
            VertexWeight vw;
            vw.vertexInd = bone->mWeights[i].mVertexId;
            vw.weight    = bone->mWeights[i].mWeight;
            joint->weights.push_back(vw);
        }
        if(nodeMap.find(joint->name) == nodeMap.end()) {
            nodeMap[joint->name] = new AssimpNode(numNode++);
        }
        joint->node = nodeMap[joint->name];

        mesh->joints.push_back(joint);
        nodeMap[joint->name]->joints.push_back(joint);
    }

    int maxBoneAffected = 0;
    std::vector<int> vertexBindedBones(mesh->vertices.size(), 0);
    for(int i = 0; i < mesh->joints.size(); i++) {
        AssimpJoint* joint = mesh->joints[i];
        for(int j = 0; j < joint->weights.size(); j++) {
            VertexWeight& vw = joint->weights[j];
            if(maxBoneAffected < vertexBindedBones[vw.vertexInd]) {
                maxBoneAffected = vertexBindedBones[vw.vertexInd];
            }
            switch (vertexBindedBones[vw.vertexInd]) {
            case 0:
                mesh->vertices[vw.vertexInd].boneInds.x = i;
                mesh->vertices[vw.vertexInd].boneWeights.x = vw.weight;
                break;
            case 1:
                mesh->vertices[vw.vertexInd].boneInds.y = i;
                mesh->vertices[vw.vertexInd].boneWeights.y = vw.weight;
                break;
            case 2:
                mesh->vertices[vw.vertexInd].boneInds.z = i;
                mesh->vertices[vw.vertexInd].boneWeights.z = vw.weight;
                break;
            case 3:
                mesh->vertices[vw.vertexInd].boneInds.w = i;
                mesh->vertices[vw.vertexInd].boneWeights.w = vw.weight;
                break;
            default:
                for(int n = 0; n < 3; n++) {
                    if(mesh->vertices[vw.vertexInd].boneWeights[n] < vw.weight) {
                        mesh->vertices[vw.vertexInd].boneInds[n] = i;
                        mesh->vertices[vw.vertexInd].boneWeights[n] = vw.weight;
                        break;
                    }
                }
                break;
            }
            vertexBindedBones[vw.vertexInd]++;
        }
    }
    // printf("%s MAX_BONE: %d\n", mesh->name.c_str(), maxBoneAffected);
    // printf("MAX_BONE_VERT: %u %u %u %u\n", mesh->vertices[vw.vertexInd].boneInds.x, mesh->vertices[vw.vertexInd].boneInds.y, mesh->vertices[vw.vertexInd].boneInds.z. mesh->vertices[vw.vertexInd].boneInds.w);
}

void AssimpModel::loadAssimpHelperAnim(const aiScene *scene) {
    for(int i = 0; i < scene->mNumAnimations; i++) {
        aiAnimation* aiAnimation = scene->mAnimations[i];
        AssimpAnimation animation;
        animation.name = aiAnimation->mName.C_Str();
        animation.duration = aiAnimation->mDuration;
        animation.tps = aiAnimation->mTicksPerSecond;
        for(int j = 0; j < aiAnimation->mNumChannels; j++) {
            aiNodeAnim* aiChannel = aiAnimation->mChannels[j];
            AssimpChannel channel;
            channel.name = aiChannel->mNodeName.C_Str();
            channel.node = nodeMap[channel.name];
            for(int k = 0; k < aiChannel->mNumPositionKeys; k++) {
                channel.positions.push_back(
                    std::make_pair(
                        aiChannel->mPositionKeys[k].mTime,
                        aiVecToVec3(aiChannel->mPositionKeys[k].mValue)
                    )
                );
            }
            for(int k = 0; k < aiChannel->mNumRotationKeys; k++) {
                channel.rotations.push_back(
                    std::make_pair(
                        aiChannel->mRotationKeys[k].mTime,
                        aiQuaternionToVec4(aiChannel->mRotationKeys[k].mValue)
                    )
                );
            }
            for(int k = 0; k < aiChannel->mNumScalingKeys; k++) {
                channel.scalings.push_back(
                    std::make_pair(
                        aiChannel->mScalingKeys[k].mTime,
                        aiVecToVec3(aiChannel->mScalingKeys[k].mValue)
                    )
                );
            }
            channel.extrapPre = AssimpChannel::loadExtrapMode(
                aiChannel->mPreState);
            channel.extrapPost = AssimpChannel::loadExtrapMode(
                aiChannel->mPostState);
            animation.channels.push_back(channel);
        }
        animations.push_back(animation);
    }
}

void AssimpModel::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    if(!rootNode) {
        return;
    }

    glUseProgram(shader);
    // TODO: remove this matrix when done with integration
    glm::mat4 betterViewMat = glm::scale(
        glm::translate(
            glm::mat4(1.0f),
            glm::vec3(0,-2,-0)
        ),
        glm::vec3(0.025f));
    for(int i = 0; i < meshes.size(); i++) {
        if(meshVisibilities[i]) {
            meshes[i]->draw(viewProjMtx, shader);
        }
    }

    if(drawNode) {
        glDepthFunc(GL_ALWAYS);
        rootNode->draw(viewProjMtx);
        glDepthFunc(GL_LEQUAL);
    }
}

void AssimpModel::loadAssimpHelperImgui() {
    animModes = new char*[1+animations.size()];
    for(int i = 0; i < animations.size()+1; i++) {
        animModes[i] = new char[256];
    }
    strcpy(animModes[0],"Mesh");
    for(int i = 0; i < animations.size(); i++) {
        strcpy(animModes[1+i],std::string("Animation ").append(std::to_string(i+1)).c_str());
    }

    for(auto& n : nodeMap) {
        nodeControlMap[n.first] = ControlInfo();
    }
}

void AssimpModel::imGui() {
    ImGui::Begin("Assimp Model Info");

    ImGui::Text("File: %s", name.c_str());
    long numTreeNode = 0;

    // Node Tree
    ImGui::SeparatorText("Node Tree");
    ImGui::Checkbox("draw node", &drawNode);
    if (ImGui::TreeNode((void*)(intptr_t)numTreeNode++, "Node Highlight (%lu)", nodeMap.size())) {
        for(auto it = nodeMap.begin(); it != nodeMap.end(); it++) {
            std::string name = it->first;
            ImGui::Checkbox(name.c_str(), &it->second->isMarked);
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode((void*)(intptr_t)numTreeNode++, "Node Tree (%lu)", nodeMap.size())) {
        rootNode->imGui();
        ImGui::TreePop();
    }

    // Meshes
    ImGui::SeparatorText("Meshes");
    if (ImGui::TreeNode((void*)(intptr_t)numTreeNode++, "Meshes (%lu)", meshes.size())) {
        for(int i = 0; i < meshes.size(); i++) {
            if (ImGui::TreeNode((void*)(intptr_t)i, "Mesh %d", i)) {
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
    if (ImGui::TreeNode((void*)(intptr_t)numTreeNode++, "Animations (%lu)", animations.size())) {
        for(int i = 0; i < animations.size(); i++) {
            if (ImGui::TreeNode((void*)(intptr_t)i, "Animation %d", i)) {
                animations[i].imGui();
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
    ImGui::SeparatorText("Current Animation");
        if (ImGui::Button(isPaused ? "Start" : "Pause")) {
            if(isAnimated) {
                isPaused = !isPaused;
            }
        }
    ImGui::Text("Animation: %lu", animations.size());
    if(ImGui::BeginListBox("Current Anim")) {
        for(int i = 0; i < animations.size()+1; i++) {
            const bool isSelected = (currentAnimation+1 == i);
            if (ImGui::Selectable(animModes[i], isSelected)) {
                if(i == 0) {
                    useMesh();
                } else {
                    useAnimation(i-1);
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

void AssimpModel::useMesh() {
    useAnimation(-1);
}

bool AssimpModel::useAnimation(int animationInd) {
    printf("Assimp: Animation - Using %d\t: %s\n", animationInd,
           animationInd < 0 || animationInd >= animations.size()
               ? "Mesh | Unknown"
               : animations[animationInd].name.c_str());

    if(animationInd < 0) {
        currentAnimation = -1;
        isAnimated = false;
        isPaused = true;
        for (auto const& x : nodeMap) {
            x.second->animationTransform = x.second->localTransform;
        }
        rootNode->update(glm::mat4(1.0f));
        for(int i = 0; i < meshes.size(); i++) {
            meshes[i]->update();
        }
        return true;
    }
    if(animationInd >= animations.size()) {
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
    if(!isAnimated || isPaused) {
        return;
    }

    animations[currentAnimation].update(deltaTimeInMs);
    rootNode->update(glm::mat4(1.0f));
    for(int i = 0; i < meshes.size(); i++) {
        meshes[i]->update();
    }
}

void AssimpModel::imGuiJointMenu() {
    for(auto& x : nodeMap) {
        AssimpNode* node = x.second;
        const std::string name = node->name;
        if(ImGui::TreeNode(name.c_str())) {
            bool& isControlled = nodeControlMap[x.first].useControl;
            glm::vec3& pose = nodeControlMap[x.first].pose;
            glm::vec3& scale = nodeControlMap[x.first].scale;
            glm::vec3& offset = nodeControlMap[x.first].offset;
            ImGui::Checkbox("Control", &isControlled);
            ImGui::SliderFloat("Pos X", &offset.x, -5.0f, 5.0f, "%.2f");
            ImGui::SliderFloat("Pos Y", &offset.y, -5.0f, 5.0f, "%.2f");
            ImGui::SliderFloat("Pos Z", &offset.z, -5.0f, 5.0f, "%.2f");
            ImGui::SliderFloat("Rot X", &pose.x, -360.0f, 360.0f, "%.2f");
            ImGui::SliderFloat("Rot Y", &pose.y, -360.0f, 360.0f, "%.2f");
            ImGui::SliderFloat("Rot Z", &pose.z, -360.0f, 360.0f, "%.2f");
            ImGui::SliderFloat("Sca X", &scale.x, 0.1f, 2.0f, "%.2f");
            ImGui::SliderFloat("Sca Y", &scale.y, 0.1f, 2.0f, "%.2f");
            ImGui::SliderFloat("Sca Z", &scale.z, 0.1f, 2.0f, "%.2f");
            ImGui::TreePop();
            if(isControlled) {
                glm::vec3 poseR = glm::radians(pose);
                glm::mat4 result(1.0f);
                result = glm::translate(result, offset);
                result = glm::rotate(result, poseR.x, glm::vec3(1.0f, 0.0f, 0.0f));
                result = glm::rotate(result, poseR.y, glm::vec3(0.0f, 1.0f, 0.0f));
                result = glm::rotate(result, poseR.z, glm::vec3(0.0f, 0.0f, 1.0f));
                result = glm::scale(result, scale);
                node->animationTransform = result;
                rootNode->update(glm::mat4(1.0f));
                for(int i = 0; i < meshes.size(); i++) {
                    meshes[i]->update();
                }
            }
        }
    }
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
    const glm::mat4& transformMtx) {

    if (!material) {
        return;
    }

    GLuint shader = material->shader;

    glUseProgram(shader);
    material->setUniforms(viewProjMtx, viewMtx,
                          transformMtx * modelMtx * betterView);
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i]->draw();
    }
    glUseProgram(0);
}

void AssimpModel::draw(const glm::mat4& viewProjMtx, const glm::mat4& viewMtx,
                       const glm::mat4& transformMtx, const bool ignoreDepth) {

    if (!material) {
        return;
    }

    GLuint shader = material->shader;
    glUseProgram(shader);

    if (ignoreDepth) glDisable(GL_DEPTH_TEST);
    material->setUniforms(viewProjMtx, viewMtx,
                          transformMtx * modelMtx * betterView);
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i]->draw();
    }
    if (ignoreDepth) glEnable(GL_DEPTH_TEST);

    glUseProgram(0);
}

void AssimpModel::draw() {
    if (!isAnimated) {
        useMesh();
    }

    for (int i = 0; i < meshes.size(); i++) {
        meshes[i]->draw();
    }
}
