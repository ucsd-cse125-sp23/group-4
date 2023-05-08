#pragma once

#include "core/game/packet/Packet.h"
#include "core/util/ByteBuffer.h"
#include <map>
#include <set>
#include <functional>

enum NetworkDirection { CLIENT_BOUND, SERVER_BOUND };
struct PacketDescription {
	std::function<void(Packet)> handle;
	std::function<Packet(ByteBuffer)> fromBytes;
	std::function<ByteBuffer(Packet)> toBytes;
};
class PacketHandler {
private:
	std::set<int> packetIds;
	std::map<int, PacketDescription> clientBoundPacketHandlers;
	std::map<int, PacketDescription> serverBoundPacketHandlers;
public:
	void registerPacket(int id, NetworkDirection dir, PacketDescription desc);
	void acceptPacket(ByteBuffer buf, NetworkDirection dir);
	void sendPacketToServer(Packet packet);
	void sendPacketToClient(size_t pid, Packet packet);
	void sendPacketToAllClients(Packet packet);
};