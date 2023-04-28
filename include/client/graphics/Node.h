#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Model.h"

// a GameThing (tm)
// drawable, with a model and child nodes. maybe extend this class later
class Node {
public:
    std::string name = "node";
    Node() { }
    Node(std::string name) { this->name = name; }

    glm::mat4 transformMtx;

    Model* model = nullptr;

    std::vector< Node* > childnodes;

    // flags to be used elsewhere
    bool _enabled = true;
    bool _renderGizmo = true;
};