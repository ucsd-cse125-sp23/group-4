/**************************************************
 * PlayerModel.h
 * implementation of Model
 * stores extra information for skeleton + skin + anims
 * and updating them
 *****************************************************/

#pragma once

#include <map>
#include <string>

#include "client/graphics/Model.h"
#include "client/graphics/SkinnedMesh.h"

class PlayerModel : public Model {
 public:
  SkinnedMesh* skin;

  virtual void setAnimation(std::string animName) {
    /* override me
    if (anims.find(animName) == anims.end()) {
      return;  // anim not found!
    }

    currAnim = anims[animName];*/
  }

  virtual void update(float dt) {
    /* override me
    // update anim + skeleton + skin
    if (currAnim) {
      currAnim->Update(dt);
      skel->SetPose(currAnim->GetCurrentPose());
    }

    skel->Update(dt);
    skin->updateSkin(skel);
    */
  }

  void draw(const glm::mat4& viewProjMtx, const glm::mat4& viewMtx,
            const glm::mat4& transformMtx) override {
    if (!material || !mesh) return;

    GLuint shader = material->shader;

    // actiavte the shader program      ---
    glUseProgram(shader);

    material->setUniforms(viewProjMtx, viewMtx, transformMtx * modelMtx);

    skin->draw();

    // deactivate the shader program    ---
    glUseProgram(0);
  }
};
