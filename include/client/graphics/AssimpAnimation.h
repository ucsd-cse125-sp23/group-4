#pragma once

#include <map>
#include <string>
#include <utility>  // pair
#include <vector>

#include "client/graphics/AssimpNode.h"
#include "client/graphics/core.h"

enum ASSIMP_EXTRAP_MODE { DEFAULT, CONSTANT, LINEAR, CYCLE };

struct AssimpChannel {
  std::string name;
  AssimpNode* node;

  ASSIMP_EXTRAP_MODE extrapPre, extrapPost;
  std::vector<std::pair<double, glm::vec3>> positions;
  std::vector<std::pair<double, glm::vec4>> rotations;
  std::vector<std::pair<double, glm::vec3>> scalings;

  void eval(double currentTick);
  glm::mat4 getMatrixInterpolate(int lastEarlyTickInd, double tick);
  static glm::mat4 getMatrix(glm::vec3& pos, glm::vec4& rot, glm::vec3& sca);
  static ASSIMP_EXTRAP_MODE loadExtrapMode(const aiAnimBehaviour& b);
  static std::string extraModeToString(ASSIMP_EXTRAP_MODE mode);
};

/** The AssimpAnimation class for storing and evaluating an animation. */
class AssimpAnimation {
 public:
  AssimpAnimation();

  /** name of the animation */
  std::string name;
  /** duration of the animation (in ticks) */
  double duration;
  /** ticks per second of the animation */
  double tps;
  std::vector<AssimpChannel> channels;

  /** Update the state of the animation
   * deltaTimeInMs: time elapsed since last rendering in miliseconds
   */
  void update(double deltaTimeInMs);
  /** Reset the animation */
  void restart();
  void imGui();

 private:
  double currentTimeInMs;
};
