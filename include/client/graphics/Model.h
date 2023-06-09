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
                    const glm::mat4& transformMtx) {
    if (!material || !mesh) return;

    GLuint shader = material->shader;

    // actiavte the shader program      ---
    glUseProgram(shader);

    material->setUniforms(viewProjMtx, viewMtx, transformMtx * modelMtx);

    glDepthFunc(depthFunction);

    if (!material->skybox) glEnable(GL_CULL_FACE);

    if (material->blending) {
      // Enable blending (transparency)
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    mesh->draw();

    glDepthFunc(GL_LESS);

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    // deactivate the shader program    ---
    glUseProgram(0);
  }
};
