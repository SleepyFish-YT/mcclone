//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_S00PACKETSERVERINFO_H
#define MCCLONE_S00PACKETSERVERINFO_H

/* not implemented yet.
class S00PacketServerInfo implements Packet<INetHandlerStatusClient> {
    private static final Gson GSON = (new GsonBuilder()).registerTypeAdapter(ServerStatusResponse.MinecraftProtocolVersionIdentifier.class, new ServerStatusResponse.MinecraftProtocolVersionIdentifier.Serializer()).registerTypeAdapter(ServerStatusResponse.PlayerCountData.class, new ServerStatusResponse.PlayerCountData.Serializer()).registerTypeAdapter(ServerStatusResponse.class, new ServerStatusResponse.Serializer()).registerTypeHierarchyAdapter(IChatComponent.class, new IChatComponent.Serializer()).registerTypeHierarchyAdapter(ChatStyle.class, new ChatStyle.Serializer()).registerTypeAdapterFactory(new EnumTypeAdapterFactory()).create();
    private ServerStatusResponse response;

    public S00PacketServerInfo(final ServerStatusResponse responseIn) {
        this.response = responseIn;
    }

    public void readPacketData(final PacketBuffer buf) throws IOException {
        this.response = GSON.fromJson(buf.readStringFromBuffer(32767), ServerStatusResponse.class);
    }

    public void writePacketData(final PacketBuffer buf) throws IOException {
        buf.writeString(GSON.toJson(this.response));
    }

    public void processPacket(final INetHandlerStatusClient handler) {
        handler.handleServerInfo(this);
    }

    public ServerStatusResponse getResponse() {
        return this.response;
    }
}
*/

/**
 * @author SleepyFish
 */
class S00PacketServerInfo {

private:


public:

    S00PacketServerInfo() = default;

};


#endif //MCCLONE_S00PACKETSERVERINFO_H
