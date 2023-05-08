#pragma once

#include "core/game/packet/Packet.h"
#include "core/game/physics/PObject.h"

class CUpdatePObjectPacket : public Packet {
private:
	ByteBuffer buf;
public:
	CUpdatePObjectPacket();
	CUpdatePObjectPacket(PObject* obj);
	virtual void pack(ByteBufferBuilder& builder);
	virtual void unpack(ByteBuffer buf);

	static void handle(Packet);
	static ByteBuffer toBytes(Packet);
	static Packet fromBytes(ByteBuffer);
};