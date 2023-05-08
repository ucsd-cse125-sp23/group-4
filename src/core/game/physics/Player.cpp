#include "core/game/physics/Player.h"
#include "core/util/global.h"

uint32_t Player::maxId = 1;

Player::Player() : PObject(PLAYER_TYPE, PLAYER_BOUNDING_SHAPE, PLAYER_LAYER, 0, false), look(vec3f(1.0f, 0.0f, 0.0f))
{
	if (isServer())
		this->pid = Player::maxId++;
	this->addModifierInstance(new ModifierInstance(GRAVITY_MODIFIER, new GravityModifierData(0.01f)));
}

Player::~Player()
{
}

void Player::tick()
{
	PObject::tick();
}


void Player::pack(ByteBufferBuilder& builder)
{
	PObject::pack(builder);
	builder.writeUInt(this->pid);
	builder.writeVec3f(this->look);
}
void Player::unpack(ByteBuffer buf)
{
	PObject::unpack(buf);
	size_t bufId = buf.nextUInt();
	if (this->pid == 0 || this->pid == bufId)
	{
		this->pid = bufId;
		this->look = buf.nextVec3f();
	}
}