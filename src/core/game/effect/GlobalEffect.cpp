#include "core/game/effect/GlobalEffect.h"

#include "core/util/global.h"

GlobalEffect::GlobalEffect(std::function<void(Level*, std::vector<PObject*>)> f) : applyTo(f)
{}
void GlobalEffect::apply(Level* level, PObject* target)
{
	applyTo(level, { target });
}
void GlobalEffect::apply(Level* level, std::vector<PObject*> targets)
{
	applyTo(level, targets);
}

GlobalEffect* SPEEDBOOST_EFFECT = new GlobalEffect([](Level* level, std::vector<PObject*> targets) {
	for (auto target : targets)
		target->addModifierInstance(new ModifierInstance(SPEEDBOOST_MODIFIER, new SpeedBoostModifierData(100, 0.2f)));
});