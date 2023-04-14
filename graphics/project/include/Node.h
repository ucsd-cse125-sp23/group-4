#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Model.h"

// a GameThing (tm)
// drawable, with a model and child nodes. maybe extend this class later
class Node {
public:
    glm::mat4 transformMtx;
    
    Model* model = nullptr;

    std::vector< Node* > childnodes;
};