////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

#include "glm/gtx/euler_angles.hpp"

////////////////////////////////////////////////////////////////////////////////

Camera::Camera() {
  Reset();
  Fixed = false;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Update(GLFWwindow* window) {
  //// Compute camera world matrix
  /*World = glm::mat4(1);
  World[3][2]=Distance;
  World=glm::eulerAngleY(glm::radians(-Azimuth)) *
  glm::eulerAngleX(glm::radians(-Incline)) * World;*/

  // Compute view matrix (inverse of world matrix)
  glm::mat4 view = glm::inverse(World);

  // Compute perspective projection matrix
  glm::mat4 project =
      glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

  // Compute final view-projection matrix
  ViewProjectMtx = project * view;
  KeyInput(window);
}

////////////////////////////////////////////////////////////////////////////////

void Camera::UpdateWorld() {
  // World = glm::eulerAngleY(glm::radians(-Azimuth)) *
  // glm::eulerAngleX(glm::radians(-Incline)) * World;
  World = glm::mat4(1);
  World[3][2] = Distance;
  World = glm::eulerAngleY(glm::radians(-Azimuth)) *
          glm::eulerAngleX(glm::radians(-Incline)) * World;

  World = glm::translate(Translations) * World;
}

void Camera::Reset() {
	FOV=45.0f;
	Aspect=1.33f;
	NearClip=0.1f;
	FarClip=500.0f;

  Distance = 10.0f;
  Azimuth = -0.0f;
  Incline = 20.0f;

  XTranslation = 0.0f;
  ZTranslation = 0.0f;

  World = glm::mat4(1);
  World[3][2] = Distance;
  World = glm::eulerAngleY(glm::radians(-Azimuth)) *
          glm::eulerAngleX(glm::radians(-Incline)) * World;

  Translations = glm::vec3(0.0, 0.0, 0.0);
}

////////////////////////////////////////////////////////////////////////////////

void Camera::ToggleFixedCamera() {
  Fixed = !Fixed;
  if (Fixed) {
    printf("Camera fixed\n");
  } else {
    printf("Camera not fixed\n");
  }
}

void Camera::KeyInput(GLFWwindow* window, float delta) {
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    Incline += delta;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    Incline -= delta;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    Azimuth += delta;
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    Azimuth -= delta;
  }
  UpdateWorld();
}

void Camera::Move(GLFWwindow* window, float delta) {
  if (!Fixed) {
    float forwardX = glm::sin(glm::radians(Azimuth));
    float forwardZ = glm::cos(glm::radians(Azimuth));
    float sideX = glm::cos(glm::radians(Azimuth));
    float sideZ = -1 * glm::sin(glm::radians(Azimuth));
    float dx, dz;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      if (abs(int(forwardZ)) == 1) {
        World = glm::translate(glm::vec3(0, 0, -delta)) * World;
        Translations[2] -= delta;
      } else if (abs(int(forwardX)) == 1) {
        World = glm::translate(glm::vec3(delta, 0, 0)) * World;
        Translations[0] += delta;
      } else {
        float m = forwardZ / forwardX;
        dx = sqrt(-4 * (1 + pow(m, 2)) * (-1 * pow(delta, 2))) /
             (2 * (1 + pow(m, 2)));
        if (forwardX < 0) {
          dx *= -1;
        }
        dz = dx * m;
        World = glm::translate(glm::vec3(dx, 0, -dz)) * World;
        Translations[0] += dx;
        Translations[2] -= dz;
      }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      if (abs(int(forwardZ)) == 1) {
        World = glm::translate(glm::vec3(0, 0, delta)) * World;
        Translations[2] += delta;
      } else if (abs(int(forwardX)) == 1) {
        World = glm::translate(glm::vec3(-delta, 0, 0)) * World;
        Translations[0] -= delta;
      } else {
        float m = forwardZ / forwardX;
        dx = sqrt(-4 * (1 + pow(m, 2)) * (-1 * pow(delta, 2))) /
             (2 * (1 + pow(m, 2)));
        if (forwardX < 0) {
          dx *= -1;
        }
        dz = dx * m;
        World = glm::translate(glm::vec3(-dx, 0, dz)) * World;
        Translations[0] -= dx;
        Translations[2] += dz;
      }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      if (abs(int(sideX)) == 1) {
        dx = sideX * delta;
        World = glm::translate(glm::vec3(dx, 0, 0)) * World;
        Translations[0] += dx;
      } else if (abs(int(sideZ)) == 1) {
        dz = -1 * sideZ * delta;
        World = glm::translate(glm::vec3(0, 0, dz));
        Translations[2] += dz;
      } else {
        float m = sideZ / sideX;
        dx = sqrt(-4 * (1 + pow(m, 2)) * (-1 * pow(delta, 2))) /
             (2 * (1 + pow(m, 2)));
        if (sideX < 0) {
          dx *= -1;
        }
        dz = -1 * dx * m;
        World = glm::translate(glm::vec3(dx, 0, dz)) * World;
        Translations[0] += dx;
        Translations[2] += dz;
      }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      if (abs(int(sideX)) == 1) {
        dx = sideX * delta;
        World = glm::translate(glm::vec3(-dx, 0, 0)) * World;
        Translations[0] -= dx;
      } else if (abs(int(sideZ) == 1)) {
        dz = -1 * sideZ * delta;
        World = glm::translate(glm::vec3(0, 0, -dz)) * World;
        Translations[2] -= dz;
      } else {
        float m = sideZ / sideX;
        dx = sqrt(-4 * (1 + pow(m, 2)) * (-1 * pow(delta, 2))) /
             (2 * (1 + pow(m, 2)));
        if (sideX < 0) {
          dx *= -1;
        }
        dz = -1 * dx * m;
        World = glm::translate(glm::vec3(-dx, 0, -dz)) * World;
        Translations[0] -= dx;
        Translations[2] -= dz;
      }
    }
  }
}
