#pragma once

#define _USE_MATH_DEFINES
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <math.h>
#include <vector>


#include "client/graphics/Node.h"
//#include "Input.h"

struct Transform {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    void updateMtx(glm::mat4* mtx) {
        // update mat4 in memory:
        glm::vec3 rotConv = rotation * float(M_PI) / 180.0f;
        *mtx = translate(position) * glm::eulerAngleXYZ(rotConv.x, rotConv.y, rotConv.z) * glm::scale(scale);
    }
};

// a GameThing (tm)
class GameThing : public Node {
public:
    Transform transform;

    virtual void update(float dt) {
        // --- example ---
        transform.rotation += glm::vec3(0, 30 * dt, 0); // spin on y axis



        transform.updateMtx(&transformMtx); // needed to update node matrix
    }

};