//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_C01PACKETPING_H
#define MCCLONE_C01PACKETPING_H

/* not implemented yet.
public class C01PacketPing implements Packet<INetHandlerStatusServer> {
    public void readPacketData(final PacketBuffer buf) throws IOException {
        this.clientTime = buf.readLong();
    }

    public void writePacketData(final PacketBuffer buf) throws IOException {
        buf.writeLong(this.clientTime);
    }

    public void processPacket(final INetHandlerStatusServer handler) {
        handler.processPing(this);
    }
}
*/

/**
 * @author SleepyFish
 */
class C01PacketPing {

private:

    long clientTime;

public:

    explicit C01PacketPing(long ping) :
        clientTime(ping)
    {}

    long getClientTime() const {
        return this->clientTime;
    }

};


#endif //MCCLONE_C01PACKETPING_H
