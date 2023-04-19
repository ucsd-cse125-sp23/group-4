#include "Skeleton.h"


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
			dfs.push_back(c->GetChildReversed(i++));	// Add children to stack
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
	token.GetToken(tempname);	// name
	root = new Joint(&tempname[0]);
	root->Load(token);

	// Finish
	token.Close();

	// Initializes joints array
	BuildJointArray(root, &joints);
	facing = Direction::Forward;

	return true;
}

void Skeleton::Update()
{
	if (root) root->Update(glm::mat4(1.0));
}

void Skeleton::Draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	if (root) root->Draw(viewProjMtx, shader);
}

void Skeleton::Show()
{
	if (root) root->ShowAll();
}

void Skeleton::SetPose(const Pose pose) {
	for (int i = 0; i < pose.size(); i+=3)
	{
		int j = (i / 3) - 1;	// account for root index!

		glm::vec3 p = glm::vec3(pose[i], pose[i + 1], pose[i + 2]);

		SetJointDOFs(j, p);
	}
}

void Skeleton::SetJointDOFs(int joint, glm::vec3 dof)
{
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
glm::mat4 Skeleton::GetWorldMatrix(int joint)
{
	Joint* j = GetJoint(joint);
	if (!j) {
		return glm::mat4(-1.0);
	}

	return j->GetWorldMatrix();
}

Joint* Skeleton::GetJoint(int j)
{
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

		if (j-- == 0) return c;	// Return current joint

		int i = 0;
		while (c->GetChildReversed(i)) {
			dfs.push_back(c->GetChildReversed(i++));	// Add children to stack
		}
	}

	return nullptr;
}

void Skeleton::MoveForward(float delta) {
	glm::vec3 root_pos = root->GetPosition();
	root_pos[2] -= delta;
	root->SetPosition(root_pos);
}

void Skeleton::MoveBackward(float delta) {
	glm::vec3 root_pos = root->GetPosition();
	root_pos[2] += delta;
	root->SetPosition(root_pos);
}

void Skeleton::MoveRight(float delta) {
	glm::vec3 root_pos = root->GetPosition();
	root_pos[0] += delta;
	root->SetPosition(root_pos);
}

void Skeleton::MoveLeft(float delta) {
	glm::vec3 root_pos = root->GetPosition();
	root_pos[0] -= delta;
	root->SetPosition(root_pos);
}

void Skeleton::TurnAround() {
	std::vector<DOF> root_dofs = root->GetDOFS();
	root->SetDOF(1, root_dofs[1].GetValue() + M_PI);
}

void Skeleton::TurnLeft() {
	std::vector<DOF> root_dofs = root->GetDOFS();
	root->SetDOF(1, root_dofs[1].GetValue() + (M_PI / 2));
}

void Skeleton::TurnRight() {
	std::vector<DOF> root_dofs = root->GetDOFS();
	root->SetDOF(1, root_dofs[1].GetValue() - (M_PI / 2));
}

void Skeleton::Move(GLFWwindow* window, Camera* camera, float delta) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (facing == Direction::Backward) {
			TurnAround();
		}
		if (facing == Direction::Left) {
			TurnRight();
		}
		if (facing == Direction::Right) {
			TurnLeft();
		}
		facing = Direction::Forward;
		MoveForward();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (facing == Direction::Forward) {
			TurnAround();
		}
		if (facing == Direction::Left) {
			TurnLeft();
		}
		if (facing == Direction::Right) {
			TurnRight();
		}
		facing = Direction::Backward;
		MoveBackward();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (facing == Direction::Forward) {
			TurnRight();
		}
		if (facing == Direction::Backward) {
			TurnLeft();
		}
		if (facing == Direction::Left) {
			TurnAround();
		}
		facing = Direction::Right;
		MoveRight();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (facing == Direction::Forward) {
			TurnLeft();
		}
		if (facing == Direction::Backward) {
			TurnRight();
		}
		if (facing == Direction::Right) {
			TurnAround();
		}
		facing = Direction::Left;
		MoveLeft();
	}

	camera->Move(window, delta);
}