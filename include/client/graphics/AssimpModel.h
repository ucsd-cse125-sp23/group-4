#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <map>
#include <string>
#include <vector>

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
  AssimpModel();
  // TODO
  // ~AssimpModel();

  /** Load a model file (displaying default pose).
   * Returns whether the model file is loaded properly.
   * If the node has already been initialized, returns false.
   */
  bool loadAssimp(const char* path);

  /** Set model to use default pose */
  void useMesh();
  /** Set model to use specified animation (-1 = use default pose)
   * returns false if animationInd is invalid
   */
  bool useAnimation(int animationInd);

  void draw(const glm::mat4& viewProjMtx, GLuint shader);
  void imGui();

  // PlayerModel
  void setAnimation(std::string name);
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
  AssimpNode* rootNode;
  unsigned int numNode;
  std::map<std::string, AssimpNode*> nodeMap;
  std::vector<bool> meshVisibilities;
  std::vector<AssimpMesh*> meshes;
  bool isAnimated, isPaused;
  int currentAnimation;
  std::vector<AssimpAnimation> animations;
  glm::mat4 betterView = glm::mat4(1.0);
  char** animModes;
  bool drawNode = false;

  struct ControlInfo {
    bool useControl = false;
    glm::vec3 pose = glm::vec3(1);
    glm::vec3 scale = glm::vec3(1);
    glm::vec3 offset = glm::vec3(1);
  };

  std::map<std::string, ControlInfo> nodeControlMap;

  void prepareNodes(const aiScene* const scene);
  bool populateNode(const aiScene* const scene, const aiNode* const aiNode,
                    const glm::mat4 accTransform,
                    std::vector<std::pair<aiMesh*, AssimpMesh*>>& meshQueue);
  bool populateMesh(const aiScene* const scene, const aiMesh* const aiMesh,
                    AssimpMesh* const mesh);
  bool bindJoint(AssimpMesh* const mesh, const aiBone* const aiBone);

  /** Add animations to this model
   * scene: the scene created by the importer
   */
  void loadAssimpHelperAnim(const aiScene* scene);
  /** Prepare ImGui information for this model */
  void loadAssimpHelperImgui();
  void imGuiJointMenu();
};
