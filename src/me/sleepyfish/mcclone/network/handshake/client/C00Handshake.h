//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_C00HANDSHAKE_H
#define MCCLONE_C00HANDSHAKE_H

#include "../INetHandlerHandshakeServer.h"
#include "../../Packet.h"

#include <string>
#include <cstdint>

/* not implemented yet.
class C00Handshake implements Packet<INetHandlerHandshakeServer> {
    public void readPacketData(final PacketBuffer buf) throws IOException {
        this.protocolVersion = buf.readVarIntFromBuffer();
        this.ip = buf.readStringFromBuffer(255);
        this.port = buf.readUnsignedShort();
        this.requestedState = EnumConnectionState.getById(buf.readVarIntFromBuffer());
    }

    public void writePacketData(final PacketBuffer buf) throws IOException {
        buf.writeVarIntToBuffer(this.protocolVersion);
        buf.writeString(this.ip);
        buf.writeShort(this.port);
        buf.writeVarIntToBuffer(this.requestedState.getId());
    }

    public void processPacket(final INetHandlerHandshakeServer handler) {
        handler.processHandshake(this);
    }
}
*/

/**
 * @author SleepyFish
 */
class C00Handshake : public Packet<INetHandlerHandshakeServer> {

private:

    const uint8_t protocolVersion;
    const std::string ip;
    const uint16_t port;
    // const EnumConnectionState requestedState;

public:

    C00Handshake() = default;

    /*
    C00Handshake(uint8_t protocolVersion, const std::string& ip, uint16_t port, EnumConnectionState requestedState) :
        protocolVersion(protocolVersion),
        ip(ip),
        port(port),
        requestedState(requestedState)
    {}

    EnumConnectionState getRequestedState() const {
        return this->requestedState;
    }
    */

    uint8_t getProtocolVersion() const {
        return this->protocolVersion;
    }

};

#endif //MCCLONE_C00HANDSHAKE_H
