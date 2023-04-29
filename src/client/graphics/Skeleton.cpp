#include "Skeleton.h"

// get the camera forward direction
glm::vec3 GetForward(Camera* camera) {
  float azimuth = camera->GetAzimuth();
  float x = glm::sin(glm::radians(azimuth));  // x component of forward axis
  float z = glm::cos(glm::radians(azimuth));  // z component of forward axis

  return glm::vec3(x, 0.0f, z);
}

// get the camera side direction
glm::vec3 GetSide(Camera* camera) {
  float azimuth = camera->GetAzimuth();
  float x = glm::cos(glm::radians(azimuth));
  float z = -1 * glm::sin(glm::radians(azimuth));

  return glm::vec3(x, 0.0f, z);
}

static Joint* BuildJointArray(Joint* root, std::vector<Joint*>* arr) {
  // Traverses tree, depth-first

  std::vector<Joint*> dfs;
  dfs.push_back(root);

  while (dfs.size() > 0) {
    Joint* c = dfs.back();
    dfs.pop_back();

    arr->push_back(c);

    int i = 0;
    while (c->GetChildReversed(i)) {
      dfs.push_back(c->GetChildReversed(i++));  // Add children to stack
    }
  }

  return nullptr;
}

bool Skeleton::Load(const char* file) {
  if (file == "") return false;

  Tokenizer token;
  bool success = token.Open(file);

  if (!success) return false;

  token.FindToken("balljoint");

  if (root) delete root;

  // Parse tree
  char tempname[256];
  token.GetToken(tempname);  // name
  root = new Joint(&tempname[0]);
  root->Load(token);

  // Finish
  token.Close();

  // Initializes joints array
  BuildJointArray(root, &joints);

  return true;
}

void Skeleton::Update(float deltaTime, std::map<std::string, AnimationPlayer*> anims) {
  if (root) {
    root->Update(glm::mat4(1.0));
    //Animate(deltaTime, anims);
  }
}

void Skeleton::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
  if (root) root->Draw(viewProjMtx, shader);
}

void Skeleton::Show() {
  if (root) root->ShowAll();
}

void Skeleton::SetPose(const Pose pose) {
  for (int i = 0; i < pose.size(); i += 3) {
    int j = (i / 3) - 1;  // account for root index!

    glm::vec3 p = glm::vec3(pose[i], pose[i + 1], pose[i + 2]);

    SetJointDOFs(j, p);
  }
}

void Skeleton::SetJointDOFs(int joint, glm::vec3 dof) {
  Joint* j = GetJoint(joint);
  // -1 is root
  if (joint < 0) {
    j = root;

    j->SetPosition(dof);

    return;
  }

  if (!j) {
    return;
  }

  j->SetDOF(0, dof.x);
  j->SetDOF(1, dof.y);
  j->SetDOF(2, dof.z);
}

// Getters
glm::mat4 Skeleton::GetWorldMatrix(int joint) {
  Joint* j = GetJoint(joint);
  if (!j) {
    return glm::mat4(-1.0);
  }

  return j->GetWorldMatrix();
}

Joint* Skeleton::GetJoint(int j) {
  if (j < 0 || j >= joints.size()) return nullptr;

  return joints[j];
}

// Finds a joint at index j
// slow and redundant
Joint* Skeleton::FindJointInTree(int j) {
  // Traverses tree, depth-first

  std::vector<Joint*> dfs;
  dfs.push_back(root);

  while (dfs.size() > 0) {
    Joint* c = dfs.back();
    dfs.pop_back();

    if (j-- == 0) return c;  // Return current joint

    int i = 0;
    while (c->GetChildReversed(i)) {
      dfs.push_back(c->GetChildReversed(i++));  // Add children to stack
    }
  }

  return nullptr;
}

