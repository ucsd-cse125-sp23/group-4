#include "ParticleSystem.h"

using std::string;

void ParticleSystem::Emit(int amount) {
  amount *= creationAmount;
  for (int i = 0; i < amount; i++) Emit();
}

void ParticleSystem::Emit() {
  if (particles.size() >= particles.capacity()) return;

  // TODO(matthew): support world/local space option
  Particle* p =
      new Particle(DOFr::GetRandomVector3(initPosition) +
                       (Tools::randomizeSphere() * radialPosition.GetValue()),
                   DOFr::GetRandomVector3(initVelocity) +
                       (Tools::randomizeSphere() * radialVelocity.GetValue()),
                   initMass.GetValue());

  p->SetColor(vec4(initColor[0], initColor[1], initColor[2], initColor[3]));
  p->SetRadius(radius.GetValue());
  p->SetLifespan(lifespan.GetRandomValue());

  p->ConfigureAir(airdensity.GetValue(), drag.GetValue(), &windvelocity);
  p->ConfigureCollisions(elasticity.GetValue(), friction.GetValue(),
                         &collision);

  p->particleModel.mesh = meshRef;
  p->particleModel.material = materialRef;

  particles.push_back(p);
}

void ParticleSystem::update(float dt) {
  // box->setMatrix(translate(GetPosition()));
  Spawner(dt);

  // Compute forces
  // For each particle: apply gravity
  for (Particle* p : particles) {
    p->AddForceRaw(vec3(0, gravity.GetValue(), 0));  // gravity
  }

  // Integrate
  // For each particle: compute physics
  for (Particle* p : particles) {
    p->Update(dt);  // integrate
  }

  // Apply constraints
  // For each particle: check intersections
  for (int i = 0; i < particles.size(); i++) {
    Particle* p = particles[i];

    bool keep = p->PostUpdate(dt);  // integrate

    if (!keep) {
      DeleteParticleAt(i);
    }
  }
}

void ParticleSystem::Spawner(float deltaTime) {
  timer += deltaTime;

  float num = deltaTime * creationRate + _roundOffError;
  int n = static_cast<int>(num);
  _roundOffError = num - static_cast<float>(n);

  Emit(n);
}

void ParticleSystem::draw(const DrawInfo info, const glm::mat4& parentMtx) {
  const mat4 viewProjMtx = info.viewProjMtx;
  GLint shader = info.shader;

  // if(showBox) box->draw(viewProjMtx, shader);

  for (Particle* p : particles) {
    p->draw(info, parentMtx * transformMtx);  // draw particles
  }
}

void ParticleSystem::show(const std::string name) {
  /*// Tweak bar
  bar = TwNewBar(name.c_str());

  TwDefine((" " + name + " color = '175 55 215' ").c_str());
  TwDefine((" " + name + " position='300 10' ").c_str());
  TwDefine((" " + name + " size='180 150' ").c_str());
  TwDefine((" " + name + " refresh=1.5 ").c_str());


  TwAddVarRO(bar, "Max Particles", TW_TYPE_UINT32, &_maxParticles, " ");
  TwAddButton(bar, "Clear Particles", reset, this, " ");
  TwAddSeparator(bar, "hline", " ");
  TwAddVarRW(bar, "Color", TW_TYPE_COLOR4F, &initColor, "");
  TwAddVarRW(bar, "Draw Origin", TW_TYPE_BOOLCPP, &showBox, "true=SHOW
  false=HIDE");

  string pname = "Origin";
  // Fixed positions
  std::ostringstream def;
  def << pname << ".X";
  string defstrX = def.str();

  std::ostringstream def2;
  def2 << pname << ".Y";
  string defstrY = def2.str();

  std::ostringstream def3;
  def3 << pname << ".Z";
  string defstrZ = def3.str();

  position[0].Show(bar, defstrX.c_str(), pname.c_str());
  position[1].Show(bar, defstrY.c_str(), pname.c_str());
  position[2].Show(bar, defstrZ.c_str(), pname.c_str());

  // Emission
  TwAddVarRW(bar, "Rate", TW_TYPE_FLOAT, &creationRate, " min=0.0f max=50.0f
  step=1.0f group=Emission "); TwAddVarRW(bar, "Amount", TW_TYPE_UINT32,
  &creationAmount, " min=0 max=50 group=Emission "); TwAddButton(bar, "Emit
  Once", emit, this, " group=Emission ");

  // Transform
  radialPosition.Show(bar, "Offset.Radial", "Transform");
  initPosition[0].Show(bar, "Offset.X", "RandomOffset");
  initPosition[1].Show(bar, "Offset.Y", "RandomOffset");
  initPosition[2].Show(bar, "Offset.Z", "RandomOffset");

  TwDefine(string(name + "/RandomOffset group=Transform").c_str());	// Move
  name/A into group Transform TwDefine(string(name + "/RandomOffset
  opened=false").c_str());	// Fold the group

  TwAddSeparator(bar, "hlineTr", " group=Transform ");

  radialVelocity.Show(bar, "Speed.Radial", "Transform");
  initVelocity[0].Show(bar, "Speed.X", "RandomSpeed");
  initVelocity[1].Show(bar, "Speed.Y", "RandomSpeed");
  initVelocity[2].Show(bar, "Speed.Z", "RandomSpeed");


  TwDefine(string(name + "/RandomSpeed group=Transform").c_str());
  TwDefine(string(name + "/RandomSpeed opened=false").c_str());


  // Particles
  lifespan.Show(bar, "Lifespan", "Particles");
  initMass.Show(bar, "Mass", "Particles");
  radius.Show(bar, "Radius", "Particles");



  // Forces
  gravity.Show(bar, "Gravity", "Forces");
  airdensity.Show(bar, "Air density", "Forces");
  drag.Show(bar, "Drag", "Forces");
  TwAddVarRW(bar, "Collision", TW_TYPE_BOOLCPP, &collision, " group=Forces ");
  elasticity.Show(bar, "Elasticity", "Forces");
  friction.Show(bar, "Friction", "Forces");*/
}
