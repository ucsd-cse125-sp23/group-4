#include "core/game/packet/PacketHandler.h"
#include "core/util/global.h"


void PacketHandler::registerPacket(int id, NetworkDirection dir, PacketDescription desc) {
	if (packetIds.find(id) != packetIds.end())
		throw "Provided id already exists";
	packetIds.insert(id);
	switch (dir)
	{
	case CLIENT_BOUND:
		if(!isServer())
			clientBoundPacketHandlers[id] = desc;
		break;
	case SERVER_BOUND:
		if (isServer())
			serverBoundPacketHandlers[id] = desc;
		break;
	}
}

void PacketHandler::acceptPacket(ByteBuffer buf, NetworkDirection dir)
{
	int nid = buf.nextInt(); buf.reset();
	if (dir == SERVER_BOUND && isServer())
	{
		PacketDescription desc = serverBoundPacketHandlers[nid];
		Packet packet = desc.fromBytes(buf);
		desc.handle(packet);
	}
	if (dir == CLIENT_BOUND && !isServer())
	{
		PacketDescription desc = clientBoundPacketHandlers[nid];
		Packet packet = desc.fromBytes(buf);
		desc.handle(packet);
	}
}
void PacketHandler::sendPacketToServer(Packet packet)
{

}
void PacketHandler::sendPacketToClient(size_t pid, Packet packet)
{

}
void PacketHandler::sendPacketToAllClients(Packet packet)
{

}