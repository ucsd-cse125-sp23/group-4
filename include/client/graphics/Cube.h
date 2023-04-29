#ifndef _CUBE_H_
#define _CUBE_H_

#include "client/graphics/core.h"
#include "client/graphics/Mesh.h"

////////////////////////////////////////////////////////////////////////////////

class Cube : public Mesh
{
public:
	Cube(glm::vec3 cubeMin=glm::vec3(-1,-1,-1), glm::vec3 cubeMax=glm::vec3(1, 1, 1));
	~Cube() {
		cleargl();
	}
};

////////////////////////////////////////////////////////////////////////////////

#endif
