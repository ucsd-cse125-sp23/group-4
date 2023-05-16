#include "core/util/global.h"

int TAG_COOLDOWN = 20;
float JUMP_VELOCITY = 0.3f;
float GRAVITY_STRENGTH = 0.01f;

int PLAYER_LAYER = 0;
int ENVIRONMENT_LAYER = 1;
int POWER_LAYER = 2;

SphereShape* POWERUP_BASE_SHAPE = new SphereShape(0.4);
CylinderShape* PLAYER_BASE_SHAPE = new CylinderShape(1.0, 0.5);
OffsetShape* PLAYER_BOUNDING_SHAPE =
    new OffsetShape(PLAYER_BASE_SHAPE, vec3f(0.0f, 0.5f, 0.0f));

Level* level = nullptr;

ControlModifier* CONTROL_MODIFIER = new ControlModifier();
GravityModifier* GRAVITY_MODIFIER = new GravityModifier();
TaggedStatusModifier* TAGGED_STATUS_MODIFIER = new TaggedStatusModifier();

SpeedBoostModifier* SPEEDBOOST_MODIFIER = new SpeedBoostModifier();
AttractModifier* ATTRACT_MODIFIER = new AttractModifier();
FreezeModifier* FREEZE_MODIFIER = new FreezeModifier();


StaticGlobalEffect* SPEEDBOOST_EFFECT = new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
	for (auto target : targets)
		target->addModifierInstance(new ModifierInstance(SPEEDBOOST_MODIFIER, new SpeedBoostModifierData(100, 0.2f)));
	});
StaticGlobalEffect* SLOWDOWN_EFFECT = new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
	for (auto target : targets)
		target->addModifierInstance(new ModifierInstance(SPEEDBOOST_MODIFIER, new SpeedBoostModifierData(100, -0.2f)));
	});
StaticGlobalEffect* FREEZE_EFFECT = new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
	for (auto target : targets)
		target->addModifierInstance(new ModifierInstance(FREEZE_MODIFIER, new FreezeModifierData(40)));
	});
StaticGlobalEffect* REVERSE_EFFECT = new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
	for (auto target : targets)
		target->addModifierInstance(new ModifierInstance(SPEEDBOOST_MODIFIER, new SpeedBoostModifierData(100, -2.0f)));
	});
StaticGlobalEffect* LAUNCH_EFFECT = new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
	for (auto target : targets)
		target->vel += vec3f(0.0f, 2.0f, 0.0f);
	});

StaticGlobalEffect* SLOW_FALL_EFFECT;
StaticGlobalEffect* FAST_FALL_EFFECT;
StaticGlobalEffect* SWAP_POSITIONS_EFFECT;

StaticGlobalEffect* ATTRACT_EFFECT = nullptr;