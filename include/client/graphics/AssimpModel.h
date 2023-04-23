#include "core.h"
#include "AssimpNode.h"
#include "AssimpMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
private:
    std::string name;
    AssimpNode* rootNode;
    std::map<std::string,AssimpNode*> nodeMap;
};