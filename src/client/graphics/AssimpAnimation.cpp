#include "AssimpAnimation.h"

void AssimpChannel::eval(double currentTick) {
    node->animationTransform = node->localTransform;
}

ASSIMP_EXTRAP_MODE AssimpChannel::loadExtrapMode(const aiAnimBehaviour& b) {
    return 
        b & aiAnimBehaviour::aiAnimBehaviour_CONSTANT ?
        ASSIMP_EXTRAP_MODE::CONSTANT :
        b & aiAnimBehaviour::aiAnimBehaviour_LINEAR ?
        ASSIMP_EXTRAP_MODE::LINEAR :
        b & aiAnimBehaviour::aiAnimBehaviour_REPEAT ?
        ASSIMP_EXTRAP_MODE::CYCLE :
        ASSIMP_EXTRAP_MODE::DEFAULT;
}

AssimpAnimation::AssimpAnimation() 
        : duration(0), tps(0), currentTimeInMs(0) { }

void AssimpAnimation::restart() {
    currentTimeInMs = 0;
}

void AssimpAnimation::update(double deltaTimeInMs) {
    if(tps == 0) {
        return;
    }

    currentTimeInMs += deltaTimeInMs;
    double currentTick = currentTimeInMs/tps;
    for(int i = 0; i < channels.size(); i++) {
        channels[i].eval(currentTick);
    }
}
