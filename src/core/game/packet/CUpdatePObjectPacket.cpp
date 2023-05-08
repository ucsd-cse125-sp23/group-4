#include "core/game/packet/CUpdatePObjectPacket.h"

#include "core/util/global.h"

CUpdatePObjectPacket::CUpdatePObjectPacket() : CUpdatePObjectPacket(0) {}
CUpdatePObjectPacket::CUpdatePObjectPacket(PObject* obj) : Packet(CLIENT_UPDATE_POBJECT_PACKET_ID) {
	if (obj != nullptr)
	{
		ByteBufferBuilder builder;
		obj->pack(builder);
		buf = builder.build();
	}
}
void CUpdatePObjectPacket::pack(ByteBufferBuilder& builder)
{
	Packet::pack(builder);
	builder.writeULL(buf.getLength());
	builder.writeBuffer(buf);
}
void CUpdatePObjectPacket::unpack(ByteBuffer buf)
{
	Packet::unpack(buf);
	size_t bufLen = buf.nextULL();
	buf = buf.nextBuffer(bufLen);
}
void CUpdatePObjectPacket::handle(Packet packet)
{
	CUpdatePObjectPacket* cPacket = dynamic_cast<CUpdatePObjectPacket*>(&packet);
	size_t id = cPacket->buf.nextULL();
	cPacket->buf.reset();
	level->getPObject(id)->unpack(cPacket->buf);
}
ByteBuffer CUpdatePObjectPacket::toBytes(Packet packet)
{
	ByteBufferBuilder builder;
	packet.pack(builder);
	return builder.build();
}
Packet CUpdatePObjectPacket::fromBytes(ByteBuffer buf)
{
	CUpdatePObjectPacket packet;
	packet.unpack(buf);
	return packet;
}