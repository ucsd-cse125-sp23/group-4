#pragma once

#include <string>
#include <vector>
#include "core/math/vector.h"

struct ByteBufferBuilder {
private:
	std::vector<uint8_t> data;

	uint8_t buf[8] = { 0 };
public:
	ByteBuffer build();
	void clear();

	void writeChar(char c);
	void writeInt(int i);
	void writeUInt(uint32_t u);
	void writeULL(unsigned long long ull);
	void writeFloat(float f);
	void writeDouble(double d);
	void writeVec3f(vec3f v);
	void writeString(std::string s);
	void writeBuffer(ByteBuffer buf);
};
struct ByteBuffer {
private:
	const size_t size;
	size_t index;
	uint8_t* data;
public:
	ByteBuffer(std::vector<uint8_t> data);
	ByteBuffer(const uint8_t* data, size_t size);
	~ByteBuffer();
	size_t getLength();
	const uint8_t* getBytes();
	void reset();

	char nextChar();
	int nextInt();
	uint32_t nextUInt();
	unsigned long long nextULL();
	float nextFloat();
	double nextDouble();
	vec3f nextVec3f();
	std::string nextString(size_t length);
	ByteBuffer nextBuffer(size_t length);
};