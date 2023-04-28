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
#include "AnimationPlayer.h"

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
	void Move(GLFWwindow* window, Camera* camera, float deltaTime, AnimationPlayer* anim, float delta = 0.25);

	glm::mat4 GetWorldMatrix(int joint);

	void SetPose(const Pose pose);
	void MoveForward(Camera* camera, float delta);
	void MoveBackward(Camera* camera, float delta);
	void MoveRight(Camera* camera, float delta);
	void MoveLeft(Camera* camera, float delta);
private:
	std::vector<Joint*> joints;	// random access

	Joint* root;	// tree structure

	void SetJointDOFs(int joint, glm::vec3 dof);
	Joint* GetJoint(int j);
	Joint* FindJointInTree(int j);
};

////////////////////////////////////////////////////////////////////////////////
