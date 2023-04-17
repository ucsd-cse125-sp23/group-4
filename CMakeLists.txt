cmake_minimum_required(VERSION 3.16)

project(core)

set( HEADERS
	core/include/math/vector.h
	core/include/math/matrix.h

	core/include/math/shape/Simplex.h
	core/include/math/shape/BoundingShape.h
	core/include/math/shape/ConvexShape.h
	core/include/math/shape/CylinderShape.h
	core/include/math/shape/SphereShape.h
	core/include/math/shape/AABShape.h
	core/include/math/shape/ConvexMeshShape.h
	core/include/math/shape/ConcaveMeshShape.h

	core/include/Player.h
)

set( SOURCES
	core/src/math/ConvexShape.cpp

	core/src/Player.cpp
)

include_directories(
    core/include
)

add_library(core_lib ${SOURCES} ${HEADERS})
add_executable(debug core/main.cpp)
target_link_libraries(debug PRIVATE core_lib)