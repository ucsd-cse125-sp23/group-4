#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <map>
#include <string>
#include <vector>

#include "client/graphics/AssimpNode.h"
#include "client/graphics/core.h"

enum ASSIMP_EXTRAP_MODE { DEFAULT, CONSTANT, LINEAR, CYCLE };

struct AssimpKeyframe {
  float time, val;

  float invT;
  float tangent = 0.0f;
  float a, b, c, d;
};

struct AssimpChannel {
 public:
  ASSIMP_EXTRAP_MODE extrapIn, extrapOut;
  std::vector<AssimpKeyframe> keyframes;

  void prep();
  float eval(float t) const;

 private:
  enum ASSIMP_TANGENT_MODE { FLAT, LINEAR, SMOOTH };
  static const ASSIMP_TANGENT_MODE TANGENT_MODE = ASSIMP_TANGENT_MODE::LINEAR;

  float timeStart, timeEnd;
};

struct AssimpAnimNode {
 public:
  AssimpAnimNode(const aiNodeAnim* const aiNodeAnim);
  void update(double currentTick);

  std::string name;
  glm::vec3 pos, sca;
  glm::vec4 rot;

 private:
  AssimpChannel posX, posY, posZ, scaX, scaY, scaZ;
  std::vector<double> rotT;
  std::vector<glm::vec4> rotQ;
  ASSIMP_EXTRAP_MODE extrapIn, extrapOut;
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
  std::vector<AssimpAnimNode> nodes;

  /** Update the state of the animation
   * deltaTimeInMs: time elapsed since last rendering in miliseconds
   */
  void update(double deltaTimeInMs, std::map<std::string, AssimpNode*> nodeMap);
  /** Reset the animation */
  void restart();
  void imGui();

 private:
  double currentTimeInMs;
};
