////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

#include "glm/gtx/euler_angles.hpp"

////////////////////////////////////////////////////////////////////////////////

Camera::Camera() {
	Reset();
	Fixed = true;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Update() {
	//// Compute camera world matrix
	//glm::mat4 world(1);
	//world[3][2]=Distance;
	//world=glm::eulerAngleY(glm::radians(-Azimuth)) * glm::eulerAngleX(glm::radians(-Incline)) * world;

	// Compute view matrix (inverse of world matrix)
	glm::mat4 view=glm::inverse(World);

	// Compute perspective projection matrix
	glm::mat4 project=glm::perspective(glm::radians(FOV),Aspect,NearClip,FarClip);

	// Compute final view-projection matrix
	ViewProjectMtx=project * view;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Reset() {
	FOV=45.0f;
	Aspect=1.33f;
	NearClip=0.1f;
	FarClip=100.0f;

	Distance=10.0f;
	Azimuth=0.0f;
	Incline=20.0f;

	World = glm::mat4(1);
	World[3][2] = Distance;
	World = glm::eulerAngleY(glm::radians(-Azimuth)) * glm::eulerAngleX(glm::radians(-Incline)) * World;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::ToggleFixedCamera(GLFWwindow* window) {
	Fixed = !Fixed;
	if (Fixed) {
		printf("Camera fixed\n");
	}
	else {
		printf("Camera not fixed\n");
	}
}

void Camera::Move(GLFWwindow* window, float delta) {
	if (!Fixed) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			World = glm::translate(glm::vec3(0, 0, -delta)) * World;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			World = glm::translate(glm::vec3(0, 0, delta)) * World;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			World = glm::translate(glm::vec3(delta,0, 0)) * World;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			World = glm::translate(glm::vec3(-delta, 0, 0)) * World;
		}
	}	
}
