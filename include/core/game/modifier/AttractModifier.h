#pragma once

#include "core/game/modifier/TimedModifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

struct AttractModifierData : TimedModifierData {
    PObject* sink;
    float factor;
    AttractModifierData(unsigned long long expire, PObject* sink, float factor)
        : TimedModifierData(expire), sink(sink), factor(factor) {}
};
class AttractModifier : public TimedModifier {
public:
    AttractModifier();
    void timedModify(Modifiable* obj, ModifierData* data) override;
};
