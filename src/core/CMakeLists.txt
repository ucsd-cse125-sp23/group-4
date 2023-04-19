project(core DESCRIPTION "tagguys core library, includes: game logic, internal data structures, etc")

message(${PROJECT_SOURCE_DIR})

set( HEADERS
	${tagguys_SOURCE_DIR}/include/core/math/vector.h
	${tagguys_SOURCE_DIR}/include/core/math/matrix.h

	${tagguys_SOURCE_DIR}/include/core/math/shape/Simplex.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/BoundingShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/ConvexShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/CylinderShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/SphereShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/AABShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/ConvexMeshShape.h
	${tagguys_SOURCE_DIR}/include/core/math/shape/ConcaveMeshShape.h

	${tagguys_SOURCE_DIR}/include/core/Player.h
)

set( SOURCES
	${PROJECT_SOURCE_DIR}/math/ConvexShape.cpp

	${PROJECT_SOURCE_DIR}/Player.cpp
)

include_directories(
	${tagguys_SOURCE_DIR}/include
)

add_executable(core_debug ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES} ${HEADERS})
add_library(core ${PROJECT_SOURCE_DIR}/lib.cpp ${SOURCES} ${HEADERS})
target_include_directories(core PUBLIC ${tagguys_SOURCE_DIR}/include)
