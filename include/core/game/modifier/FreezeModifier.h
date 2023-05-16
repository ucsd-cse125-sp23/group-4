#pragma once

#include "core/game/modifier/TimedModifier.h"
#include "core/game/physics/PObject.h"
#include "core/math/vector.h"

struct FreezeModifierData : TimedModifierData {
    FreezeModifierData(unsigned long long expire)
        : TimedModifierData(expire) {}
};
class FreezeModifier : public TimedModifier {
public:
    FreezeModifier();
    void timedModify(Modifiable* obj, ModifierData* data) override;
};
