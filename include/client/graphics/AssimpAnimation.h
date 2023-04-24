#pragma once

#include "core.h"
#include "AssimpNode.h"

#include <vector>
#include <string>
#include <map>
#include <utility>      // pair

enum ASSIMP_EXTRAP_MODE {
    DEFAULT, CONSTANT, LINEAR, CYCLE
};

struct AssimpChannel {
    std::string name;
    AssimpNode* node;

    ASSIMP_EXTRAP_MODE extrapPre, extrapPost;
    std::vector<std::pair<double,glm::vec3>> positions;
    std::vector<std::pair<double,aiQuaternion>> rotations;
    std::vector<std::pair<double,glm::vec3>> scalings;

    void eval(double currentTick);
    static ASSIMP_EXTRAP_MODE loadExtrapMode(const aiAnimBehaviour& b);
};

class AssimpAnimation {
public:
    AssimpAnimation();

    double duration;
    double tps;
    std::vector<AssimpChannel> channels;

    void update(double deltaTimeInMs);
    void restart();
private:
    double currentTimeInMs;
};
