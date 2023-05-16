#include "core/game/effect/Effect.h"

#include "core/util/global.h"


Effect:Effect(U u) : applyTo(u) {}
void Effect::apply(Level* level, PObject* target)
{
	applyTo(level, { target });
}
void Effect::apply(Level* level, std::vector<PObject*> targets)
{
	applyTo(level, targets);
}

Effect* SPEEDBOOST_EFFECT = new Effect([](Level* level, std::vector<PObject*> targets) {
	for (auto target : targets)
		target->addModifierInstance(new ModifierInstance(SPEEDBOOST_MODIFIER, new SpeedBoostModifierData(100, 0.2f)));
});