cmake_minimum_required(VERSION 3.16)

project(core)

set( HEADERS
	include/core/math/vector.h
	include/core/math/matrix.h

	include/core/math/shape/Simplex.h
	include/core/math/shape/BoundingShape.h
	include/core/math/shape/ConvexShape.h
	include/core/math/shape/CylinderShape.h
	include/core/math/shape/SphereShape.h
	include/core/math/shape/AABShape.h
	include/core/math/shape/ConvexMeshShape.h
	include/core/math/shape/ConcaveMeshShape.h

	include/core/Player.h
)

set( SOURCES
	src/core/math/ConvexShape.cpp

	src/core/Player.cpp
)

include_directories(
	include
)

add_library(core_lib ${SOURCES} ${HEADERS})
add_executable(debug src/core/main.cpp)
target_link_libraries(debug PRIVATE core_lib)