#include "AssimpNode.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtx/string_cast.hpp>

#include "client/graphics/AssimpMath.h"
#include "client/graphics/imported/parallel.h"
#include "client/graphics/shader.h"

AssimpNode::AssimpNode(unsigned int id) : id(id) {}

void AssimpNode::update(const glm::mat4& accWorldMtx) {
  matWorldTransform = accWorldMtx * this->animationTransform;

  for (unsigned int i = 0; i < joints.size(); i++) {
    NodeJoint& nj = joints[i];
    AssimpMesh* const mesh = nj.mesh;
    glm::mat4 worldMtx = matWorldTransform * nj.invBindMat;

    if (!mesh->isUpdated) {
      mesh->isUpdated = true;
    }

    /*constexpr int segmentLength = 16;
    parallel_for([&](const int& i) {
          VertexWeight& vw = nj.vertexWeights[i];
          mesh->animPos[vw.vertexInd] +=
              vw.weight * worldMtx * mesh->pos[vw.vertexInd];
          mesh->animNorm[vw.vertexInd] +=
              vw.weight * worldMtx * mesh->norm[vw.vertexInd];
        }, nj.vertexWeights.size(), 1024);*/
    for (unsigned int n = 0; n < nj.vertexWeights.size(); n++) {
      VertexWeight& vw = nj.vertexWeights[n];
      mesh->animPos[vw.vertexInd] +=
          vw.weight * worldMtx * mesh->pos[vw.vertexInd];
      mesh->animNorm[vw.vertexInd] +=
          vw.weight * worldMtx * mesh->norm[vw.vertexInd];
    }
  }

  for (int i = 0; i < children.size(); i++) {
    children[i]->update(matWorldTransform);
  }
}
