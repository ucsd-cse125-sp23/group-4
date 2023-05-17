#include "AssimpAnimation.h"

#include "AssimpMath.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>

void AssimpChannel::eval(double currentTick) {
    // IMPORTANT: assuming that pos,rot,sca has the same ticks
    double start = positions[0].first;
    double end   = positions[positions.size()-1].first;
    // before start or after end, extrapolate
    if(currentTick < start || currentTick > end) {
        bool isBefore = currentTick < start;
        switch(isBefore ? extrapPre : extrapPost) {
        case ASSIMP_EXTRAP_MODE::CONSTANT:
            node->animationTransform = 
                isBefore ? 
                getMatrix(
                    positions[0].second,
                    rotations[0].second,
                    scalings [0].second) :
                getMatrix(
                    positions[positions.size()-1].second,
                    rotations[positions.size()-1].second,
                    scalings [positions.size()-1].second);
            break;
        case ASSIMP_EXTRAP_MODE::LINEAR:
            // TODO: implement linear extrapolation
            printf("LINEAR NOT SUPPORTED\n");
            break;
        case ASSIMP_EXTRAP_MODE::CYCLE:
            // TODO: not tested
            eval(std::fmod((currentTick-start),glm::abs(end-start)));
            break;
        default:
            // TODO: bug - this mode does not work
            break;
        }
        return;
    }

    int lastEarlyTickInd = -1;
    for(int i = 0; i < positions.size(); i++) {
        if(currentTick == positions[i].first) {
            node->animationTransform = getMatrix(
                positions[i].second,
                rotations[i].second,
                scalings [i].second);
            return;
        }
        if(currentTick > positions[i].first) {
            lastEarlyTickInd = i;
        }
    }

    node->animationTransform = getMatrixInterpolate(lastEarlyTickInd,
                                                    currentTick);
}

glm::mat4 AssimpChannel::getMatrixInterpolate(int lastEarlyTickInd, 
                                              double tick) {
    // linear interpolation
    double t0 = positions[lastEarlyTickInd].first;
    double t1 = positions[lastEarlyTickInd+1].first;
    glm::vec3 interpPos =
         positions[lastEarlyTickInd  ].second  +
        (positions[lastEarlyTickInd+1].second  -
         positions[lastEarlyTickInd  ].second) *
        (float)((tick - t0) / (t1 - t0));
    
    // spherical interpolation
    glm::vec4 interpRot = quarternionInterpolateSpherical(
        rotations[lastEarlyTickInd  ].second,
        rotations[lastEarlyTickInd+1].second,
        (float)((tick - t0) / (t1 - t0)));

    // linear interpolation
    glm::vec3 interpSca =
         scalings [lastEarlyTickInd  ].second  +
        (scalings [lastEarlyTickInd+1].second  -
         scalings [lastEarlyTickInd  ].second) * 
        (float)((tick - t0) / ( t1 - t0 ));
    return getMatrix(interpPos, interpRot, interpSca);
}

// TODO: we can have unit test for this function
glm::mat4 AssimpChannel::getMatrix(glm::vec3& pos, glm::vec4& rot, glm::vec3& sca) {
    glm::mat4 result(1.0f);
    result = glm::translate(result, pos);
    result = result * quarternionToRotationMat4x4(rot);
    result = glm::scale(result, sca);
    return result;
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

std::string AssimpChannel::extraModeToString(ASSIMP_EXTRAP_MODE mode) {
    switch (mode) {
    case ASSIMP_EXTRAP_MODE::DEFAULT:
        return "Default";
    case ASSIMP_EXTRAP_MODE::CONSTANT:
        return "Constant";
    case ASSIMP_EXTRAP_MODE::LINEAR:
        return "Linear";
    case ASSIMP_EXTRAP_MODE::CYCLE:
        return "Cycle";
    default:
        return "Unknown";
    }
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
    double currentTick = currentTimeInMs * tps;
    for(int i = 0; i < channels.size(); i++) {
        channels[i].eval(currentTick);
    }
}

void AssimpAnimation::imGui() {
    ImGui::Text("name    : %s", name.c_str());
    ImGui::Text("duration: %f ticks; %f secs", duration, duration/tps);
    ImGui::Text("tps     : %f t/s", tps);
    if (ImGui::TreeNode((void*)(intptr_t)0, "Channels (%lu)", channels.size())) {
        for(int i = 0; i < channels.size(); i++) {
            if (ImGui::TreeNode((void*)(intptr_t)i, "Channel %d", i)) {
                AssimpChannel& channel = channels[i];
                ImGui::Text("name: %s %s", channel.name.c_str(), channel.node->name.c_str());
                ImGui::Text("extrapIn : %s", AssimpChannel::extraModeToString(channel.extrapPre).c_str());
                ImGui::Text("extrapOut: %s", AssimpChannel::extraModeToString(channel.extrapPost).c_str());
                ImGui::Text(
                    "Pos(%lu) Rot(%lu) Sca(%lu)",
                    channel.positions.size(),
                    channel.rotations.size(),
                    channel.scalings .size());
                ImGui::Text(
                    "Start (%f,%f,%f)",
                    channel.positions[0].first,
                    channel.rotations[0].first,
                    channel.scalings [0].first);
                ImGui::Text(
                    "End   (%f,%f,%f)",
                    channel.positions[channel.positions.size()-1].first,
                    channel.rotations[channel.rotations.size()-1].first,
                    channel.scalings [channel.scalings .size()-1].first);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}
