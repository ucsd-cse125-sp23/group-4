#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "client/graphics/Model.h"

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

    virtual void draw(const glm::mat4& viewProjMtx, const glm::mat4& modelMtx) {
        if (model) {
            model->draw(viewProjMtx, modelMtx);
        }

        /*if (Scene::_gizmos && _renderGizmo) { // nope. header file circular dependencies are annoying :(
            Scene::_globalSceneResources.models["_gz-xyz"]->draw(viewProjMtx, modelMtx, true);
        }*/
    }

    virtual void draw_debug(const glm::mat4& viewProjMtx,
                            const glm::mat4& modelMtx, bool gizmos,
                            Model* gizmo_mdl, Model* gizmoCube_mdl) {
        if (gizmos && _renderGizmo) {
            gizmo_mdl->draw(viewProjMtx, modelMtx, true);
        }
    }

    // flags to be used elsewhere
    bool _enabled = true;
    bool _renderGizmo = true;
};