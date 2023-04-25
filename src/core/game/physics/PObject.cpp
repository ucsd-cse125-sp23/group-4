#include "core/game/physics/PObject.h"


uint32_t PObject::maxId = 0;

PObject::PObject(BoundingShape* shape, unsigned int layer, float friction, bool static_)
	: bounds(new CollisionBounds(shape, layer, friction)), static_(static_), pos(vec3f(0.0f, 0.0f, 0.0f)), vel(vec3f(0.0f, 0.0f, 0.0f)),
	onGround(false) {
	this->id = PObject::maxId++;
}
PObject::~PObject() {
	delete bounds;
}
void PObject::tick() {
	Modifiable::tick();

	pos += vel;
	this->bounds->setPos(pos);
}
vec3f PObject::getPos() {
	return pos;
}
void PObject::setPos(vec3f pos) {
	this->pos = pos;
	this->bounds->setPos(pos);
}
void PObject::addPos(vec3f dPos) {
	this->setPos(pos + dPos);
}
bool PObject::markedRemove() {
	return toRemove;
}
void PObject::markRemove() {
	toRemove = true;
}

bool PObject::isStatic()
{
	return this->static_;
}
const CollisionBounds* PObject::getBounds()
{
	return this->bounds;
}