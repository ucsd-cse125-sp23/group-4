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
    void useAnimation(int animationInd);
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
    /** -1  - use mesh
      * 0+ - use animation (n in total)
      */
    int currentAnimation;
    std::vector<AssimpAnimation> animations;

    /** Prepare a new AssimpNode
      * accTransform: accumulative transform from parent node
      * aiNode: aiNode from the importer corresponded to this node
      * scene: the scene created by the importer
      */
    void loadAssimpHelperNode(AssimpNode* node, glm::mat4 accTransform, aiNode *aiNode, const aiScene *scene);
    void loadAssimpHelperMesh(AssimpMesh* mesh, aiMesh *aiMesh, const aiScene *scene);
    void loadAssimpHelperSkel(AssimpMesh* mesh, aiMesh *aiMesh, const aiScene *scene);
    void loadAssimpHelperAnim(const aiScene *scene);
    void loadAssimpHelperImgui();

    char** animModes;
};
