//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_INETHANDLERPLAYSERVER_H
#define MCCLONE_INETHANDLERPLAYSERVER_H

#include "../INetHandler.h"

/* not implemented yet.
#include "play/client/C00PacketKeepAlive.h"
#include "play/client/C01PacketChatMessage.h"
#include "play/client/C02PacketUseEntity.h"
#include "play/client/C03PacketPlayer.h"
#include "play/client/C07PacketPlayerDigging.h"
#include "play/client/C08PacketPlayerBlockPlacement.h"
#include "play/client/C09PacketHeldItemChange.h"
#include "play/client/C0APacketAnimation.h"
#include "play/client/C0BPacketEntityAction.h"
#include "play/client/C0CPacketInput.h"
#include "play/client/C0DPacketCloseWindow.h"
#include "play/client/C0EPacketClickWindow.h"
#include "play/client/C0FPacketConfirmTransaction.h"
#include "play/client/C10PacketCreativeInventoryAction.h"
#include "play/client/C11PacketEnchantItem.h"
#include "play/client/C12PacketUpdateSign.h"
#include "play/client/C13PacketPlayerAbilities.h"
#include "play/client/C14PacketTabComplete.h"
#include "play/client/C15PacketClientSettings.h"
#include "play/client/C16PacketClientStatus.h"
#include "play/client/C17PacketCustomPayload.h"
#include "play/client/C18PacketSpectate.h"
#include "play/client/C19PacketResourcePackStatus.h"
*/

/**
 * @author SleepyFish
 */
class INetHandlerPlayServer : public INetHandler {

public:

    /*
    virtual void handleAnimation(C0APacketAnimation packetIn) = 0;
    virtual void processChatMessage(C01PacketChatMessage packetIn) = 0;
    virtual void processTabComplete(C14PacketTabComplete packetIn) = 0;
    virtual void processClientStatus(C16PacketClientStatus packetIn) = 0;
    virtual void processClientSettings(C15PacketClientSettings packetIn) = 0;
    virtual void processConfirmTransaction(C0FPacketConfirmTransaction packetIn) = 0;
    virtual void processEnchantItem(C11PacketEnchantItem packetIn) = 0;
    virtual void processClickWindow(C0EPacketClickWindow packetIn) = 0;
    virtual void processCloseWindow(C0DPacketCloseWindow packetIn) = 0;
    virtual void processVanilla250Packet(C17PacketCustomPayload packetIn) = 0;
    virtual void processUseEntity(C02PacketUseEntity packetIn) = 0;
    virtual void processKeepAlive(C00PacketKeepAlive packetIn) = 0;
    virtual void processPlayer(C03PacketPlayer packetIn) = 0;
    virtual void processPlayerAbilities(C13PacketPlayerAbilities packetIn) = 0;
    virtual void processPlayerDigging(C07PacketPlayerDigging packetIn) = 0;
    virtual void processEntityAction(C0BPacketEntityAction packetIn) = 0;
    virtual void processInput(C0CPacketInput packetIn) = 0;
    virtual void processHeldItemChange(C09PacketHeldItemChange packetIn) = 0;
    virtual void processCreativeInventoryAction(C10PacketCreativeInventoryAction packetIn) = 0;
    virtual void processUpdateSign(C12PacketUpdateSign packetIn) = 0;
    virtual void processPlayerBlockPlacement(C08PacketPlayerBlockPlacement packetIn) = 0;
    virtual void handleSpectate(C18PacketSpectate packetIn) = 0;
    virtual void handleResourcePackStatus(C19PacketResourcePackStatus packetIn) = 0;
    */

};


#endif //MCCLONE_INETHANDLERPLAYSERVER_H
