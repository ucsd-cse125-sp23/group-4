#pragma once

#include "core.h"
#include "AssimpNode.h"
#include "AssimpMesh.h"
#include "AssimpAnimation.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <map>

class AssimpModel {
public:
    AssimpModel();
    // ~AssimpModel();

    /** Load a model file using this node as root node.
      * Returns whether the model file is loaded properly.
      * If the node has already been initialized, returns false.
      */
    bool loadAssimp(const char* path);

    void useMesh();
    void useAnimation(unsigned int animationInd);
    void update(float deltaTimeInMs);
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void imGui();
private:
    std::string name;
    AssimpNode* rootNode;
    unsigned int numNode;
    std::map<std::string, AssimpNode*> nodeMap;
    std::map<std::string, AssimpJoint*> jointMap;
    std::vector<bool> meshVisibilities;
    std::vector<AssimpMesh*> meshes;
    bool isAnimated, isPaused;
    /** 0  - use mesh
      * 1+ - use animation (n-1)
      */
    int currentAnimation;
    std::vector<AssimpAnimation> animations;

    void loadAssimpHelperNode(AssimpNode* node, aiNode *aiNode, const aiScene *scene);
    void loadAssimpHelperMesh(AssimpMesh* mesh, aiMesh *aiMesh, const aiScene *scene);
    void loadAssimpHelperSkel(AssimpMesh* mesh, aiMesh *aiMesh, const aiScene *scene);
    void loadAssimpHelperAnim(const aiScene *scene);
    void loadAssimpHelperImgui();

    char** animModes;
};
