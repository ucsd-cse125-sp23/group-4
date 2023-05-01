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
        glm::vec3 rotConv = glm::radians(rotation);
        *mtx = translate(position) * glm::eulerAngleXYZ(rotConv.x, rotConv.y, rotConv.z) * glm::scale(scale);
    }

    glm::mat4 getRotationMtx() {
        glm::vec3 rotConv = rotation * float(M_PI) / 180.0f;
      return glm::eulerAngleXYZ(rotConv.x, rotConv.y, rotConv.z);
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


    // transform helpers

    void move(glm::vec3 movement) {
      // movement is in world space
      this->transform.position += movement;
      transform.updateMtx(&transformMtx);
    }

    void move_local(glm::vec3 movement) {
      // movement is in world space
      this->transform.position += glm::vec3(transform.getRotationMtx() * glm::vec4(movement, 1));
      transform.updateMtx(&transformMtx);
    }


    glm::vec3 getForward() {
      return glm::vec3(transform.getRotationMtx() * glm::vec4(0, 0, 1, 1));
    }
    glm::vec3 getRight() {
      return glm::vec3(transform.getRotationMtx() * glm::vec4(1, 0, 0, 1));
    }

};