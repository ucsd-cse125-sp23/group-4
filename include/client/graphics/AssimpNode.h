#pragma once

#include "AssimpMesh.h"

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <map>

class AssimpMesh;
struct AssimpJoint;

class AssimpNode {
public:
    AssimpNode(unsigned int id);

    /** Numeric identifier that is unique for nodes in an AssimpModel */
    const unsigned int id;
    std::string name;
    std::vector<AssimpMesh*> meshes;
    /** This matrix is relative to parent's position */
    glm::mat4 localTransform;
    /** This matrix make node's meshes to go from node space to global space */
    glm::mat4 accTransform;
    /** This matrix is calculated by the animation system */
    glm::mat4 animationTransform;

    glm::mat4 matWorldTransform;

    AssimpNode* parent;
    std::vector<AssimpNode*> children;
    std::vector<AssimpJoint*> joints;

    void update(const glm::mat4& parentWorldTransform);
    void imGui();
};
