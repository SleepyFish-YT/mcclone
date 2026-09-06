//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_S01PACKETPONG_H
#define MCCLONE_S01PACKETPONG_H

/* not implemented yet.
class S01PacketPong implements Packet<INetHandlerStatusClient> {
    public void readPacketData(final PacketBuffer buf) throws IOException {
        this.clientTime = buf.readLong();
    }

    public void writePacketData(final PacketBuffer buf) throws IOException {
        buf.writeLong(this.clientTime);
    }

    public void processPacket(final INetHandlerStatusClient handler) {
        handler.handlePong(this);
    }
}
*/

/**
 * @author SleepyFish
 */
class S01PacketPong {

private:

    long clientTime;

public:

    S01PacketPong(const long time) :
        clientTime(time)
    {}

};


#endif //MCCLONE_S01PACKETPONG_H
