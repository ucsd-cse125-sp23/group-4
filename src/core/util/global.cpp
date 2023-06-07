#include "core/util/global.h"

#include <chrono>
#include <random>

#include "core/game/effect/AttractEffects.h"

int COYOTE_TIME = 2;
int TAG_COOLDOWN = 20;
float MOVE_VELOCITY = 1.2f;
float JUMP_VELOCITY = 1.7f;
float GRAVITY_STRENGTH = 0.2f;

int PLAYER_LAYER = 0;
int ENVIRONMENT_LAYER = 1;
int POWER_LAYER = 2;

float PLAYER_RADIUS = 1.3f;
float PLAYER_HEIGHT = 1.0f;

SphereShape* POWERUP_BASE_SHAPE = new SphereShape(0.4);
CapsuleShape* PLAYER_BASE_SHAPE =
    new CapsuleShape(PLAYER_HEIGHT, PLAYER_RADIUS);
OffsetShape* PLAYER_BOUNDING_SHAPE =
    new OffsetShape(PLAYER_BASE_SHAPE, vec3f(0.0f, PLAYER_HEIGHT/2, 0.0f));


ControlModifier* CONTROL_MODIFIER = new ControlModifier();
TaggedStatusModifier* TAGGED_STATUS_MODIFIER = new TaggedStatusModifier();

NumberModifier* GRAVITY_MODIFIER = new NumberModifier();
SpeedBoostModifier* SPEEDBOOST_MODIFIER = new SpeedBoostModifier();
AttractModifier* ATTRACT_MODIFIER = new AttractModifier();
FreezeModifier* FREEZE_MODIFIER = new FreezeModifier();

NumberModifier* FRICTION_MODIFIER = new NumberModifier();

GlobalEffect* SPEEDBOOST_EFFECT =
    new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
      for (auto target : targets)
        target->addModifierInstance(new ModifierInstance(
            SPEEDBOOST_MODIFIER,
            new SpeedBoostModifierData(level, 100, 0.25f)));
    });
GlobalEffect* SLOWDOWN_EFFECT =
    new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
      for (auto target : targets)
        target->addModifierInstance(new ModifierInstance(
            SPEEDBOOST_MODIFIER,
            new SpeedBoostModifierData(level, 100, -0.25f)));
    });
GlobalEffect* FREEZE_EFFECT =
    new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
      for (auto target : targets)
        target->addModifierInstance(new ModifierInstance(
            FREEZE_MODIFIER, new FreezeModifierData(level, 40)));
    });
GlobalEffect* REVERSE_EFFECT =
    new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
      for (auto target : targets)
        target->addModifierInstance(new ModifierInstance(
            SPEEDBOOST_MODIFIER,
            new SpeedBoostModifierData(level, 100, -2.0f)));
    });
GlobalEffect* LAUNCH_EFFECT =
    new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
      for (auto target : targets) {
        target->vel += vec3f(0.0f, 2.0f, 0.0f);
        target->onGround = 0;
      }
    });
GlobalEffect* SLOW_FALL_EFFECT =
    new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
      for (auto target : targets)
        target->addModifierInstance(new ModifierInstance(
            GRAVITY_MODIFIER,
            new NumberModifierData(level, Operation::MULTIPLY, 0.5, 200)));
    });
GlobalEffect* FAST_FALL_EFFECT =
    new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
      for (auto target : targets)
        target->addModifierInstance(new ModifierInstance(
            GRAVITY_MODIFIER,
            new NumberModifierData(level, Operation::MULTIPLY, 2, 200)));
    });
extern GlobalEffect* SLIPPERY_EFFECT =
    new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
      for (auto target : targets)
        target->addModifierInstance(new ModifierInstance(
            FRICTION_MODIFIER,
            new NumberModifierData(level, Operation::MULTIPLY, 0.01, 400)));
    });

int swaptable2[1][2] = {{1, 0}};
int swaptable3[2][3] = {{1, 2, 0}, {2, 0, 1}};
int swaptable4[9][4] = {{1, 0, 3, 2}, {1, 2, 3, 0}, {1, 3, 0, 2},
                        {2, 0, 3, 1}, {2, 3, 0, 1}, {2, 3, 1, 0},
                        {3, 0, 1, 2}, {3, 2, 0, 1}, {3, 2, 1, 0}};
void swap_positions(std::vector<PObject*> targets, int* inds, int len) {
  vec3f poss[4] = {vec3f(0, 0, 0)};
  for (int i = 0; i < len; i++) poss[i] = targets[i]->getPos();
  for (int i = 0; i < len; i++) targets[i]->setPos(poss[inds[i]]);
}
GlobalEffect* SWAP_POSITIONS_EFFECT =
    new StaticGlobalEffect([](Level* level, std::vector<PObject*> targets) {
      std::mt19937 rng(
          std::chrono::system_clock::now().time_since_epoch().count());
      std::uniform_real_distribution<double> dist(0.0, 1.0);
      switch (targets.size()) {
        case 2:
          swap_positions(targets, swaptable2[0], 2);
          break;
        case 3:
          swap_positions(targets, swaptable3[static_cast<int>(dist(rng) * 2)],
                         3);
          break;
        case 4:
          swap_positions(targets, swaptable4[static_cast<int>(dist(rng) * 9)],
                         4);
          break;
        default:
          break;
      }
    });

extern GlobalEffect* SPEEDBOOST_SELF_EFFECT = new ContextAwareGlobalEffect(
    SPEEDBOOST_EFFECT, ContextAwareGlobalEffect::Targets::SELF);
extern GlobalEffect* REPELL_TAGGER_FROM_SELF =
    new AttractOtherToTaggersEffect();
extern GlobalEffect* ATTRACT_OTHER_TO_TAGGERS_EFFECT =
    new RepellTaggersFromSelfEffect();
extern GlobalEffect* FREEZE_OTHER_EFFECT = new ContextAwareGlobalEffect(
    FREEZE_EFFECT, ContextAwareGlobalEffect::Targets::OTHER);
extern GlobalEffect* SLOWDOWN_OTHER_EFFECT = new ContextAwareGlobalEffect(
    SLOWDOWN_EFFECT, ContextAwareGlobalEffect::Targets::OTHER);
extern GlobalEffect* REVERSE_OTHER_EFFECT = new ContextAwareGlobalEffect(
    REVERSE_EFFECT, ContextAwareGlobalEffect::Targets::OTHER);
extern GlobalEffect* SLIPPERY_OTHER_EFFECT = new ContextAwareGlobalEffect(
    SLIPPERY_EFFECT, ContextAwareGlobalEffect::Targets::OTHER);

extern GlobalEffect* SPEEDBOOST_SELF_TAG_STATUS_EFFECT =
    new ContextAwareGlobalEffect(
        SPEEDBOOST_EFFECT, ContextAwareGlobalEffect::Targets::SELF_TAG_STATUS);
extern GlobalEffect* FREEZE_NOT_SELF_TAG_STATUS_EFFECT =
    new ContextAwareGlobalEffect(
        FREEZE_EFFECT, ContextAwareGlobalEffect::Targets::NOT_SELF_TAG_STATUS);
extern GlobalEffect* SLOWDOWN_NOT_SELF_TAG_STATUS_EFFECT =
    new ContextAwareGlobalEffect(
        SLOWDOWN_EFFECT,
        ContextAwareGlobalEffect::Targets::NOT_SELF_TAG_STATUS);
extern GlobalEffect* REVERSE_NOT_SELF_TAG_STATUS_EFFECT =
    new ContextAwareGlobalEffect(
        REVERSE_EFFECT, ContextAwareGlobalEffect::Targets::NOT_SELF_TAG_STATUS);

extern GlobalEffect* LAUNCH_OTHER_EFFECT = new ContextAwareGlobalEffect(
    LAUNCH_EFFECT, ContextAwareGlobalEffect::Targets::OTHER);
extern GlobalEffect* LAUNCH_NOT_SELF_TAG_STATUS_EFFECT =
    new ContextAwareGlobalEffect(
        LAUNCH_EFFECT, ContextAwareGlobalEffect::Targets::NOT_SELF_TAG_STATUS);
extern GlobalEffect* SLOW_FALL_OTHER_EFFECT = new ContextAwareGlobalEffect(
    SLOW_FALL_EFFECT, ContextAwareGlobalEffect::Targets::OTHER);
extern GlobalEffect* FAST_FALL_OTHER_EFFECT = new ContextAwareGlobalEffect(
    FAST_FALL_EFFECT, ContextAwareGlobalEffect::Targets::OTHER);
extern GlobalEffect* SWAP_POSITIONS_OTHER_EFFECT = new ContextAwareGlobalEffect(
    SWAP_POSITIONS_EFFECT, ContextAwareGlobalEffect::Targets::OTHER);
extern GlobalEffect* SWAP_POSITIONS_ALL_EFFECT = new ContextAwareGlobalEffect(
    SWAP_POSITIONS_EFFECT, ContextAwareGlobalEffect::Targets::ALL);
