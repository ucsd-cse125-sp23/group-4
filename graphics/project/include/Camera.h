////////////////////////////////////////
// Camera.h
////////////////////////////////////////

#pragma once

#include "core.h"
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
////////////////////////////////////////////////////////////////////////////////

// The Camera class provides a simple means to controlling the 3D camera. It could
// be extended to support more interactive controls. Ultimately. the camera sets the
// GL projection and viewing matrices.

class Camera {
public:
	Camera();

	void Update();
	void Reset();
	void ToggleFixedCamera(GLFWwindow* window);
	void Move(GLFWwindow* window, float delta = 0.25);

	// Access functions
	void SetAspect(float a)					{Aspect=a;}
	void SetDistance(float d)				{Distance=d;}
	void SetAzimuth(float a)				{Azimuth=a;}
	void SetIncline(float i)				{Incline=i;}

	float GetDistance()						{return Distance;}
	float GetAzimuth()						{return Azimuth;}
	float GetIncline()						{return Incline;}

	const glm::mat4 &GetViewProjectMtx()	{return ViewProjectMtx;}

private:
	// Perspective controls
	float FOV;			// Field of View Angle (degrees)
	float Aspect;		// Aspect Ratio
	float NearClip;		// Near clipping plane distance
	float FarClip;		// Far clipping plane distance

	// Polar controls
	float Distance;		// Distance of the camera eye position to the origin (meters)
	float Azimuth;		// Rotation of the camera eye position around the Y axis (degrees)
	float Incline;		// Angle of the camera eye position over the XZ plane (degrees)

	bool Fixed;
	glm::mat4 World;

	// Computed data
	glm::mat4 ViewProjectMtx;
};

////////////////////////////////////////////////////////////////////////////////
