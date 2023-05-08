#include "core/game/packet/CRemovePObjectPacket.h"

#include "core/util/global.h"

CRemovePObjectPacket::CRemovePObjectPacket() : CRemovePObjectPacket(0) {}
CRemovePObjectPacket::CRemovePObjectPacket(size_t id) : Packet(CLIENT_REMOVE_POBJECT_PACKET_ID), id(id) {}
void CRemovePObjectPacket::pack(ByteBufferBuilder& builder)
{
	Packet::pack(builder);
	builder.writeULL(id);
}
void CRemovePObjectPacket::unpack(ByteBuffer buf)
{
	Packet::unpack(buf);
	this->id = buf.nextULL();
}
void CRemovePObjectPacket::handle(Packet packet)
{
	level->removePObject(dynamic_cast<CRemovePObjectPacket*>(&packet)->id);
}
ByteBuffer CRemovePObjectPacket::toBytes(Packet packet)
{
	ByteBufferBuilder builder;
	packet.pack(builder);
	return builder.build();
}
Packet CRemovePObjectPacket::fromBytes(ByteBuffer buf)
{
	CRemovePObjectPacket packet;
	packet.unpack(buf);
	return packet;
}