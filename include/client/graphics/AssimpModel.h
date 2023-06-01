#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <map>
#include <string>
#include <vector>
#include <utility>

#include "client/graphics/AssimpAnimation.h"
#include "client/graphics/PlayerModel.h"

/** The AssimpModel class for loading and displaying an animated model file.
 *
 * IMPORTANT:
 * Due to defect in curent implementation,
 * every bone/joint must have a channel with at least two values;
 * Fixed timestamp keyframing is preferred.
 */
class AssimpModel : public PlayerModel, public SkinnedMesh {
 public:
  /** Empty constructor
   *    initialize model by calling loadAssimp()
   */
  AssimpModel();
  /** Construct a model using a reference model
   *    by calling loadAssimp() with stored path of reference model
   *    DO NOT call loadAssimp() again
   * IMPORTANT:
   *   Points to the same Material as the reference model
   *   Current animation is not inherited
   */
  AssimpModel(const AssimpModel& am);
  // TODO
  // ~AssimpModel();

  /** Load a model file (displaying default pose).
   * Returns whether the model file is loaded properly.
   * If the node has already been initialized, returns false.
   */
  bool loadAssimp(const char* path);

  // PlayerModel
  void setAnimation(std::string animName);
  void update(float deltaTimeInMs);
  void draw(const glm::mat4& viewProjMtx, const glm::mat4& viewMtx,
            const glm::mat4& transformMtx, const bool ignoreDepth = false);

  // SkinnedMesh
  void init(const char* filename) {
    if (!loadAssimp(filename)) {
      exit(EXIT_FAILURE);
    }
  }
  void draw();

 private:
  std::string name;
  std::string path;

  unsigned int numNode;
  AssimpNode* rootNode;
  std::map<std::string, AssimpNode*> nodeMap;
  std::vector<AssimpMesh*> meshes;

  AssimpAnimation animation;
  glm::mat4 betterView = glm::mat4(1.0);

  void prepareNodes(const aiScene* const scene);
  bool populateNode(const aiScene* const scene, const aiNode* const aiNode,
                    const glm::mat4 accTransform,
                    std::vector<std::pair<aiMesh*, AssimpMesh*>>& meshQueue);
  bool populateMesh(const aiScene* const scene, const aiMesh* const aiMesh,
                    AssimpMesh* const mesh);
  bool bindJoint(AssimpMesh* const mesh, const aiBone* const aiBone);
};
