//
// Created by SleepyFish on 05.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_PACKET_H
#define MCCLONE_PACKET_H

#include "INetHandler.h"
// #include "PacketBuffer.h"

#include <concepts>

// not FULLY implemented yet.

/**
 * @author SleepyFish
 */
template<typename T = INetHandler>
requires std::derived_from<T, INetHandler>
class Packet {

public:

    virtual ~Packet() = default;

    // virtual void readPacketData(PacketBuffer buf) = 0; // throws IOException;

    // virtual void writePacketData(PacketBuffer buf) = 0; // throws IOException;

    virtual void processPacket(T handler) = 0;

};


#endif //MCCLONE_PACKET_H
