#include "AssimpAnimation.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "AssimpMath.h"

void AssimpChannel::prep() {
  const glm::mat4 INV_MAT(2.0, -3.0, 0.0, 1.0, -2.0, 3.0, 0.0, 0.0, 1.0, -2.0,
                          1.0, 0.0, 1.0, -1.0, 0.0, 0.0);
  glm::vec4 ABCD(0.0f);
  glm::vec4 goal(0.0f);
  if (keyframes.empty()) {
    return;
  }

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
      case ASSIMP_TANGENT_MODE::SMOOTH:
        kf.tangent = (keyframes[i + 1].val - keyframes[i - 1].val) /
                     (keyframes[i + 1].time - keyframes[i - 1].time);
      case ASSIMP_TANGENT_MODE::LINEAR:
        kf.tangent =
            (keyframes[i + 1].val - kf.val) / (keyframes[i + 1].time - kf.time);
        break;
      case ASSIMP_TANGENT_MODE::FLAT:
      default:
        return;
    }
  }
  if (TANGENT_MODE != ASSIMP_TANGENT_MODE::FLAT) {
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

float AssimpChannel::eval(float t) const {
  if (keyframes.size() == 1) {
    return keyframes[0].val;
  }

  if (t < timeStart) {
    switch (extrapIn) {
      case ASSIMP_EXTRAP_MODE::CONSTANT:
        return eval(t + timeEnd - timeStart);  // TODO
        return keyframes[0].val;
      case ASSIMP_EXTRAP_MODE::LINEAR:
        return keyframes[0].val + keyframes[0].tangent * (timeStart - t);
      case ASSIMP_EXTRAP_MODE::CYCLE:
      case ASSIMP_EXTRAP_MODE::DEFAULT:  // TODO
      default:
        return eval(t + timeEnd - timeStart);
    }
  } else if (t > timeEnd) {
    switch (extrapOut) {
      case ASSIMP_EXTRAP_MODE::CONSTANT:
        return eval(t - timeEnd + timeStart);  // TODO
        return keyframes[keyframes.size() - 1].val;
      case ASSIMP_EXTRAP_MODE::LINEAR:
        return keyframes[keyframes.size() - 1].val +
               keyframes[keyframes.size() - 1].tangent *
                   (t - keyframes[keyframes.size() - 1].time);
      case ASSIMP_EXTRAP_MODE::CYCLE:
      case ASSIMP_EXTRAP_MODE::DEFAULT:  // TODO
      default:
        return eval(t - timeEnd + timeStart);
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

// glm::mat4 AssimpChannel::getMatrixInterpolate(int lastEarlyTickInd,
//                                               double tick) {
//   // linear interpolation
//   double t0 = positions[lastEarlyTickInd].first;
//   double t1 = positions[lastEarlyTickInd + 1].first;
//   glm::vec3 interpPos = positions[lastEarlyTickInd].second +
//                         (positions[lastEarlyTickInd + 1].second -
//                          positions[lastEarlyTickInd].second) *
//                             static_cast<float>((tick - t0) / (t1 - t0));
//
//   // spherical interpolation
//   glm::vec4 interpRot = quarternionInterpolateSpherical(
//       rotations[lastEarlyTickInd].second,
//       rotations[lastEarlyTickInd + 1].second,
//       static_cast<float>((tick - t0) / (t1 - t0)));
//
//   // linear interpolation
//   glm::vec3 interpSca = scalings[lastEarlyTickInd].second +
//                         (scalings[lastEarlyTickInd + 1].second -
//                          scalings[lastEarlyTickInd].second) *
//                             static_cast<float>((tick - t0) / (t1 - t0));
//   return getMatrix(interpPos, interpRot, interpSca);
// }

AssimpAnimNode::AssimpAnimNode(const aiNodeAnim* const aiNodeAnim) {
  name = aiNodeAnim->mNodeName.C_Str();

  for (int k = 0; k < aiNodeAnim->mNumPositionKeys; k++) {
    float t = aiNodeAnim->mPositionKeys[k].mTime;
    glm::vec3 pos = aiVecToVec3(aiNodeAnim->mPositionKeys[k].mValue);
    posX.keyframes.push_back(AssimpKeyframe{t, pos.x});
    posY.keyframes.push_back(AssimpKeyframe{t, pos.y});
    posZ.keyframes.push_back(AssimpKeyframe{t, pos.z});
  }

  for (int k = 0; k < aiNodeAnim->mNumRotationKeys; k++) {
    float t = aiNodeAnim->mScalingKeys[k].mTime;
    glm::vec4 rotQuaternion =
        aiQuaternionToVec4(aiNodeAnim->mRotationKeys[k].mValue);
    rotT.push_back(t);
    rotQ.push_back(rotQuaternion);
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
  posX.extrapIn = extrapIn;
  posY.extrapIn = extrapIn;
  posZ.extrapIn = extrapIn;
  scaX.extrapIn = extrapIn;
  scaY.extrapIn = extrapIn;
  scaZ.extrapIn = extrapIn;
  posX.extrapOut = extrapOut;
  posY.extrapOut = extrapOut;
  posZ.extrapOut = extrapOut;
  scaX.extrapOut = extrapOut;
  scaY.extrapOut = extrapOut;
  scaZ.extrapOut = extrapOut;

  posX.prep();
  posY.prep();
  posZ.prep();
  scaX.prep();
  scaY.prep();
  scaZ.prep();

  pos = glm::vec3(posX.eval(0), posY.eval(0), posZ.eval(0));
  sca = glm::vec3(scaX.eval(0), scaY.eval(0), scaZ.eval(0));
}

void AssimpAnimNode::update(double currentTick) {
  /*pos = glm::vec3(posX.eval(currentTick), posY.eval(currentTick),
                  posZ.eval(currentTick));
  sca = glm::vec3(scaX.eval(currentTick), scaY.eval(currentTick),
                  scaZ.eval(currentTick));*/

  /*size_t t = 0;
  for (; t < rotQ.size(); t++) {
    if (currentTick < rotT[t]) {
      t--;
      break;
    }
  }

  if (t >= rotQ.size() - 1) {
    rot = rotQ[rotQ.size() - 1];
  } else {
    rot = quarternionInterpolateSpherical(
        rotQ[t], rotQ[t + 1],
        static_cast<float>((currentTick - rotT[t]) / (rotT[t + 1] - rotT[t])));
  }*/

  rot = rotQ[0];
}

AssimpAnimation::AssimpAnimation()
    : duration(0), tps(0), currentTimeInMs(0) {}

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
