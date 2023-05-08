#pragma once

#include "core/game/Serializable.h"

class Packet : public Serializable {
private:
	int nid;
public:
	Packet(int nid) : nid(nid) {}
	int getNId() {
		return nid;
	}
	virtual void pack(ByteBufferBuilder& builder) {
		builder.writeInt(this->getNId());
	}
	virtual void unpack(ByteBuffer buf) {
		this->nid = buf.nextInt();
	}
};