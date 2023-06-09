#include "AssimpAnimation.h"

#include "AssimpMath.h"

void AssimpChannel::prep() {
  if (keyframes.empty()) {
    return;
  }
  const glm::mat4 INV_MAT(2.0, -3.0, 0.0, 1.0, -2.0, 3.0, 0.0, 0.0, 1.0, -2.0,
                          1.0, 0.0, 1.0, -1.0, 0.0, 0.0);
  glm::vec4 ABCD(0.0f);
  glm::vec4 goal(0.0f);

  timeStart = keyframes[0].time;
  timeEnd = keyframes[keyframes.size() - 1].time;
  timeLen = timeEnd - timeStart;
  if (keyframes.size() == 1) {
    AssimpKeyframe& kf = keyframes[0];
    kf.invT = 1.0f;
    goal = glm::vec4(kf.val, kf.val, 0.0f, 0.0f);
    ABCD = INV_MAT * goal;
    kf.a = ABCD.x;
    kf.b = ABCD.y;
    kf.c = ABCD.z;
    kf.d = ABCD.w;
    return;
  }

  for (size_t i = 1; i < keyframes.size() - 1; i++) {
    AssimpKeyframe& kf = keyframes[i];
    switch (TANGENT_MODE) {
      case A_ANIM_TANGENT::SMOOTH:
        kf.tangent = (keyframes[i + 1].val - keyframes[i - 1].val) /
                     (keyframes[i + 1].time - keyframes[i - 1].time);
        break;
      case A_ANIM_TANGENT::LINEAR:
        kf.tangent =
            (keyframes[i + 1].val - kf.val) / (keyframes[i + 1].time - kf.time);
        break;
      case A_ANIM_TANGENT::FLAT:
      default:
        return;
    }
  }
  if (TANGENT_MODE != A_ANIM_TANGENT::FLAT) {
    keyframes[0].tangent = (keyframes[1].val - keyframes[0].val) /
                           (keyframes[1].time - keyframes[0].time);
    const size_t kfSize = keyframes.size();
    keyframes[kfSize - 1].tangent =
        (keyframes[kfSize - 1].val - keyframes[kfSize - 2].val) /
        (keyframes[kfSize - 1].time - keyframes[kfSize - 2].time);
  }

  for (size_t i = 0; i < keyframes.size() - 1; i++) {
    AssimpKeyframe& kf = keyframes[i];
    AssimpKeyframe& kfNext = keyframes[i + 1];
    kf.invT = 1.0 / (kfNext.time - kf.time);
    goal = glm::vec4(kf.val, kfNext.val, (kfNext.time - kf.time) * kf.tangent,
                     (kfNext.time - kf.time) * kfNext.tangent);
    ABCD = INV_MAT * goal;
    kf.a = ABCD.x;
    kf.b = ABCD.y;
    kf.c = ABCD.z;
    kf.d = ABCD.w;
  }
}

float AssimpChannel::eval(float t, const A_ANIM_EXTRAP& extrapIn,
                          const A_ANIM_EXTRAP& extrapOut) const {
  if (keyframes.size() == 1) {
    return keyframes[0].val;
  }

  if (t < timeStart) {
    switch (extrapIn) {
      case A_ANIM_EXTRAP::CONSTANT:
        return keyframes[0].val;
      case A_ANIM_EXTRAP::LINEAR:
        return keyframes[0].val + keyframes[0].tangent * (timeStart - t);
      case A_ANIM_EXTRAP::CYCLE:
      case A_ANIM_EXTRAP::DEFAULT:
      default:
        t = std::fmod(t - timeStart, timeLen) + timeStart;
        if (t < timeStart) {
          t += timeLen;
        }
    }
  } else if (t > timeEnd) {
    switch (extrapOut) {
      case A_ANIM_EXTRAP::CONSTANT:
        return keyframes[keyframes.size() - 1].val;
      case A_ANIM_EXTRAP::LINEAR:
        return keyframes[keyframes.size() - 1].val +
               keyframes[keyframes.size() - 1].tangent * (t - timeEnd);
      case A_ANIM_EXTRAP::CYCLE:
      case A_ANIM_EXTRAP::DEFAULT:
      default:
        t = std::fmod(t - timeStart, timeLen) + timeStart;
    }
  }

  size_t ind = 0;
  for (; ind < keyframes.size(); ind++) {
    if (t < keyframes[ind].time) {
      ind--;
      break;
    }
  }

  const AssimpKeyframe& kf = keyframes[ind];
  float uTime = (t - kf.time) * kf.invT;
  return uTime * (uTime * (uTime * kf.a + kf.b) + kf.c) + kf.d;
}

A_ANIM_QUART_INTERP AssimpRotChannel::INTERP_MODE = A_ANIM_QUART_INTERP::LERP;

void AssimpRotChannel::prep() {
  if (keyframes.empty()) {
    return;
  }

  timeStart = keyframes[0].time;
  timeEnd = keyframes[keyframes.size() - 1].time;
  timeLen = timeEnd - timeStart;
  if (keyframes.size() == 1) {
    AssimpRotKeyframe& rkf = keyframes[0];
    rkf.invT = 1.0f;
    rkf.diffVal = glm::vec4(0.0f);
    return;
  }

  // LERP optimization
  for (size_t i = 0; i < keyframes.size() - 1; i++) {
    AssimpRotKeyframe& rkf = keyframes[i];
    rkf.diffVal = keyframes[i + 1].val - rkf.val;
  }

  // SLERP optimization
  for (size_t i = 0; i < keyframes.size() - 1; i++) {
    AssimpRotKeyframe& rkf = keyframes[i];
    float dot = glm::dot(rkf.val, keyframes[i + 1].val);
    if (glm::abs(dot) > SLERP_THRESHOLD) {
      rkf.nextVal = dot >= 0 ? keyframes[i + 1].val : -keyframes[i + 1].val;
      rkf.theta = glm::acos(glm::dot(rkf.val, rkf.nextVal));
      rkf.invTheta = 1.0f / rkf.theta;
      rkf.useLerp = false;
    }
  }

  // Time optimization
  for (size_t i = 0; i < keyframes.size() - 1; i++) {
    AssimpRotKeyframe& rkf = keyframes[i];
    AssimpRotKeyframe& rkfNext = keyframes[i + 1];
    rkf.invT = 1.0 / (rkfNext.time - rkf.time);
  }
}

glm::vec4 AssimpRotChannel::eval(float t, const A_ANIM_EXTRAP& extrapIn,
                                 const A_ANIM_EXTRAP& extrapOut) const {
  if (keyframes.size() == 1) {
    return keyframes[0].val;
  }

  if (t < timeStart) {
    switch (extrapIn) {
      case A_ANIM_EXTRAP::CONSTANT:
        return keyframes[0].val;
      case A_ANIM_EXTRAP::LINEAR:  // not supported
      case A_ANIM_EXTRAP::CYCLE:
      case A_ANIM_EXTRAP::DEFAULT:
      default:
        t = std::fmod(t - timeStart, timeLen) + timeStart;
        if (t < timeStart) {
          t += timeLen;
        }
    }
  } else if (t > timeEnd) {
    switch (extrapOut) {
      case A_ANIM_EXTRAP::CONSTANT:
        return keyframes[keyframes.size() - 1].val;
      case A_ANIM_EXTRAP::LINEAR:  // not supported
      case A_ANIM_EXTRAP::CYCLE:
      case A_ANIM_EXTRAP::DEFAULT:
      default:
        t = std::fmod(t - timeStart, timeLen) + timeStart;
    }
  }

  size_t ind = 0;
  for (; ind < keyframes.size(); ind++) {
    if (t < keyframes[ind].time) {
      ind--;
      break;
    }
  }

  const AssimpRotKeyframe& rkf = keyframes[ind];
  float uTime = uTime = (t - rkf.time) * rkf.invT;
  switch (INTERP_MODE) {
    case A_ANIM_QUART_INTERP::NONE:
      return rkf.val;
    case A_ANIM_QUART_INTERP::LERP:
      return rkf.val + uTime * rkf.diffVal;
    case A_ANIM_QUART_INTERP::SLERP:
      if (rkf.useLerp) {
        return rkf.val + uTime * rkf.diffVal;
      }
      return rkf.invTheta * (glm::sin((1.0f - uTime) * rkf.theta) * rkf.val +
                             glm::sin(uTime * rkf.theta) * rkf.nextVal);
  }
}

// TODO(eddie): return valid ok val
AssimpAnimNode::AssimpAnimNode(const aiNodeAnim* const aiNodeAnim, bool& ok) {
  name = aiNodeAnim->mNodeName.C_Str();

  for (int k = 0; k < aiNodeAnim->mNumPositionKeys; k++) {
    float t = aiNodeAnim->mPositionKeys[k].mTime;
    glm::vec3 pos = aiVecToVec3(aiNodeAnim->mPositionKeys[k].mValue);
    posX.keyframes.push_back(AssimpKeyframe{t, pos.x});
    posY.keyframes.push_back(AssimpKeyframe{t, pos.y});
    posZ.keyframes.push_back(AssimpKeyframe{t, pos.z});
  }

  for (int k = 0; k < aiNodeAnim->mNumRotationKeys; k++) {
    rotQ.keyframes.push_back(AssimpRotKeyframe{
        static_cast<float>(aiNodeAnim->mRotationKeys[k].mTime),
        aiQuaternionToVec4(aiNodeAnim->mRotationKeys[k].mValue)});
  }

  for (int k = 0; k < aiNodeAnim->mNumScalingKeys; k++) {
    float t = aiNodeAnim->mScalingKeys[k].mTime;
    glm::vec3 sca = aiVecToVec3(aiNodeAnim->mScalingKeys[k].mValue);
    scaX.keyframes.push_back(AssimpKeyframe{t, sca.x});
    scaY.keyframes.push_back(AssimpKeyframe{t, sca.y});
    scaZ.keyframes.push_back(AssimpKeyframe{t, sca.z});
  }

  extrapIn = extrapModeOf(aiNodeAnim->mPreState);
  extrapOut = extrapModeOf(aiNodeAnim->mPostState);

  posX.prep();
  posY.prep();
  posZ.prep();
  scaX.prep();
  scaY.prep();
  scaZ.prep();
  rotQ.prep();

  pos = glm::vec3(posX.eval(0, extrapIn, extrapOut),
                  posY.eval(0, extrapIn, extrapOut),
                  posZ.eval(0, extrapIn, extrapOut));
  sca = glm::vec3(scaX.eval(0, extrapIn, extrapOut),
                  scaY.eval(0, extrapIn, extrapOut),
                  scaZ.eval(0, extrapIn, extrapOut));
  rot = rotQ.eval(0, extrapIn, extrapOut);
}

void AssimpAnimNode::update(double currentTick) {
  pos = glm::vec3(posX.eval(currentTick, extrapIn, extrapOut),
                  posY.eval(currentTick, extrapIn, extrapOut),
                  posZ.eval(currentTick, extrapIn, extrapOut));
  sca = glm::vec3(scaX.eval(currentTick, extrapIn, extrapOut),
                  scaY.eval(currentTick, extrapIn, extrapOut),
                  scaZ.eval(currentTick, extrapIn, extrapOut));
  rot = rotQ.eval(currentTick, extrapIn, extrapOut);
}

AssimpAnimationClip::AssimpAnimationClip() : duration(0), tps(0) {}

void AssimpAnimationClip::update(double currentTimeInMs,
                                 std::map<std::string, BlendPose>& poseMap,
                                 bool isBase) {
  if (tps == 0) {
    return;
  }

  double currentTick = currentTimeInMs * tps;
  for (int i = 0; i < nodes.size(); i++) {
    nodes[i].update(currentTick);
    if (isBase) {
      BlendPose bp;
      bp.pos1 = nodes[i].pos;
      bp.rot1 = nodes[i].rot;
      bp.sca1 = nodes[i].sca;
      poseMap[nodes[i].name] = bp;
    } else {
      BlendPose& bp = poseMap[nodes[i].name];
      bp.pos2 = nodes[i].pos;
      bp.rot2 = nodes[i].rot;
      bp.sca2 = nodes[i].sca;
    }
  }
}

void AssimpAnimationClip::update(double currentTimeInMs,
                                 std::map<std::string, AssimpNode*> nodeMap) {
  if (tps == 0) {
    return;
  }

  double currentTick = currentTimeInMs * tps;
  for (int i = 0; i < nodes.size(); i++) {
    nodes[i].update(currentTick);
    nodeMap[nodes[i].name]->animationTransform =
        getMatrixFromDOFs(nodes[i].pos, nodes[i].rot, nodes[i].sca);
  }
}

const std::map<AssimpAnimation::PLAYER_AC, std::string>
    AssimpAnimation::AC_TO_NAME = {
        {AssimpAnimation::PLAYER_AC::IDLE, "Armature|idle0"},
        {AssimpAnimation::PLAYER_AC::WALK, "Armature|walk"},
        {AssimpAnimation::PLAYER_AC::JUMP, "Armature|jump"},
        {AssimpAnimation::PLAYER_AC::TAG, "Armature|tag"},
        {AssimpAnimation::PLAYER_AC::TRIP, "Armature|trip"},
        {AssimpAnimation::PLAYER_AC::FALL, "Armature|fall"},
        {AssimpAnimation::PLAYER_AC::LOBBY1, "Armature|idle1"},
        {AssimpAnimation::PLAYER_AC::LOBBY2, "Armature|idle2"},
        {AssimpAnimation::PLAYER_AC::LOBBY3, "Armature|idle3"},
        {AssimpAnimation::PLAYER_AC::LOBBY4, "Armature|idle4"},
        {AssimpAnimation::PLAYER_AC::PLACE1_1, "Armature|place1.0"},
        {AssimpAnimation::PLAYER_AC::PLACE1_2, "Armature|place1.1"},
        {AssimpAnimation::PLAYER_AC::PLACE2_1, "Armature|place2.0"},
        {AssimpAnimation::PLAYER_AC::PLACE2_2, "Armature|place2.1"},
        {AssimpAnimation::PLAYER_AC::PLACE3_1, "Armature|place3.0"},
        {AssimpAnimation::PLAYER_AC::PLACE3_2, "Armature|place3.1"},
        {AssimpAnimation::PLAYER_AC::PLACE4_1, "Armature|place4.0"},
        {AssimpAnimation::PLAYER_AC::PLACE4_2, "Armature|place4.1"}};
const std::map<std::string, AssimpAnimation::PLAYER_AC>
    AssimpAnimation::NAME_TO_AC = {
        {"Armature|idle0", AssimpAnimation::PLAYER_AC::IDLE},
        {"Armature|walk", AssimpAnimation::PLAYER_AC::WALK},
        {"Armature|jump", AssimpAnimation::PLAYER_AC::JUMP},
        {"Armature|tag", AssimpAnimation::PLAYER_AC::TAG},
        {"Armature|trip", AssimpAnimation::PLAYER_AC::TRIP},
        {"Armature|fall", AssimpAnimation::PLAYER_AC::FALL},
        {"Armature|idle1", AssimpAnimation::PLAYER_AC::LOBBY1},
        {"Armature|idle2", AssimpAnimation::PLAYER_AC::LOBBY2},
        {"Armature|idle3", AssimpAnimation::PLAYER_AC::LOBBY3},
        {"Armature|idle4", AssimpAnimation::PLAYER_AC::LOBBY4},
        {"Armature|place1.0", AssimpAnimation::PLAYER_AC::PLACE1_1},
        {"Armature|place1.1", AssimpAnimation::PLAYER_AC::PLACE1_2},
        {"Armature|place2.0", AssimpAnimation::PLAYER_AC::PLACE2_1},
        {"Armature|place2.1", AssimpAnimation::PLAYER_AC::PLACE2_2},
        {"Armature|place3.0", AssimpAnimation::PLAYER_AC::PLACE3_1},
        {"Armature|place3.1", AssimpAnimation::PLAYER_AC::PLACE3_2},
        {"Armature|place4.0", AssimpAnimation::PLAYER_AC::PLACE4_1},
        {"Armature|place4.1", AssimpAnimation::PLAYER_AC::PLACE4_2}};
const char AssimpAnimation::EMOTE_CYC_SUFFIX[] = ".cycle";
const float AssimpAnimation::MS_DISSOLVE = 2.0f;
const float AssimpAnimation::MS_JUMP = 4.0f;
const std::vector<std::string> AssimpAnimation::NODES_TAG(
    {"R.shoulder", "R.arm", "R.finger", "R.hand", "Body", "neck", "head",
     "eye"});

bool AssimpAnimation::init(const aiScene* const scene,
                           const std::map<std::string, AssimpNode*> nodeMap,
                           bool& isPlayer) {
  isPlayer = false;
  this->isPlayer = false;

  for (int i = 0; i < scene->mNumAnimations; i++) {
    aiAnimation* aiAnimation = scene->mAnimations[i];

    AssimpAnimationClip animation;
    animation.name = aiAnimation->mName.C_Str();
    animation.duration = aiAnimation->mDuration;
    animation.tps = aiAnimation->mTicksPerSecond;
    printf("Assimp: [LOG] loading animation clip: %s\n",
           aiAnimation->mName.C_Str());
    for (int j = 0; j < aiAnimation->mNumChannels; j++) {
      bool okNode = true;
      AssimpAnimNode animNode(aiAnimation->mChannels[j], okNode);
      if (!okNode) {
        printf("Assimp: [ERROR] loading animation clip %s failed, aborting\n",
               aiAnimation->mName.C_Str());
        return false;
      }
      animation.nodes.push_back(animNode);
    }

    if (animMap.find(aiAnimation->mName.C_Str()) != animMap.end()) {
      printf("Assimp: [ERROR] duplicate animation name '%s', aborting\n",
             aiAnimation->mName.C_Str());
      return false;
    }

    animMap[aiAnimation->mName.C_Str()] = animation;
  }

  isPlayer = true;
  this->isPlayer = true;
  for (auto& kv : NAME_TO_AC) {
    if (animMap.find(kv.first) == animMap.end()) {
      printf("Assimp: [LOG] cannot find animation for player action %s\n",
             kv.first.c_str());
      isPlayer = false;
      this->isPlayer = false;
    }
  }

  this->nodeMap = nodeMap;
  if (isPlayer) {
    baseAnim = PLAYER_AC::IDLE;
    currAnimName = AC_TO_NAME.at(PLAYER_AC::IDLE);

    // prepare tag
    const AssimpAnimationClip& tag = animMap.at(AC_TO_NAME.at(PLAYER_AC::TAG));
    for (size_t i = 0; i < tag.nodes.size(); i++) {
      const AssimpAnimNode& node = tag.nodes[i];
      for (size_t j = 0; j < NODES_TAG.size(); j++) {
        if (node.name.find(NODES_TAG[j]) != std::string::npos) {
          ind_tag.push_back(i);
          break;
        }
      }
    }
  }
}

void AssimpAnimation::update(float deltaTimeInMs) {
  currTimeInMs += deltaTimeInMs;

  // not a player, just render single animation
  if (!isPlayer) {
    animMap[currAnimName].update(currTimeInMs, nodeMap);
    return;
  }

  /** hmmm, it's a player! */
  // emote
  if (isEmote) {
    if (!isEmoteCyc && currTimeInMs * animMap[currAnimName].tps >=
                           animMap[currAnimName].duration) {
      currAnimName = currAnimName + EMOTE_CYC_SUFFIX;
      currTimeInMs = 0.0f;
      isEmoteCyc = true;
    }

    animMap[currAnimName].update(currTimeInMs, nodeMap);
    return;
  }

  bool doneDissolve = false;
  poseMap.clear();
  if (isFall || isFallRecovering) {
    timeFall += deltaTimeInMs;
  }

  if (isPlayThenDissolve) {
    AssimpAnimationClip& play = animMap.at(AC_TO_NAME.at(baseAnim));
    if ((timePlayThenDissolve + deltaTimeInMs) * play.tps > play.duration) {
      isPlayThenDissolve = false;
      isDissolve = true;
      isDissolveReversed = false;
      timeDissolve = 0.0f;
      timeDissolveMult = isALobbyEmote(dissolveAnim) ? 10.0f : MS_JUMP;
    } else {
      timePlayThenDissolve += deltaTimeInMs;
      play.update(timePlayThenDissolve, poseMap, true);
      doneDissolve = true;
    }
  }

  if (!doneDissolve && isDissolve) {
    timeDissolve += !isDissolveReversed ? deltaTimeInMs * timeDissolveMult
                                        : -deltaTimeInMs * timeDissolveMult;
    if (timeDissolve >= 1.0f) {
      if (isALobbyEmote(dissolveAnim)) {
        baseAnim = dissolveAnim;
        dissolveAnim = PLAYER_AC::IDLE;
        timePlayThenDissolve = 0.0f;
        isPlayThenDissolve = true;
        isDissolve = false;
        isDissolveReversed = false;
      } else {
        if (isFallRecovering) {
          isFallRecovering = false;
        }
        isDissolve = false;
        baseAnim = dissolveAnim;
        currAnimName = AC_TO_NAME.at(baseAnim);
      }
    } else if (timeDissolve <= 0.0f) {
      isDissolve = false;
    } else {
      // blend two output
      AssimpAnimationClip& clip0 = animMap.at(AC_TO_NAME.at(baseAnim));
      AssimpAnimationClip& clip1 = animMap.at(AC_TO_NAME.at(dissolveAnim));

      if (isAPlayThenDissolve(baseAnim) || isALobbyEmote(baseAnim)) {
        clip0.update(timePlayThenDissolve, poseMap, true);
      } else if (isFall) {
        clip0.update(currTimeInMs - deltaTimeInMs, poseMap, true);
      } else if (isFallRecovering) {
        clip0.update(timeFall, poseMap, true);
      } else {
        clip0.update(currTimeInMs, poseMap, true);
      }
      if (isALobbyEmote(dissolveAnim)) {
        clip1.update(0.0f, poseMap, false);
      } else if (isFall) {
        clip1.update(timeFall, poseMap, false);
      } else {
        clip1.update(currTimeInMs, poseMap, false);
      }
      for (auto& kv : poseMap) {
        BlendPose& bp = kv.second;
        bp.pos1 = timeDissolve * bp.pos1 + (1.0f - timeDissolve) * bp.pos2;
        bp.rot1 =
            quarternionInterpolateSpherical(bp.rot1, bp.rot2, timeDissolve);
        bp.sca1 = timeDissolve * bp.sca1 + (1.0f - timeDissolve) * bp.sca2;
      }
      doneDissolve = true;
    }
  }

  if (!doneDissolve) {
    animMap[currAnimName].update(currTimeInMs, poseMap, true);
    doneDissolve = true;
  }

  if (!isTag) {
    for (auto& kv : poseMap) {
      BlendPose& bp = kv.second;
      nodeMap[kv.first]->animationTransform =
          getMatrixFromDOFs(bp.pos1, bp.rot1, bp.sca1);
    }
  } else {
    AssimpAnimationClip& tag = animMap.at(AC_TO_NAME.at(PLAYER_AC::TAG));
    if (timeTag + deltaTimeInMs > tag.duration / tag.tps) {
      isTag = false;
      for (auto& kv : poseMap) {
        BlendPose& bp = kv.second;
        nodeMap[kv.first]->animationTransform =
            getMatrixFromDOFs(bp.pos1, bp.rot1, bp.sca1);
      }
    } else {
      timeTag += deltaTimeInMs;
      for (auto& kv : poseMap) {
        BlendPose& bp = kv.second;
        nodeMap[kv.first]->animationTransform =
            getMatrixFromDOFs(bp.pos1, bp.rot1, bp.sca1);
      }
      for (size_t i = 0; i < ind_tag.size(); i++) {
        AssimpAnimNode& animNode = tag.nodes[ind_tag[i]];
        animNode.update(timeTag * tag.tps);
        nodeMap[animNode.name]->animationTransform =
            getMatrixFromDOFs(animNode.pos, animNode.rot, animNode.sca);
      }
    }
  }
}

void AssimpAnimation::useAnimation(std::string animName) {
  if (animMap.find(animName) == animMap.end()) {
    printf("Assimp: [WARNING] setting animation ignored, %s not found\n",
           animName.c_str());
    return;
  }

  if (!isPlayer || NAME_TO_AC.find(animName) == NAME_TO_AC.end()) {
    if (currAnimName.compare(animName) != 0) {
      reset();
      currTimeInMs = 0.0f;
      currAnimName = animName;
    }
    return;
  }

  PLAYER_AC ac = NAME_TO_AC.at(animName);
  if (isALobbyEmote(ac)) {
    setLobby(ac);
    return;
  }
  if (isAEmote(ac)) {
    setEmote(ac);
    return;
  }
  blendAnimation(ac);
}

void AssimpAnimation::blendAnimation(const PLAYER_AC& ac) {
  if (!isPlayer) {
    printf("Assimp: [WARNING] ignored blending on non-player model\n");
    return;
  }

  // TODO(Eddie): test this!
  if (isEmote || isALobbyEmote(baseAnim) || isALobbyEmote(dissolveAnim)) {
    reset();
  }

  if (isAPlayThenDissolve(ac)) {
    if (baseAnim == PLAYER_AC::TRIP || isFall) {
      return;
    }

    // play full jump animation first, then dissolve out jump's last frame
    if (!isAPlayThenDissolve(baseAnim)) {
      dissolveAnim = baseAnim;
    }
    baseAnim = ac;
    timePlayThenDissolve = ac == PLAYER_AC::JUMP ? 0.1f : 0.0f;
    isPlayThenDissolve = true;
    isDissolve = false;
    isDissolveReversed = false;
    return;
  }

  if (ac == PLAYER_AC::IDLE || ac == PLAYER_AC::WALK) {
    if (isPlayThenDissolve || isAPlayThenDissolve(baseAnim) ||
        isFallRecovering) {
      // No blending if player switch between idle & walk when jump is
      // dissolving out :(
      dissolveAnim = ac;
      return;
    }
    if (isFall) {
      currTimeInMs = 0.0f;
      isFall = false;
      isFallRecovering = true;
      isDissolve = true;
      isDissolveReversed = false;
      timeDissolve = 0.0f;
      timeDissolveMult = 10.0f;
      dissolveAnim = ac;
      return;
    }

    if (ac == baseAnim) {
      if (isDissolve && !isDissolveReversed) {
        // backward dissolve
        isDissolveReversed = true;
      }
      return;
    }

    if (!isDissolve) {
      // start dissolve blending to the other animation
      isDissolve = true;
      isDissolveReversed = false;
      timeDissolve = 0.0f;
      timeDissolveMult = MS_DISSOLVE;
      dissolveAnim =
          baseAnim == PLAYER_AC::IDLE ? PLAYER_AC::WALK : PLAYER_AC::IDLE;
    } else {
      // forward dissolve
      isDissolveReversed = false;
    }
  }

  // idea:
  //   dissolve from current idle/walk pose to fall
  //   keep playing fall
  //   if any other animation happens
  if (ac == PLAYER_AC::FALL && !isFall) {
    isDissolve = true;
    isDissolveReversed = false;
    timeDissolve = 0.0f;
    timeDissolveMult = 10.0f;
    dissolveAnim = ac;
    isFall = true;
    isFallRecovering = false;
    timeFall = 0.0f;
  }

  if (ac == PLAYER_AC::TAG && !isTag) {
    isTag = true;
    isTagReversed = false;
    timeTag = 0.0f;
  }
}

// idea:
//   dissolve from current idle pose to lobby emote (0.1 sec)
//   use play then dissolve with lobby emote
//   ends with idle emote
void AssimpAnimation::setLobby(const PLAYER_AC& ac) {
  if (baseAnim != PLAYER_AC::IDLE) {
    reset();
  }

  isDissolve = true;
  isDissolveReversed = false;
  timeDissolve = 0.0f;
  timeDissolveMult = 10.0f;
  dissolveAnim = ac;
  AssimpRotChannel::INTERP_MODE = A_ANIM_QUART_INTERP::NONE;
}

void AssimpAnimation::reset() {
  currTimeInMs = 0.0f;
  baseAnim = PLAYER_AC::IDLE;
  dissolveAnim = PLAYER_AC::WALK;
  currAnimName = AC_TO_NAME.at(PLAYER_AC::IDLE);
  isDissolve = false;
  isDissolveReversed = false;
  timeDissolve = 0.0f;
  timeDissolveMult = 1.0f;
  isTag = false;
  isTagReversed = false;
  timeTag = 0.0f;
  isEmote = false;
  isEmoteCyc = false;
  isFall = false;
  isFallRecovering = false;
  timeFall = 0.0f;
  AssimpRotChannel::INTERP_MODE = A_ANIM_QUART_INTERP::LERP;
}

void AssimpAnimation::setEmote(const PLAYER_AC& ac) {
  reset();
  currTimeInMs = 0.0f;
  baseAnim = ac;
  currAnimName = AC_TO_NAME.at(baseAnim);
  isEmote = true;
  isEmoteCyc = false;
  AssimpRotChannel::INTERP_MODE = A_ANIM_QUART_INTERP::NONE;
}
