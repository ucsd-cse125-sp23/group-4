////////////////////////////////////////
// Particle.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include "core.h"
using glm::mat4;
using glm::vec3;
using glm::vec4;

#include "Cube.h"
#include "Mesh.h"

////////////////////////////////////////////////////////////////////////////////

// The Particle class for dealing with physics at points.

class Particle {
 public:
  Mesh* mesh;

  mat4 particleMtx;  // applied first!
  vec3 colorTint;

  Particle(vec3 pos, vec3 vel, float m)
      : position(pos), velocity(vel), refPosition(pos), mass(m) {
    force = vec3(0);

    // mesh = new Quad();
    // mesh->setMatrix(translate(position));
    fixed = false;
  }
  Particle(vec3 pos, float m) : Particle(pos, vec3(0), m) {}
  Particle(vec3 pos) : Particle(pos, vec3(0), 0.25f) {}
  ~Particle() {
    if (mesh) delete mesh;
  }

  void AddForce(vec3& f) { force += f; }
  void AddForceRaw(vec3& f) { forceraw += f; }
  vec3 GetPosition() { return position; }
  vec3 GetVelocity() { return velocity; }
  void SetFixed(bool f) {
    fixed = f;
    if (fixed) {
      velocity = vec3(0);
      force = vec3(0);
    }
  }
  void SetColor(vec4 color) { colorTint = vec3(color); }
  void SetRadius(float r) { radius = r; }
  void SetLifespan(float duration) { lifespan = duration; }
  void ConfigureAir(float density, float dragCoeff, vec3* windVel) {
    airDensity = density;
    drag = dragCoeff;
    windVelocity = windVel;
  }
  void ConfigureCollisions(float elasticity, float friction, bool* collide) {
    restitution = elasticity;
    Particle::friction = friction;
    collisions = collide;
  }

  void SetOffset(vec3 off) { position = refPosition + off; }

  void Update(float deltaTime);
  bool PostUpdate(float deltaTime);  // Return false if should be destroyed
  void draw(const DrawInfo info);

 private:
  const vec3 refPosition;

  vec3 position;
  vec3 velocity;
  vec3 force;
  vec3 forceraw;  // Gravity
  float mass;

  float radius = 1.0f;  // Also for air
  float lifespan = -1.0f;
  float lifeTimer = 0.0f;

  bool fixed;

  vec3* windVelocity = nullptr;
  float airDensity = 0.0f;
  float drag = 0.0f;  // Coefficient
  float area() { return glm::pi<float>() * (float)pow(radius, 2); }

  bool* collisions = nullptr;
  float restitution = 0.1f;  // elasticity
  float friction = 0.1f;     // just dynamic
  float CollisionLowBound();

  void Integrate(float dt);
};

////////////////////////////////////////////////////////////////////////////////
