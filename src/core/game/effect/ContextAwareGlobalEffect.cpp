#include "core/game/effect/ContextAwareGlobalEffect.h"


ContextAwareGlobalEffect::ContextAwareGlobalEffect(GlobalEffect* effect, Targets target) : effect(effect), target(target)
{}
void ContextAwareGlobalEffect::apply(Level* level, std::vector<PObject*> targets)
{
	switch (target)
	{
	case ContextAwareGlobalEffect::Targets::ALL:
		break;
	case ContextAwareGlobalEffect::Targets::TAGGERS:
		break;
	case ContextAwareGlobalEffect::Targets::NONTAGGERS:
		break;
	case ContextAwareGlobalEffect::Targets::SELF:
		break;
	case ContextAwareGlobalEffect::Targets::OTHER:
		break;
	}
}