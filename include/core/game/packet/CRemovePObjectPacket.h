#pragma once

#include "core/game/packet/Packet.h"

class CRemovePObjectPacket : public Packet {
private:
	size_t id;
public:
	CRemovePObjectPacket();
	CRemovePObjectPacket(size_t id);
	virtual void pack(ByteBufferBuilder& builder);
	virtual void unpack(ByteBuffer buf);

	static void handle(Packet);
	static ByteBuffer toBytes(Packet);
	static Packet fromBytes(ByteBuffer);
};