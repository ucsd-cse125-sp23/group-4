////////////////////////////////////////
// Skeleton.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "core.h"
#include "Pose.h"
#include "Joint.h"
#include "Camera.h"

enum class Direction {
	Forward,
	Backward,
	Left,
	Right
};

////////////////////////////////////////////////////////////////////////////////

// The Skeleton class for holding joints of a skeleton tree structure.

class Skeleton 
{
public:
	Skeleton()
	{
		root = nullptr;
	}
	~Skeleton()
	{
		delete root;
	}

	bool Load(const char* file);
	void Update();
	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Show();	// GUI
	void Move(GLFWwindow* window, Camera* camera, float delta = 0.25);

	glm::mat4 GetWorldMatrix(int joint);

	void SetPose(const Pose pose);
	void MoveForward(float delta = 0.25);
	void MoveBackward(float delta = 0.25);
	void MoveRight(float delta = 0.25);
	void MoveLeft(float delta = 0.25);
	void TurnAround();
	void TurnLeft();
	void TurnRight();
private:
	std::vector<Joint*> joints;	// random access

	Joint* root;	// tree structure

	void SetJointDOFs(int joint, glm::vec3 dof);
	Joint* GetJoint(int j);
	Joint* FindJointInTree(int j);
	Direction facing;
};

////////////////////////////////////////////////////////////////////////////////
