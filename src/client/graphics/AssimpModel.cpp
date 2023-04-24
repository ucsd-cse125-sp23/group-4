#include "AssimpModel.h"

#include "AssimpMath.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

AssimpModel::AssimpModel() 
        : name("N/A"), rootNode(nullptr), numNode(0), isAnimated(false), 
          isPaused(true), currentAnimation(0), animModes(nullptr) { }

bool AssimpModel::loadAssimp(const char* path) {
    if(rootNode) {
        return false;
    }

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
    loadAssimpHelperNode(rootNode, scene->mRootNode, scene);
    loadAssimpHelperAnim(scene);
    loadAssimpHelperImgui();
    useAnimation(0);
    return true;
}

void AssimpModel::loadAssimpHelperNode(AssimpNode* node, aiNode *aiNode, const aiScene *scene) {
    node->name = aiNode->mName.C_Str();
    nodeMap[node->name] = node;
    node->localTransform = aiMatToMat4x4(aiNode->mTransformation);
    node->animationTransform = glm::mat4(1.0f);
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < aiNode->mNumMeshes; i++) {
        // TODO: this implmentation will create duplicate
        //       AssimpMesh if mesh has multiple reference aiNode
        aiMesh* mesh = scene->mMeshes[aiNode->mMeshes[i]];
        AssimpMesh* childMesh = new AssimpMesh();
        childMesh->node = node;
        loadAssimpHelperMesh(childMesh, mesh, scene);
        for(int j = 0; j < childMesh->joints.size(); j++) {
            childMesh->joints[j]->meshNode = node;
        }
        node->meshes.push_back(childMesh);
        meshes.push_back(childMesh);
        meshVisibilities.push_back(true);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < aiNode->mNumChildren; i++) {
        AssimpNode* childNode = new AssimpNode(numNode++);
        childNode->parent = node;
        loadAssimpHelperNode(childNode, aiNode->mChildren[i], scene);
        node->children.push_back(childNode);
    }
}

void AssimpModel::loadAssimpHelperMesh(AssimpMesh* mesh, aiMesh *aiMesh, const aiScene *scene) {
    // Is it possible to use vector.resize() & index?

    for(unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = glm::vec3(aiMesh->mVertices[i].x,
                                    aiMesh->mVertices[i].y,
                                    aiMesh->mVertices[i].z);
        vertex.normal = glm::vec3(aiMesh->mNormals[i].x,
                                  aiMesh->mNormals[i].y,
                                  aiMesh->mNormals[i].z);
        mesh->vertices.push_back(vertex);
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
        joint->node = nodeMap[joint->name];

        mesh->joints.push_back(joint);
        nodeMap[joint->name]->joints.push_back(joint);
    }

    std::vector<int> vertexBindedBones(mesh->vertices.size(), 0);
    for(int i = 0; i < mesh->joints.size(); i++) {
        AssimpJoint* joint = mesh->joints[i];
        for(int j = 0; j < joint->weights.size(); j++) {
            VertexWeight& vw = joint->weights[j];
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
                break;
            }
            vertexBindedBones[vw.vertexInd]++;
        }
    }
}

void AssimpModel::loadAssimpHelperAnim(const aiScene *scene) {
    for(int i = 0; i < scene->mNumAnimations; i++) {
        aiAnimation* aiAnimation = scene->mAnimations[i];
        AssimpAnimation animation;
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

    // TODO: remove this matrix when done with integration
    glm::mat4 betterViewMat = glm::scale(
        glm::translate(
            glm::mat4(1.0f),
            glm::vec3(0,-2,-0)
        ),
        glm::vec3(0.025f));
    for(int i = 0; i < meshes.size(); i++) {
        if(meshVisibilities[i]) {
            meshes[i]->draw(viewProjMtx * betterViewMat, shader);
        }
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
}

void AssimpModel::imGui() {
    ImGui::Begin("Assimp Model Info");

    ImGui::Text("File: %s", name.c_str());
    long numTreeNode = 0;

    // Node Tree
    ImGui::SeparatorText("Node Tree");
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
            const bool isSelected = (currentAnimation == i);
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

    ImGui::End();
}

void AssimpModel::useMesh() {
    printf("Using mesh\n");
    isAnimated = false;
    isPaused = true;
    for(int i = 0; i < meshes.size(); i++) {
        meshes[i]->setDraw(true);
    }
    currentAnimation = 0;
}

void AssimpModel::useAnimation(unsigned int animationInd) {
    printf("Using %d\n", animationInd);
    isAnimated = true;
    isPaused = true;
    for(int i = 0; i < meshes.size(); i++) {
        meshes[i]->setDraw(false);
    }
    currentAnimation = animationInd + 1;
    animations[animationInd].restart();
}

void AssimpModel::update(float deltaTimeInMs) {
    if(!isAnimated || isPaused) {
        return;
    }

    animations[currentAnimation-1].update(deltaTimeInMs);
    rootNode->update(glm::mat4(1.0f));
    for(int i = 0; i < meshes.size(); i++) {
        meshes[i]->update();
    }
}
