#pragma once

#include "core/game/effect/GlobalEffect.h"

struct ContextAwareGlobalEffect : public GlobalEffect {
public:
	enum class Targets {
		ALL, TAGGERS, NONTAGGERS, SELF, OTHER
	};

	ContextAwareGlobalEffect(GlobalEffect* effect, Targets target);
	void apply(Level* level, std::vector<PObject*> targets) override;
private:
	GlobalEffect* effect;
	Targets target;
};