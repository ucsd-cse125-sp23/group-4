#include "core/game/physics/PObject.h"

#include "core/util/global.h"


uint32_t PObject::maxId = 1;

PObject::PObject(/*PObjectType* type, */BoundingShape* shape, unsigned int layer, float friction, bool static_)
	: /*type(type),*/ bounds(new CollisionBounds(shape, layer, friction)), static_(static_), pos(vec3f(0.0f, 0.0f, 0.0f)), vel(vec3f(0.0f, 0.0f, 0.0f)),
	onGround(false) {
	if(isServer())
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


void PObject::pack(ByteBufferBuilder& builder)
{/*
	builder.writeULL(this->type->key.size());
	builder.writeString(this->type->key);
	builder.writeUInt(this->id);
	builder.writeVec3f(this->pos);
	builder.writeInt((toRemove ? 0b10 : 0) | (onGround ? 0b01 : 0));
	Modifiable::pack(builder);*/
}
void PObject::unpack(ByteBuffer buf)
{/*
	size_t keyLen = buf.nextULL();
	if (buf.nextString(keyLen).compare(this->type->key) == 0)
	{
		size_t bufId = buf.nextUInt();
		if (this->id == 0 || this->id == bufId)
		{
			this->id = bufId;
			this->setPos(buf.nextVec3f());
			int flags = buf.nextInt();
			toRemove = (flags & 0b10) != 0;
			onGround = (flags & 0b01) != 0;
			Modifiable::unpack(buf);
		}
	}*/
}