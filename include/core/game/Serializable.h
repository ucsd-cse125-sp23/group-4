#pragma once

#include "core/util/ByteBuffer.h"

class Serializable {
	virtual void pack(ByteBufferBuilder& builder) = 0;
	ByteBuffer pack() {
		ByteBufferBuilder builder;
		this->pack(builder);
		return builder.build();
	}
	virtual void unpack(ByteBuffer buf) = 0;
};