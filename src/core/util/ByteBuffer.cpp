#include "core/util/ByteBuffer.h"

#include <cstring>

ByteBuffer ByteBufferBuilder::build() { return ByteBuffer(data); }
void ByteBufferBuilder::clear() { data.clear(); }

void ByteBufferBuilder::writeChar(char c)
{
	memcpy(buf, &c, 1);
	data.push_back(buf[0]);
}
void ByteBufferBuilder::writeInt(int i)
{
	memcpy(buf, &i, 4);
	data.push_back(buf[0]);
	data.push_back(buf[1]);
	data.push_back(buf[2]);
	data.push_back(buf[3]);
}
void ByteBufferBuilder::writeUInt(uint32_t u)
{
	memcpy(buf, &u, 4);
	data.push_back(buf[0]);
	data.push_back(buf[1]);
	data.push_back(buf[2]);
	data.push_back(buf[3]);
}
void ByteBufferBuilder::writeUnsignedLongLong(unsigned long long ull)
{
	memcpy(buf, &ull, 8);
	data.push_back(buf[0]);
	data.push_back(buf[1]);
	data.push_back(buf[2]);
	data.push_back(buf[3]);
	data.push_back(buf[4]);
	data.push_back(buf[5]);
	data.push_back(buf[6]);
	data.push_back(buf[7]);
}
void ByteBufferBuilder::writeFloat(float f)
{
	memcpy(buf, &f, 4);
	data.push_back(buf[0]);
	data.push_back(buf[1]);
	data.push_back(buf[2]);
	data.push_back(buf[3]);
}
void ByteBufferBuilder::writeDouble(double d)
{
	memcpy(buf, &d, 8);
	data.push_back(buf[0]);
	data.push_back(buf[1]);
	data.push_back(buf[2]);
	data.push_back(buf[3]);
	data.push_back(buf[4]);
	data.push_back(buf[5]);
	data.push_back(buf[6]);
	data.push_back(buf[7]);
}
void ByteBufferBuilder::writeVec3f(vec3f v)
{
	writeFloat(v.x);
	writeFloat(v.y);
	writeFloat(v.z);
}
void ByteBufferBuilder::writeString(std::string s)
{
	for (char c : s)
		writeChar(c);
}
void ByteBufferBuilder::writeBuffer(ByteBuffer buf)
{
	const uint8_t* data = buf.getBytes();
	for (size_t i = 0; i < buf.getLength(); i++)
		this->data.push_back(data[i]);
}


ByteBuffer::ByteBuffer(std::vector<uint8_t> data) : size(data.size()), index(0)
{
	this->data = (uint8_t*)malloc(sizeof(char) * data.size());
	memcpy(this->data, &data, size);
}
ByteBuffer::ByteBuffer(const uint8_t* data, size_t size) : size(size)
{
	this->data = (uint8_t*)malloc(sizeof(char) * size);
	memcpy(this->data, data, size);
}
ByteBuffer::~ByteBuffer() { free(data); }
size_t ByteBuffer::getLength() { return size; }
const uint8_t* ByteBuffer::getBytes() { return data; }
void ByteBuffer::reset() { index = 0; }

char ByteBuffer::nextChar()
{
	char c; memcpy(&c, this->data + index, 1); index += 1;
	return c;
}
int ByteBuffer::nextInt()
{
	int i; memcpy(&i, this->data + index, 4); index += 4;
	return i;
}
uint32_t ByteBuffer::nextUInt()
{
	uint32_t u; memcpy(&u, this->data + index, 4); index += 4;
	return u;
}
unsigned long long ByteBuffer::nextUnsignedLongLong()
{
	int ull; memcpy(&ull, this->data + index, 8); index += 8;
	return ull;
}
float ByteBuffer::nextFloat()
{
	float f; memcpy(&f, this->data + index, 4); index += 4;
	return f;
}
double ByteBuffer::nextDouble()
{
	double d; memcpy(&d, this->data + index, 8); index += 8;
	return d;
}
vec3f ByteBuffer::nextVec3f()
{
	return vec3f(nextFloat(), nextFloat(), nextFloat());
}
std::string ByteBuffer::nextString(size_t length)
{
	std::string str = std::string((char*)(this->data + index), length);
	index += length;
	return str;
}

ByteBuffer ByteBuffer::nextBuffer(size_t length)
{
	ByteBuffer buf = ByteBuffer(this->data + index, length);
	index += length;
	return buf;
}