#include "core/game/packet/CAddPObjectPacket.h"

#include "core/util/global.h"

CAddPObjectPacket::CAddPObjectPacket() : CAddPObjectPacket(nullptr) {}
CAddPObjectPacket::CAddPObjectPacket(PObject* obj) : Packet(CLIENT_ADD_POBJECT_PACKET_ID) {
	if (obj != nullptr)
	{
		ByteBufferBuilder builder;
		obj->pack(builder);
		buf = builder.build();
	}
}
void CAddPObjectPacket::pack(ByteBufferBuilder& builder)
{
	Packet::pack(builder);
	builder.writeULL(buf.getLength());
	builder.writeBuffer(buf);
}
void CAddPObjectPacket::unpack(ByteBuffer buf)
{
	Packet::unpack(buf);
	size_t bufLen = buf.nextULL();
	buf = buf.nextBuffer(bufLen);
}
void CAddPObjectPacket::handle(Packet packet)
{
	CAddPObjectPacket* cPacket = dynamic_cast<CAddPObjectPacket*>(&packet);
	level->addPObject(PObject::fromBytes(cPacket->buf));
}
ByteBuffer CAddPObjectPacket::toBytes(Packet packet)
{
	ByteBufferBuilder builder;
	packet.pack(builder);
	return builder.build();
}
Packet CAddPObjectPacket::fromBytes(ByteBuffer buf)
{
	CAddPObjectPacket packet;
	packet.unpack(buf);
	return packet;
}