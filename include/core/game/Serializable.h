#pragma once

#include "core/util/ByteBuffer.h"

class Serializable {
	virtual void pack(ByteBufferBuilder& builder) = 0;
	virtual void unpack(ByteBuffer buf) = 0;
};