#include <iostream>
#include "core/Player.h"

#include "core/math/shape/BoundingShape.h"
#include "core/math/shape/ConvexShape.h"
#include "core/math/shape/SphereShape.h"
#include "core/math/shape/CylinderShape.h"
#include "core/math/shape/AABShape.h"

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
	/*
	std::cout << (shape0->collides(shape0) ? "hits" : "misses") << std::endl;
	std::cout << (shape0->collides(shape0, translate(vec3f(0.5f, 0.0f, 0.0f))) ? "hits" : "misses") << std::endl;
	std::cout << (shape0->collides(shape0, translate(vec3f(1.4f, 0.0f, 1.4f))) ? "hits" : "misses") << std::endl;
	std::cout << (shape0->collides(shape0, translate(vec3f(0.0f, 0.9f, 0.0f))) ? "hits" : "misses") << std::endl;
	std::cout << (shape0->collides(shape0, translate(vec3f(0.0f, 1.9f, 0.0f))) ? "hits" : "misses") << std::endl;
	std::cout << (shape0->collides(shape0, translate(vec3f(1.5f, 0.0f, 1.5f))) ? "hits" : "misses") << std::endl;
	*/
	vec4f v;
	//std::cout << (v=shape0->mtv(shape0, translate(vec3f(1.9f, 0.0f, 0.0f)))) << std::endl;
	//std::cout << (shape0->collides(shape0, translate(vec3f(1.9f, 0.0f, 0.0f) - vec3f(v) * (v.w + 0.01f))) ? "hits" : "misses") << std::endl;
	//std::cout << shape0->mtv(shape0, translate(vec3f(1.4f, 0.0f, 1.4f))) << std::endl;

	AABShape* aab = new AABShape(vec3f(-1, -1, -1), vec3f(1, 1, 1));
	
	std::cout << (v=aab->mtv(aab, translate(vec3f(1.4f, 0.1f, 0.0f)), scale(vec3f(0.5f, 1.0f, 0.5f)))) << std::endl;
	std::cout << (shape0->collides(shape0, translate(vec3f(1.4f, 0.0f, 0.0f) + vec3f(v)*(v.w + 0.001f)), scale(vec3f(0.5f, 1.0f, 0.5f))) ? "hits" : "misses") << std::endl;

	delete shape0;
	delete aab;
}