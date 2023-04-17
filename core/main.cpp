#include <iostream>
#include "Player.h"

#include "math/shape/BoundingShape.h"
#include "math/shape/ConvexShape.h"
#include "math/shape/SphereShape.h"
#include "math/shape/CylinderShape.h"

int main(int argc, char* argv[])
{
	std::cout << "Hello world!" << std::endl;
	vec4f a = vec4f(1, 2, 3, 4);
	vec3f b = vec3f(a);
	std::cout << b << std::endl;
	CylinderShape* shape0 = new CylinderShape(2, 1);

	mat4f m = translate(b);
	m = rotate(b);
	m = scale(b);

	std::cout << (shape0->collides(shape0) ? "hits" : "misses") << std::endl;
	std::cout << (shape0->collides(shape0, translate(vec3f(0.5f, 0.0f, 0.0f))) ? "hits" : "misses") << std::endl;
	std::cout << (shape0->collides(shape0, translate(vec3f(1.4f, 0.0f, 1.4f))) ? "hits" : "misses") << std::endl;
	std::cout << (shape0->collides(shape0, translate(vec3f(1.4f, 0.5f, 1.4f))) ? "hits" : "misses") << std::endl;
	std::cout << (shape0->collides(shape0, translate(vec3f(1.5f, 0.0f, 1.5f))) ? "hits" : "misses") << std::endl;


	std::cout << shape0->mtv(shape0, translate(vec3f(1.5f, 0.0f, 0.0f))) << std::endl;
	std::cout << shape0->mtv(shape0, translate(vec3f(1.4f, 0.0f, 1.4f))) << std::endl;
	

	delete shape0;
}