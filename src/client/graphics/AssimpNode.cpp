#include "AssimpNode.h"

#include "AssimpMath.h"

unsigned int AssimpNode::count = 0;
std::map<aiNode*, AssimpNode*> AssimpNode::nodeMap;

AssimpNode::AssimpNode()
        : id(++count), isLoaded(false), parent(nullptr), localTransform(1.0f) {
    name = std::to_string(id);
}

void AssimpNode::loadFromAssimp(aiNode* node, const aiScene* scene) {
    name = node->mName.C_Str();
    if (node->mParent != nullptr) {
        if (nodeMap.find(node->mParent) != nodeMap.end()) {
            parent = nodeMap[node->mParent];
        } else {
        // this should not happen if the tree hirarechy is correct
        }
    }
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // TODO: this implmentation will create duplicate
        //       AssimpMesh if mesh has multiple reference node
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        AssimpMesh* childMesh = new AssimpMesh();
        childMesh->loadFromAssimp(mesh, scene);
        meshes.push_back(childMesh);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        AssimpNode* childNode = new AssimpNode();
        childNode->loadFromAssimp(node->mChildren[i], scene);
        children.push_back(childNode);
    }
    localTransform = aiMatToMat4x4(node->mTransformation);

    nodeMap[node] = this;
}

void AssimpNode::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i]->draw(viewProjMtx * localTransform, shader);
    }
    for (unsigned int i = 0; i < children.size(); i++) {
        children[i]->draw(viewProjMtx * localTransform, shader);
    }
}