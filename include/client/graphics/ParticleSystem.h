////////////////////////////////////////
// ParticleSystem.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include "client/graphics/core.h"
using glm::vec3;
using glm::vec4;

#include <sstream>
#include <string>
#include <vector>

#include "client/graphics/Cube.h"
#include "client/graphics/DOF.h"
#include "client/graphics/GameThing.h"
#include "client/graphics/Material.h"
#include "client/graphics/Mesh.h"
#include "client/graphics/Particle.h"

////////////////////////////////////////////////////////////////////////////////

// The ParticleSystem class for spawning and managing many particles.

class ParticleSystem : public GameThing {
 public:
  Mesh* meshRef;
  Material* materialRef;

  // Properties
  bool worldSpace = false;
  float creationRate = 5.0f;        // How many emissions per second
  unsigned int creationAmount = 1;  // How many particles to emit

  float initColor[4] = {1.0f, 0.9f, 0.3f, 1.0f};
  std::vector<DOFr> initPosition = {DOFr(-50, 50, 0), DOFr(-50, 50, 0),
                                    DOFr(-50, 50, 0)};  // Ranges for offset
  std::vector<DOFr> initVelocity = {DOFr(-50, 50, 0), DOFr(-50, 50, 0),
                                    DOFr(-50, 50, 0)};  // Ranges
  DOF radialPosition = DOF(0, 50, 0);
  DOF radialVelocity = DOF(0, 100, 4);

  DOF initMass = DOF(0.1f, 10, 0.1f);

  DOFr lifespan = DOFr(0, 50, 2);
  DOF radius = DOF(0.1f, 2, 0.1f);

  DOF gravity = DOF(-20, 4.9f, -9.8f);

  DOF airdensity = DOF(0, 1000, 1.2f);
  DOF drag = DOF(0, 1.5f, 0.1f);

  bool collision = false;
  DOF elasticity = DOF(0, 1, 0.1f);
  DOF friction = DOF(0, 1.5f, 0.1f);

  ParticleSystem() {
    particles.reserve(_maxParticles);

    meshRef = nullptr;
    materialRef = nullptr;
  }

  ~ParticleSystem() {}

  void Reset(bool deletion = true) {
    if (deletion) {
      for (Particle* p : particles) {
        delete p;
        p = nullptr;
      }
    }

    particles.clear();
    particles.reserve(_maxParticles);
  }

  void Emit(int amount);

  void update(float dt) override;
  void draw(const DrawInfo info, const glm::mat4& parentMtx) override;
  void show(const std::string name);

 private:
  // Spawning
  void Emit();
  void Spawner(float deltaTime);
  float timer = 0.0f;     // Internal timer for emissions
  float duration = 1.0f;  // Negative for infinite loop

  float _roundOffError = 0.0f;

  const unsigned int _maxParticles = 1000;

  std::vector<Particle*> particles;

  // For maintaining vector structure
  void DeleteParticleAt(int index) {
    Particle* pRemove = particles[index];
    Particle* pSwap = particles.back();

    particles[index] = pSwap;  // Put end element in empty spot

    delete pRemove;
    pRemove = nullptr;

    particles.pop_back();  // Pop end element out
  }

  // (private) Properties
  const vec3 vairdir = vec3(0, 0, 1);  // wind
  float windspeed = 4.0f;
  float winddir = 0.0f;

  vec3 windvelocity = vec3(0);
  void SetWindVelocity() {
    windvelocity =
        (vec3(normalize(vec4(vairdir, 1) * rotate(winddir, vec3(0, 1, 0)))) *
         windspeed);
  }
};

////////////////////////////////////////////////////////////////////////////////
