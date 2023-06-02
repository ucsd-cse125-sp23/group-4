////////////////////////////////////////
// ParticleSystem.h
// By Matthew Lawrence
////////////////////////////////////////

#pragma once

#include "core.h"
using namespace ::std;
using namespace ::glm;

#include <sstream>

#include "Cube.h"
#include "DOF.h"
#include "Particle.h"

////////////////////////////////////////////////////////////////////////////////

// The ParticleSystem class for spawning and managing many particles.

class ParticleSystem {
 public:
  ParticleSystem() : rotation(vec4(0)) {
    particles.reserve(_maxParticles);

    box = new Cube(vec3(-0.05f), vec3(0.05f));
  }
  ~ParticleSystem() {
    if (box) delete box;
  }

  vec3 GetPosition() {
    return vec3(position[0].GetValue(), position[1].GetValue(),
                position[2].GetValue());
  }
  void SetPosition(vec3 pos) {
    position[0].SetValue(pos.x);
    position[1].SetValue(pos.y);
    position[2].SetValue(pos.z);
  }

  void Reset() {
    for (Particle* p : particles) {
      delete p;
      p = nullptr;
    }

    particles.clear();
  }

  void Emit(int amount);
  void Update(float deltaTime);
  void Draw(const DrawInfo info);
  void Show(const string name);

 private:
  vector<DOF> position = {DOF(), DOF(), DOF()};  // System position
  vec4 rotation;

  Cube* box;

  bool showBox = true;

  // Spawning
  void Emit();
  void Spawner(float deltaTime);
  float timer = 0.0f;     // Internal timer for emissions
  float duration = 1.0f;  // Negative for infinite loop

  float creationRate = 1.0f;        // How many emissions per second
  unsigned int creationAmount = 1;  // How many particles to emit
  float _roundOffError = 0.0f;

  const unsigned int _maxParticles = 1000;

  vector<Particle*> particles;

  // For maintaining vector structure
  void DeleteParticleAt(int index) {
    Particle* pRemove = particles[index];
    Particle* pSwap = particles.back();

    particles[index] = pSwap;  // Put end element in empty spot

    delete pRemove;
    pRemove = nullptr;

    particles.pop_back();  // Pop end element out
  }

  // Properties
  float initColor[4] = {1.0f, 0.9f, 0.3f, 1.0f};
  vector<DOFr> initPosition = {DOFr(-50, 50, 0), DOFr(-50, 50, 0),
                               DOFr(-50, 50, 0)};  // Ranges for offset
  vector<DOFr> initVelocity = {DOFr(-50, 50, 0), DOFr(-50, 50, 0),
                               DOFr(-50, 50, 0)};  // Ranges
  DOF radialPosition = DOF(0, 50, 0);
  DOF radialVelocity = DOF(0, 100, 0);

  DOF initMass = DOF(0.1f, 10, 0.1f);

  DOFr lifespan = DOFr(0, 50, 2);
  DOF radius = DOF(0.1f, 2, 0.1f);

  DOF gravity = DOF(-20, 4.9f, -9.8f);

  const vec3 vairdir = vec3(0, 0, 1);  // wind
  float windspeed = 4.0f;
  float winddir = 0.0f;

  vec3 windvelocity = vec3(0);
  void SetWindVelocity() {
    windvelocity =
        (vec3(normalize(vec4(vairdir, 1) * rotate(winddir, vec3(0, 1, 0)))) *
         windspeed);
  }

  DOF airdensity = DOF(0, 1000, 1.2f);
  DOF drag = DOF(0, 1.5f, 0.1f);

  bool collision = true;
  DOF elasticity = DOF(0, 1, 0.1f);
  DOF friction = DOF(0, 1.5f, 0.1f);
};

////////////////////////////////////////////////////////////////////////////////
