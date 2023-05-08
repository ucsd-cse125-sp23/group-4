#pragma once

#include "core/game/packet/Packet.h"

class CLevelStatusPacket : public Packet {
private:
	size_t age;
public:
	CLevelStatusPacket();
	CLevelStatusPacket(Level* level);
	virtual void pack(ByteBufferBuilder& builder);
	virtual void unpack(ByteBuffer buf);

	static void handle(Packet);
	static ByteBuffer toBytes(Packet);
	static Packet fromBytes(ByteBuffer);
};