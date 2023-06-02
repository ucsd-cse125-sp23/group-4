#include "Particle.h"

void Particle::Update(float deltaTime) {
  // Any local forces

  // Aerodynamic drag
  vec3 v = velocity;
  if (windVelocity) v = velocity - *windVelocity;
  vec3 e = -unit(v);
  vec3 fdrag = 0.5f * airDensity * (float)pow(length(v), 2) * drag * area() * e;

  AddForce(fdrag);

  // Integrate kinematics
  Integrate(deltaTime);
}

bool Particle::PostUpdate(float deltaTime) {
  // Check constraints/lifespan

  // Lifespan
  if (lifespan >= 0.0f && lifeTimer > lifespan) return false;
  lifeTimer += deltaTime;

  // Update mesh
  particleMtx = translate(position) * glm::scale(vec3(radius * 2.0f));

  return true;
}

static mat4 alignToViewMtx(const vec3 pos, const mat4 vMatrix) {
  // Face camera
  vec3 camRight = vec3(vMatrix[0][0], vMatrix[1][0], vMatrix[2][0]);
  vec3 camUp = vec3(vMatrix[0][1], vMatrix[1][1], vMatrix[2][1]);
  vec3 camFwd = cross(camUp, camRight);

  return glm::inverse(glm::lookAt(pos, pos - camFwd, camUp));
}

void Particle::draw(const DrawInfo info) {
  // Face camera
  mat4 vMatrix = mat4(info.viewMtx);
  vec3 pos = vec3(particleMtx[3]);

  particleMtx = alignToViewMtx(pos, vMatrix);

  // draw
}

float Particle::CollisionLowBound() { return 0.0f; }

void Particle::Integrate(float dt) {
  if (fixed) return;

  vec3 accel = (1 / mass) * force;
  accel += forceraw;
  velocity += accel * dt;
  position += velocity * dt;

  force = vec3(0);
  forceraw = vec3(0);

  // Check ground
  if ((!collisions || *collisions) && position.y < CollisionLowBound()) {
    float delta = abs(CollisionLowBound() - position.y);
    position.y = CollisionLowBound();  // New ending position

    // Response:
    vec3 n = vec3(0, 1, 0);

    // Restitution/frictionless
    float vclose = dot(velocity, n);
    vec3 deltaBounce = (-(1 + restitution)) * vclose *
                       n;  // Note impulse, since mass is taken out

    // Friction
    vec3 vnorm = vclose * n;
    vec3 vtan = velocity - vnorm;
    // Opposite direction of tangential velocity,
    // With magnitude (drag coeff) * |j|
    vec3 deltaFriction = -unit(vtan) * friction * length(deltaBounce);
    deltaFriction =
        clampMagnitude(deltaFriction, length(vtan));  // cap friction

    velocity += deltaBounce;
    velocity += deltaFriction;
  }
}
