/**************************************************
 * Model.h
 * basic data container for a Mesh + Material pair
 *****************************************************/
#include "client/graphics/Material.h"
#include "client/graphics/Mesh.h"

#ifndef __MODEL_H__
#define __MODEL_H__

struct Model {
  glm::mat4 modelMtx;  // applied first!

  Mesh* mesh;
  Material* material;

  void draw(const glm::mat4& viewProjMtx, const glm::mat4& viewMtx,
            const glm::mat4& transformMtx, const bool ignoreDepth = false) {
    if (!material || !mesh) return;

    GLuint shader = material->shader;

    // actiavte the shader program      ---
    glUseProgram(shader);

    material->setUniforms(viewProjMtx, viewMtx, transformMtx * modelMtx);

    if (ignoreDepth) glDisable(GL_DEPTH_TEST);

    mesh->draw();

    if (ignoreDepth) glEnable(GL_DEPTH_TEST);

    // deactivate the shader program    ---
    glUseProgram(0);
  }
};

#endif
