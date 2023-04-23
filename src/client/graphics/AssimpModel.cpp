#include "AssimpModel.h"

AssimpModel::AssimpModel() : name("N/A"), rootNode(nullptr) { }

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

    rootNode = new AssimpNode();
    AssimpNode::count = 0;
    AssimpNode::nodeMap.clear();
    rootNode->loadFromAssimp(scene->mRootNode, scene);
    return true;
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
    rootNode->draw(viewProjMtx * betterViewMat, shader);
}