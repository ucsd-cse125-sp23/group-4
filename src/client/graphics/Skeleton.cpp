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

void Skeleton::Update(GLFWwindow* window, Camera* camera, float deltaTime, std::map<std::string, AnimationPlayer*> anims, float step) {
  if (root) {
    root->Update(glm::mat4(1.0));
    Move(window, camera, deltaTime, anims, step);
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

void Skeleton::MoveForward(Camera* camera, float delta) {
  // orient skeleton to face forward
  root->SetDOF(1, glm::radians(-1.0 * camera->GetAzimuth()));

  glm::vec3 root_pos = root->GetPosition();

  // rotate axis (0, 0, 1) to get the current forward axis
  glm::vec3 forward_axis = GetForward(camera);
  float forwardX = forward_axis[0];
  float forwardZ = forward_axis[2];

  float dx;
  float dz;
  if (abs(int(forwardZ)) == 1) {
    dx = 0;
    dz = forwardZ * delta;
  } else if (abs(int(forwardX)) == 1) {
    dz = 0;
    dx = forwardX * delta;
  } else {
    float m = forwardZ / forwardX;
    dx = sqrt(-4 * (1 + pow(m, 2)) * (-1 * pow(delta, 2))) /
         (2 * (1 + pow(m, 2)));
    if (forwardX < 0) {
      dx *= -1;
    }
    dz = dx * m;
  }
  root_pos[0] += dx;
  root_pos[2] -= dz;
  root->SetPosition(root_pos);
}

void Skeleton::MoveBackward(Camera* camera, float delta) {
  root->SetDOF(1, glm::radians((-1.0 * camera->GetAzimuth()) + 180.0f));
  glm::vec3 root_pos = root->GetPosition();

  // rotate axis (0, 0, 1) to get the current forward axis
  glm::vec3 forward_axis = GetForward(camera);
  float forwardX = forward_axis[0];
  float forwardZ = forward_axis[2];

  float dx;
  float dz;
  if (abs(int(forwardZ)) == 1) {
    dx = 0;
    dz = forwardZ * delta;
  } else if (abs(int(forwardX)) == 1) {
    dz = 0;
    dx = forwardX * delta;
  } else {
    float m = forwardZ / forwardX;
    dx = sqrt(-4 * (1 + pow(m, 2)) * (-1 * pow(delta, 2))) /
         (2 * (1 + pow(m, 2)));
    if (forwardX < 0) {
      dx *= -1;
    }
    dz = dx * m;
  }
  root_pos[0] -= dx;
  root_pos[2] += dz;
  root->SetPosition(root_pos);
}

void Skeleton::MoveRight(Camera* camera, float delta) {
  root->SetDOF(1, glm::radians((-1.0 * camera->GetAzimuth()) - 90.0f));
  glm::vec3 root_pos = root->GetPosition();

  // rotate (1, 0, 0) to get the new left/right axis
  glm::vec3 side_axis = GetSide(camera);
  float sideX = side_axis[0];  // x component of left/right axis
  float sideZ = side_axis[2];
  float dx, dz;

  if (abs(int(sideX)) == 1) {
    dz = 0;
    dx = sideX * delta;
  } else if (abs(int(sideZ)) == 1) {
    dx = 0;
    dz = -1 * sideZ * delta;
  } else {
    float m = sideZ / sideX;
    dx = sqrt(-4 * (1 + pow(m, 2)) * (-1 * pow(delta, 2))) /
         (2 * (1 + pow(m, 2)));
    if (sideX < 0) {
      dx *= -1;
    }
    dz = -1 * dx * m;
  }
  root_pos[0] += dx;
  root_pos[2] += dz;
  root->SetPosition(root_pos);
}

void Skeleton::MoveLeft(Camera* camera, float delta) {
  root->SetDOF(1, glm::radians((-1.0 * camera->GetAzimuth()) + 90.0f));
  glm::vec3 root_pos = root->GetPosition();

  glm::vec3 side_axis = GetSide(camera);
  float sideX = side_axis[0];  // x component of left/right axis
  float sideZ = side_axis[2];
  float dx, dz;

  if (abs(int(sideX)) == 1) {
    dz = 0;
    dx = sideX * delta;
  } else if (abs(int(sideZ)) == 1) {
    dx = 0;
    dz = -1 * sideZ * delta;
  } else {
    float m = sideZ / sideX;
    dx = sqrt(-4 * (1 + pow(m, 2)) * (-1 * pow(delta, 2))) /
         (2 * (1 + pow(m, 2)));
    if (sideX < 0) {
      dx *= -1;
    }
    dz = -1 * dx * m;
  }
  root_pos[0] -= dx;
  root_pos[2] -= dz;
  root->SetPosition(root_pos);
}

void Skeleton::Move(GLFWwindow* window, Camera* camera, float deltaTime,
                    std::map<std::string,AnimationPlayer*> anims, float delta) {
  AnimationPlayer* anim = anims["walk"];
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    MoveForward(camera, delta);
    glm::vec3 root_pos = root->GetPosition();
    std::vector<DOF> root_rot = root->GetDOFS();
    anim->Update(deltaTime);
    SetPose(anim->GetCurrentPose());
    root->SetPosition(root_pos);
    root->SetDOF(0, root_rot[0].GetValue());
    root->SetDOF(1, root_rot[1].GetValue());
    root->SetDOF(2, root_rot[2].GetValue());
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    MoveBackward(camera, delta);
    glm::vec3 root_pos = root->GetPosition();
    std::vector<DOF> root_rot = root->GetDOFS();
    anim->Update(deltaTime);
    SetPose(anim->GetCurrentPose());
    root->SetPosition(root_pos);
    root->SetDOF(0, root_rot[0].GetValue());
    root->SetDOF(1, root_rot[1].GetValue());
    root->SetDOF(2, root_rot[2].GetValue());
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    MoveRight(camera, delta);
    glm::vec3 root_pos = root->GetPosition();
    std::vector<DOF> root_rot = root->GetDOFS();
    anim->Update(deltaTime);
    SetPose(anim->GetCurrentPose());
    root->SetPosition(root_pos);
    root->SetDOF(0, root_rot[0].GetValue());
    root->SetDOF(1, root_rot[1].GetValue());
    root->SetDOF(2, root_rot[2].GetValue());
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    MoveLeft(camera, delta);
    glm::vec3 root_pos = root->GetPosition();
    std::vector<DOF> root_rot = root->GetDOFS();
    anim->Update(deltaTime);
    SetPose(anim->GetCurrentPose());
    root->SetPosition(root_pos);
    root->SetDOF(0, root_rot[0].GetValue());
    root->SetDOF(1, root_rot[1].GetValue());
    root->SetDOF(2, root_rot[2].GetValue());
  }

  camera->Move(window, delta);
}

glm::vec3 Skeleton::getPos() { return root->GetPosition(); }