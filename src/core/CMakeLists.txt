project(core DESCRIPTION "tagguys core library, includes: game logic, internal data structures, etc")

message(${PROJECT_SOURCE_DIR})

set( HEADERS
	${tagguys_SOURCE_DIR}/include/core/util/global.h
	${tagguys_SOURCE_DIR}/include/core/util/ByteBuffer.h
	${tagguys_SOURCE_DIR}/include/core/math/vector.h
	${tagguys_SOURCE_DIR}/include/core/math/matrix.h

	${tagguys_SOURCE_DIR}/include/core/math/shape/Simplex.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/BoundingShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/ConvexShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/CylinderShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/OffsetShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/SphereShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/AABShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/ConvexMeshShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/ConcaveMeshShape.h

	${tagguys_SOURCE_DIR}/include/core/game/Serializable.h
	${tagguys_SOURCE_DIR}/include/core/game/Registry.h

	${tagguys_SOURCE_DIR}/include/core/game/packet/Packet.h
	${tagguys_SOURCE_DIR}/include/core/game/packet/PacketHandler.h
	${tagguys_SOURCE_DIR}/include/core/game/packet/CRemovePObjectPacket.h

	${tagguys_SOURCE_DIR}/include/core/game/physics/CollisionBounds.h
	${tagguys_SOURCE_DIR}/include/core/game/physics/PObject.h
	${tagguys_SOURCE_DIR}/include/core/game/physics/PObjectType.h
	${tagguys_SOURCE_DIR}/include/core/game/physics/Player.h

	${tagguys_SOURCE_DIR}/include/core/game/level/Level.h
	${tagguys_SOURCE_DIR}/include/core/game/level/Environment.h
	${tagguys_SOURCE_DIR}/include/core/game/level/PObjectCollection.h
	
	${tagguys_SOURCE_DIR}/include/core/game/modifier/Modifiable.h
	${tagguys_SOURCE_DIR}/include/core/game/modifier/Modifier.h
	${tagguys_SOURCE_DIR}/include/core/game/modifier/ModifierInstance.h

	${tagguys_SOURCE_DIR}/include/core/game/modifier/TimedModifier.h
	${tagguys_SOURCE_DIR}/include/core/game/modifier/GravityModifier.h
	${tagguys_SOURCE_DIR}/include/core/game/modifier/ControlModifier.h
	${tagguys_SOURCE_DIR}/include/core/game/modifier/SpeedBoostModifier.h
)

set( SOURCES
	${PROJECT_SOURCE_DIR}/util/global.cpp
	${PROJECT_SOURCE_DIR}/util/ByteBuffer.cpp

	${PROJECT_SOURCE_DIR}/game/level/Level.cpp
	${PROJECT_SOURCE_DIR}/game/level/Environment.cpp
	${PROJECT_SOURCE_DIR}/game/level/PObjectCollection.cpp

	${PROJECT_SOURCE_DIR}/math/ConvexShape.cpp

	${PROJECT_SOURCE_DIR}/game/packet/PacketHandler.cpp
	${PROJECT_SOURCE_DIR}/game/packet/CRemovePObjectPacket.cpp
	
	${PROJECT_SOURCE_DIR}/game/physics/CollisionBounds.cpp
	${PROJECT_SOURCE_DIR}/game/physics/PObject.cpp
	${PROJECT_SOURCE_DIR}/game/physics/PObjectType.cpp
	${PROJECT_SOURCE_DIR}/game/physics/Player.cpp
	
	${PROJECT_SOURCE_DIR}/game/modifier/Modifiers.cpp
	${PROJECT_SOURCE_DIR}/game/modifier/Modifiable.cpp
)

include_directories(
	${tagguys_SOURCE_DIR}/include
)

add_executable(core_debug ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES} ${HEADERS})
target_compile_features(core_debug PRIVATE cxx_std_17)
add_library(core ${PROJECT_SOURCE_DIR}/lib.cpp ${SOURCES} ${HEADERS})
target_include_directories(core PUBLIC ${tagguys_SOURCE_DIR}/include)
target_compile_features(core PRIVATE cxx_std_17)