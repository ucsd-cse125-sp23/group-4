#include "core/game/packet/CLevelStatusPacket.h"

#include "core/util/global.h"

CLevelStatusPacket::CLevelStatusPacket() : CLevelStatusPacket(0) {}
CLevelStatusPacket::CLevelStatusPacket(Level* level) : Packet(CLIENT_LEVEL_STATUS_PACKET_ID), age(level->getAge()) {}
void CLevelStatusPacket::pack(ByteBufferBuilder& builder)
{
	Packet::pack(builder);
	builder.writeULL(age);
}
void CLevelStatusPacket::unpack(ByteBuffer buf)
{
	Packet::unpack(buf);
	this->age = buf.nextULL();
}
void CLevelStatusPacket::handle(Packet packet)
{
	level->setAge(dynamic_cast<CLevelStatusPacket*>(&packet)->age);
}
ByteBuffer CLevelStatusPacket::toBytes(Packet packet)
{
	ByteBufferBuilder builder;
	packet.pack(builder);
	return builder.build();
}
Packet CLevelStatusPacket::fromBytes(ByteBuffer buf)
{
	CLevelStatusPacket packet;
	packet.unpack(buf);
	return packet;
}