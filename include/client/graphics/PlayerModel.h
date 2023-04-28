#pragma once

#include "Model.h"
#include "Skeleton.h"
#include "SkinnedMesh.h"
#include "AnimationPlayer.h"
#include <map>
#include <string>

class PlayerModel : public Model {
 public:
  Skeleton* skel;
  SkinnedMesh* skin;
  std::map<std::string, AnimationPlayer*> anims;

  void draw(const glm::mat4& viewProjMtx, const glm::mat4& modelMtx) {
    if (!material || !mesh) return;

    GLuint shader = material->shader;

    // actiavte the shader program      ---
    glUseProgram(shader);

    material->setUniforms(viewProjMtx, modelMtx * transformMtx);

    skin->draw();

    // deactivate the shader program    ---
    glUseProgram(0);
  }
};