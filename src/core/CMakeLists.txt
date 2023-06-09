project(
  core
  DESCRIPTION
    "tagguys core library, includes: game logic, internal data structures, etc")

set(_CORE_SOURCES
    ${PROJECT_SOURCE_DIR}/game/effect/ContextAwareGlobalEffect.cpp
    ${PROJECT_SOURCE_DIR}/game/effect/GlobalEffect.cpp
    ${PROJECT_SOURCE_DIR}/game/effect/StaticGlobalEffect.cpp

    ${PROJECT_SOURCE_DIR}/game/event/EventManager.cpp

    ${PROJECT_SOURCE_DIR}/game/level/Environment.cpp
    ${PROJECT_SOURCE_DIR}/game/level/Level.cpp
    ${PROJECT_SOURCE_DIR}/game/level/PObjectCollection.cpp
    ${PROJECT_SOURCE_DIR}/game/level/StatisticManager.cpp

    ${PROJECT_SOURCE_DIR}/game/mode/GameMode.cpp

    ${PROJECT_SOURCE_DIR}/game/modifier/Modifiers.cpp

    ${PROJECT_SOURCE_DIR}/game/physics/CollisionBounds.cpp
    ${PROJECT_SOURCE_DIR}/game/physics/Player.cpp
    ${PROJECT_SOURCE_DIR}/game/physics/PObject.cpp
    ${PROJECT_SOURCE_DIR}/game/physics/PowerUp.cpp
    ${PROJECT_SOURCE_DIR}/game/physics/Mushroom.cpp
    ${PROJECT_SOURCE_DIR}/game/physics/River.cpp

    ${PROJECT_SOURCE_DIR}/math/ConvexShape.cpp

    ${PROJECT_SOURCE_DIR}/util/global.cpp

    ${PROJECT_SOURCE_DIR}/lib.cpp
)

add_library(core ${_CORE_SOURCES})
target_include_directories(core PUBLIC ${tagguys_SOURCE_DIR}/include)
