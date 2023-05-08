#pragma once

#include "core/game/packet/Packet.h"
#include "core/game/physics/PObject.h"

class CAddPObjectPacket : public Packet {
private:
	ByteBuffer buf;
public:
	CAddPObjectPacket();
	CAddPObjectPacket(PObject* obj);
	virtual void pack(ByteBufferBuilder& builder);
	virtual void unpack(ByteBuffer buf);

	static void handle(Packet);
	static ByteBuffer toBytes(Packet);
	static Packet fromBytes(ByteBuffer);
};