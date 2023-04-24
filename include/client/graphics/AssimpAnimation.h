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
    std::vector<std::pair<double,glm::vec4>> rotations;
    std::vector<std::pair<double,glm::vec3>> scalings;

    void eval(double currentTick);
    glm::mat4 getMatrixInterpolate(int lastEarlyTickInd, double tick);
    static glm::mat4 getMatrix(glm::vec3& pos, glm::vec4& rot, glm::vec3& sca);
    static ASSIMP_EXTRAP_MODE loadExtrapMode(const aiAnimBehaviour& b);
    static std::string extraModeToString(ASSIMP_EXTRAP_MODE mode);
};

class AssimpAnimation {
public:
    AssimpAnimation();

    double duration;
    double tps;
    std::vector<AssimpChannel> channels;

    void update(double deltaTimeInMs);
    void restart();
    void imGui();
private:
    double currentTimeInMs;
};
