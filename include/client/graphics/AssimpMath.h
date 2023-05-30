/** Utility functions for Assimp related operations */
#pragma once

#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "client/graphics/AssimpAnimation.h"

inline glm::vec3 aiVecToVec3(const aiVector3D& vec) {
  return glm::vec3(vec.x, vec.y, vec.z);
}

inline glm::vec4 aiQuaternionToVec4(const aiQuaternion& q) {
  return glm::vec4(q.x, q.y, q.z, q.w);
}

// References:
// http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
// https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
const float SLERP_THRESHOLD = 0.9995;
/** tInvLerp is in (0,1)
 * LinearLerp is recommended if q0 and q1 have small time difference.
 */
inline glm::vec4 quarternionInterpolateSpherical(const glm::vec4& q0,
                                                 const glm::vec4& q1,
                                                 float tInvLerp,
                                                 bool useLinear = true) {
  if (useLinear) {
    return glm::dot(q0, q1) >= 0 ? glm::normalize(q0 + tInvLerp * (q1 - q0))
                                 : glm::normalize(q0 + tInvLerp * (-q1 - q0));
  }

  float dot = glm::dot(q0, q1);
  if (glm::abs(dot) > SLERP_THRESHOLD) {
    return quarternionInterpolateSpherical(q0, q1, tInvLerp, true);
  }
  float theta_0 = glm::dot(q0, q1) < 0 ? glm::acos(glm::dot(-q0, q1))
                                       : glm::acos(glm::dot(q0, q1));
  float theta = theta_0 * tInvLerp;
  return q0 * glm::cos(theta) + (q1 - q0 * dot) * glm::sin(theta);
}

inline glm::mat4 quarternionToRotationMat4x4(const glm::vec4& q) {
  const float &q0 = q.x, &q1 = q.y, &q2 = q.z, &q3 = q.w;
  // TODO: I don't understand why I need a transpose here
  return glm::transpose(glm::mat4(
      glm::mat3(1 - 2 * q1 * q1 - 2 * q2 * q2, 2 * q0 * q1 - 2 * q2 * q3,
                2 * q0 * q2 + 2 * q1 * q3, 2 * q0 * q1 + 2 * q2 * q3,
                1 - 2 * q0 * q0 - 2 * q2 * q2, 2 * q1 * q2 - 2 * q0 * q3,
                2 * q0 * q2 - 2 * q1 * q3, 2 * q1 * q2 + 2 * q0 * q3,
                1 - 2 * q0 * q0 - 2 * q1 * q1)));
}

inline glm::mat4 aiMatToMat4x4(const aiMatrix4x4& mat) {
  return glm::mat4(mat.a1, mat.b1, mat.c1, mat.d1, mat.a2, mat.b2, mat.c2,
                   mat.d2, mat.a3, mat.b3, mat.c3, mat.d3, mat.a4, mat.b4,
                   mat.c4, mat.d4);
}

inline glm::mat3 aiMatToMat3x3(const aiMatrix3x3& mat) {
  return glm::mat3(mat.a1, mat.b1, mat.c1, mat.a2, mat.b2, mat.c2, mat.a3,
                   mat.b3, mat.c3);
}

inline glm::mat3 aiQuaternionToMat3x3(const aiQuaternion& q) {
  return aiMatToMat3x3(q.GetMatrix());
}

inline glm::mat4 aiQuaternionToMat4x4(const aiQuaternion& q) {
  return glm::mat4(aiQuaternionToMat3x3(q));
}

inline glm::mat4 getMatrixFromDOFs(const glm::vec3& pos, const glm::vec4& rot,
                                   const glm::vec3& sca) {
  glm::mat4 result(1.0f);
  result = glm::translate(result, pos);
  result = result * quarternionToRotationMat4x4(rot);
  result = glm::scale(result, sca);
  return result;
}

inline A_ANIM_EXTRAP extrapModeOf(const aiAnimBehaviour& b) {
  return b & aiAnimBehaviour::aiAnimBehaviour_CONSTANT ? A_ANIM_EXTRAP::CONSTANT
         : b & aiAnimBehaviour::aiAnimBehaviour_LINEAR ? A_ANIM_EXTRAP::LINEAR
         : b & aiAnimBehaviour::aiAnimBehaviour_REPEAT ? A_ANIM_EXTRAP::CYCLE
                                                       : A_ANIM_EXTRAP::DEFAULT;
}
