#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <map>
#include <string>
#include <vector>

#include "client/graphics/AssimpNode.h"
#include "client/graphics/core.h"

enum class A_ANIM_EXTRAP { DEFAULT, CONSTANT, LINEAR, CYCLE };
enum class A_ANIM_TANGENT { FLAT, LINEAR, SMOOTH };
enum class A_ANIM_QUART_INTERP { NONE, LERP, SLERP };

struct AssimpKeyframe {
  float time, val;

  float invT;
  float tangent = 0.0f;
  float a, b, c, d;
};

struct AssimpChannel {
 public:
  A_ANIM_EXTRAP extrapIn, extrapOut;
  std::vector<AssimpKeyframe> keyframes;

  void prep();
  float eval(float t, const A_ANIM_EXTRAP& extrapIn,
             const A_ANIM_EXTRAP& extrapOut) const;

 private:
  static const A_ANIM_TANGENT TANGENT_MODE = A_ANIM_TANGENT::LINEAR;

  float timeStart, timeEnd, timeLen;
};

struct AssimpRotKeyframe {
  float time;
  glm::vec4 val;

  // u = (t - t_a) / (t_b - t_a) in [0,1]
  float invT;

  // Lerp(u,a,b) = a + u(b-a)
  glm::vec4 diffVal;

  // Slerp(u,a,b) = 1/sin(theta) * (sin((1-u) * theta) * a + sin(t * theta) * b)
  //   where theta = acos(a,b)
  //   flip b if dot(a,b) < 0
  bool useLerp = true;
  glm::vec4 nextVal;
  float invTheta, theta;
};

struct AssimpRotChannel {
 public:
  std::vector<AssimpRotKeyframe> keyframes;

  void prep();
  glm::vec4 eval(float t, const A_ANIM_EXTRAP& extrapIn,
                 const A_ANIM_EXTRAP& extrapOut) const;

 private:
  static const A_ANIM_QUART_INTERP INTERP_MODE = A_ANIM_QUART_INTERP::SLERP;

  float timeStart, timeEnd, timeLen;
};

struct BlendPose {
  glm::vec3 pos1, pos2;
  glm::vec4 rot1, rot2;
  glm::vec3 sca1, sca2;
};

struct AssimpAnimNode {
 public:
  AssimpAnimNode(const aiNodeAnim* const aiNodeAnim, bool& ok);

  void update(double currentTick);

  std::string name;

  /** output of update result */
  glm::vec3 pos, sca;
  glm::vec4 rot;

 private:
  AssimpChannel posX, posY, posZ, scaX, scaY, scaZ;
  AssimpRotChannel rotQ;
  A_ANIM_EXTRAP extrapIn, extrapOut;
};

/** The AssimpAnimationClip class for storing and evaluating an animation. */
class AssimpAnimationClip {
 public:
  AssimpAnimationClip();

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
  void update(double currentTimeInMs,
              std::map<std::string, AssimpNode*> nodeMap);
  void update(double currentTimeInMs, std::map<std::string, BlendPose>& poseMap,
              bool isBase);
};

class AssimpAnimation {
 public:
  enum class PLAYER_AC {
    IDLE = 0,
    WALK,
    JUMP,
    TAG,
    TRIP,
    FALL,
    LOBBY1,
    LOBBY2,
    LOBBY3,
    LOBBY4,
    PLACE1_1,
    PLACE1_2,
    PLACE2_1,
    PLACE2_2,
    PLACE3_1,
    PLACE3_2,
    PLACE4_1,
    PLACE4_2
  };
  static const char EMOTE_CYC_SUFFIX[];
  static const std::map<PLAYER_AC, std::string> AC_TO_NAME;
  static const std::map<std::string, PLAYER_AC> NAME_TO_AC;

  /** Bind animation player to a AssimpModel through passing nodeMap;
   *   returns false if binding fails;
   *   isPlayer is set to whether this AssimpModel supports player actions.
   * IMPORTANT:
   *   DO NOT modify AssimpModel after binding the animation player.
   */
  bool init(const aiScene* const scene,
            const std::map<std::string, AssimpNode*> nodeMap, bool& isPlayer);
  void update(float deltaTimeInMs);
  void useAnimation(std::string animName);

 private:
  static const float MS_DISSOLVE, MS_JUMP;
  static const std::vector<std::string> NODES_TAG;

  void blendAnimation(const PLAYER_AC& ac);
  void setEmote(const PLAYER_AC& ac);

  bool isPlayer = false;
  std::map<std::string, AssimpAnimationClip> animMap;
  std::map<std::string, AssimpNode*> nodeMap;

  // Playback props
  float currTimeInMs = 0.0f;
  std::string currAnimName;

  // Blending props - dissolve (idle, walk, jump)
  PLAYER_AC baseAnim, dissolveAnim;
  std::map<std::string, BlendPose> poseMap;
  bool isDissolve = false;
  bool isDissolveReversed = false;
  float timeDissolve = 0.0f;
  float msCurrent = 1.0f;
  // jump takes jump animation duration + MS_JUMP amount of time
  bool isJump = false;
  float timeJump = 0.0f;

  // Blending props - replace (tag)
  // tag takes tag animatio duration amount of time (no blending for now)
  bool isTag = false, isTagReversed = false;
  float timeTag = 0.0f;
  std::vector<size_t> ind_tag;

  // Cycle emote props (winning animation)
  bool isEmote = false, isEmoteCyc = false;
};
