#include "AssimpAnimation.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
  timeEnd = keyframes[1].time;
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
        return eval(t + timeEnd - timeStart, extrapIn,
                    extrapOut);  // TODO(eddie): remove this line
        return keyframes[0].val;
      case A_ANIM_EXTRAP::LINEAR:
        return keyframes[0].val + keyframes[0].tangent * (timeStart - t);
      case A_ANIM_EXTRAP::CYCLE:
      case A_ANIM_EXTRAP::DEFAULT:  // TODO(eddie): default extrap impl
      default:
        return eval(t + timeEnd - timeStart, extrapIn, extrapOut);
    }
  } else if (t > timeEnd) {
    switch (extrapOut) {
      case A_ANIM_EXTRAP::CONSTANT:
        return eval(t - timeEnd + timeStart, extrapIn,
                    extrapOut);  // TODO(eddie): remove this line
        return keyframes[keyframes.size() - 1].val;
      case A_ANIM_EXTRAP::LINEAR:
        return keyframes[keyframes.size() - 1].val +
               keyframes[keyframes.size() - 1].tangent *
                   (t - keyframes[keyframes.size() - 1].time);
      case A_ANIM_EXTRAP::CYCLE:
      case A_ANIM_EXTRAP::DEFAULT:  // TODO(eddie): default extrap impl
      default:
        return eval(t - timeEnd + timeStart, extrapIn, extrapOut);
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

void AssimpRotChannel::prep() {
  if (keyframes.empty()) {
    return;
  }

  timeStart = keyframes[0].time;
  timeEnd = keyframes[keyframes.size() - 1].time;
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
    if (glm::abs(dot) < SLERP_THRESHOLD) {
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
        return eval(t + timeEnd - timeStart, extrapIn,
                    extrapOut);  // TODO(eddie): remove this line
        return keyframes[0].val;
      case A_ANIM_EXTRAP::CYCLE:
      case A_ANIM_EXTRAP::LINEAR:   // not supported
      case A_ANIM_EXTRAP::DEFAULT:  // TODO(eddie): default extrap impl
      default:
        return eval(t + timeEnd - timeStart, extrapIn, extrapOut);
    }
  } else if (t > timeEnd) {
    switch (extrapOut) {
      case A_ANIM_EXTRAP::CONSTANT:
        return eval(t - timeEnd + timeStart, extrapIn,
                    extrapOut);  // TODO(eddie): remove this line
        return keyframes[keyframes.size() - 1].val;
      case A_ANIM_EXTRAP::LINEAR:  // not supported
      case A_ANIM_EXTRAP::CYCLE:
      case A_ANIM_EXTRAP::DEFAULT:  // TODO(eddie): default extrap impl
      default:
        return eval(t - timeEnd + timeStart, extrapIn, extrapOut);
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

AssimpAnimation::AssimpAnimation() : duration(0), tps(0), currentTimeInMs(0) {}

void AssimpAnimation::restart() { currentTimeInMs = 0; }

void AssimpAnimation::update(double deltaTimeInMs,
                             std::map<std::string, AssimpNode*> nodeMap) {
  if (tps == 0) {
    return;
  }

  currentTimeInMs += deltaTimeInMs;
  double currentTick = currentTimeInMs * tps;
  for (int i = 0; i < nodes.size(); i++) {
    nodes[i].update(currentTick);
    nodeMap[nodes[i].name]->animationTransform =
        getMatrixFromDOFs(nodes[i].pos, nodes[i].rot, nodes[i].sca);
  }
}
