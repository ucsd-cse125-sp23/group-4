/**************************************************
 * Model.h
 * basic data container for a Mesh + Material pair
 *****************************************************/

#pragma once

#include "client/graphics/Material.h"
#include "client/graphics/Mesh.h"

struct Model {
  glm::mat4 modelMtx;  // applied first!

  Mesh* mesh;
  Material* material;

  GLenum depthFunction = GL_LESS;

  virtual void draw(const glm::mat4& viewProjMtx, const glm::mat4& viewMtx,
            const glm::mat4& transformMtx, const bool ignoreDepth = false) {
    if (!material || !mesh) return;

    GLuint shader = material->shader;

    // actiavte the shader program      ---
    glUseProgram(shader);

    material->setUniforms(viewProjMtx, viewMtx, transformMtx * modelMtx);

    glDepthFunc(depthFunction);

    mesh->draw();

    glDepthFunc(GL_LESS);

    // deactivate the shader program    ---
    glUseProgram(0);
  }
};
