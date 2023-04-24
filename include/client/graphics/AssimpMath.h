#include <glm/glm.hpp>
#include <assimp/scene.h>

inline glm::vec3 aiVecToVec3(const aiVector3D& vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}

inline glm::mat4 aiMatToMat4x4(const aiMatrix4x4& mat) {
    return glm::mat4(
        mat.a1, mat.b1, mat.c1, mat.d1,
        mat.a2, mat.b2, mat.c2, mat.d2,
        mat.a3, mat.b3, mat.c3, mat.d3,
        mat.a4, mat.b4, mat.c4, mat.d4);
}

inline glm::mat3 aiMatToMat3x3(const aiMatrix3x3& mat) {
    return glm::mat3(
        mat.a1, mat.b1, mat.c1,
        mat.a2, mat.b2, mat.c2,
        mat.a3, mat.b3, mat.c3);
}

inline glm::mat3 aiQuaternionToMat3x3(const aiQuaternion& q) {
    return aiMatToMat3x3(q.GetMatrix());
}

inline glm::mat4 aiQuaternionToMat4x4(const aiQuaternion& q) {
    return glm::mat4(aiQuaternionToMat3x3(q));
}
