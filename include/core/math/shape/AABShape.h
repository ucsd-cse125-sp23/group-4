#pragma once

#include "core/math/shape/ConvexShape.h"

class AABShape : public ConvexShape {
private:
    vec3f minP, maxP;
protected:
    vec4f furthestPoint(vec3f dir) const override {
        return vec4f(
            dir.x > 0 ? maxP.x : minP.x,
            dir.y > 0 ? maxP.y : minP.y,
            dir.z > 0 ? maxP.z : minP.z,
            1.0f
        );
    };
 public:
     AABShape(vec3f min, vec3f max) : minP(min), maxP(max) {}
};
