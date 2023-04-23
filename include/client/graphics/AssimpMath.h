#include <glm/glm.hpp>
#include <assimp/scene.h>

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