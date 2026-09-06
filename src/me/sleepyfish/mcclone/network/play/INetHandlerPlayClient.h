//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_INETHANDLERPLAYCLIENT_H
#define MCCLONE_INETHANDLERPLAYCLIENT_H

#include "../INetHandler.h"

/* not implemented yet.
#include "play/server/S00PacketKeepAlive"
#include "play/server/S01PacketJoinGame"
#include "play/server/S02PacketChat"
#include "play/server/S03PacketTimeUpdate"
#include "play/server/S04PacketEntityEquipment"
#include "play/server/S05PacketSpawnPosition"
#include "play/server/S06PacketUpdateHealth"
#include "play/server/S07PacketRespawn"
#include "play/server/S08PacketPlayerPosLook"
#include "play/server/S09PacketHeldItemChange"
#include "play/server/S0APacketUseBed"
#include "play/server/S0BPacketAnimation"
#include "play/server/S0CPacketSpawnPlayer"
#include "play/server/S0DPacketCollectItem"
#include "play/server/S0EPacketSpawnObject"
#include "play/server/S0FPacketSpawnMob"
#include "play/server/S10PacketSpawnPainting"
#include "play/server/S11PacketSpawnExperienceOrb"
#include "play/server/S12PacketEntityVelocity"
#include "play/server/S13PacketDestroyEntities"
#include "play/server/S14PacketEntity"
#include "play/server/S18PacketEntityTeleport"
#include "play/server/S19PacketEntityHeadLook"
#include "play/server/S19PacketEntityStatus"
#include "play/server/S1BPacketEntityAttach"
#include "play/server/S1CPacketEntityMetadata"
#include "play/server/S1DPacketEntityEffect"
#include "play/server/S1EPacketRemoveEntityEffect"
#include "play/server/S1FPacketSetExperience"
#include "play/server/S20PacketEntityProperties"
#include "play/server/S21PacketChunkData"
#include "play/server/S22PacketMultiBlockChange"
#include "play/server/S23PacketBlockChange"
#include "play/server/S24PacketBlockAction"
#include "play/server/S25PacketBlockBreakAnim"
#include "play/server/S26PacketMapChunkBulk"
#include "play/server/S27PacketExplosion"
#include "play/server/S28PacketEffect"
#include "play/server/S29PacketSoundEffect"
#include "play/server/S2APacketParticles"
#include "play/server/S2BPacketChangeGameState"
#include "play/server/S2CPacketSpawnGlobalEntity"
#include "play/server/S2DPacketOpenWindow"
#include "play/server/S2EPacketCloseWindow"
#include "play/server/S2FPacketSetSlot"
#include "play/server/S30PacketWindowItems"
#include "play/server/S31PacketWindowProperty"
#include "play/server/S32PacketConfirmTransaction"
#include "play/server/S33PacketUpdateSign"
#include "play/server/S34PacketMaps"
#include "play/server/S35PacketUpdateTileEntity"
#include "play/server/S36PacketSignEditorOpen"
#include "play/server/S37PacketStatistics"
#include "play/server/S38PacketPlayerListItem"
#include "play/server/S39PacketPlayerAbilities"
#include "play/server/S3APacketTabComplete"
#include "play/server/S3BPacketScoreboardObjective"
#include "play/server/S3CPacketUpdateScore"
#include "play/server/S3DPacketDisplayScoreboard"
#include "play/server/S3EPacketTeams"
#include "play/server/S3FPacketCustomPayload"
#include "play/server/S40PacketDisconnect"
#include "play/server/S41PacketServerDifficulty"
#include "play/server/S42PacketCombatEvent"
#include "play/server/S43PacketCamera"
#include "play/server/S44PacketWorldBorder"
#include "play/server/S45PacketTitle"
#include "play/server/S46PacketSetCompressionLevel"
#include "play/server/S47PacketPlayerListHeaderFooter"
#include "play/server/S48PacketResourcePackSend"
#include "play/server/S49PacketUpdateEntityNBT"
*/

/**
 * @author SleepyFish
 */
class INetHandlerPlayClient : public INetHandler {

public:

    /* not implemented yet.
    virtual void handleSpawnObject(S0EPacketSpawnObject packetIn) = 0;
    virtual void handleSpawnExperienceOrb(S11PacketSpawnExperienceOrb packetIn) = 0;
    virtual void handleSpawnGlobalEntity(S2CPacketSpawnGlobalEntity packetIn) = 0;
    virtual void handleSpawnMob(S0FPacketSpawnMob packetIn) = 0;
    virtual void handleScoreboardObjective(S3BPacketScoreboardObjective packetIn) = 0;
    virtual void handleSpawnPainting(S10PacketSpawnPainting packetIn) = 0;
    virtual void handleSpawnPlayer(S0CPacketSpawnPlayer packetIn) = 0;
    virtual void handleAnimation(S0BPacketAnimation packetIn) = 0;
    virtual void handleStatistics(S37PacketStatistics packetIn) = 0;
    virtual void handleBlockBreakAnim(S25PacketBlockBreakAnim packetIn) = 0;
    virtual void handleSignEditorOpen(S36PacketSignEditorOpen packetIn) = 0;
    virtual void handleUpdateTileEntity(S35PacketUpdateTileEntity packetIn) = 0;
    virtual void handleBlockAction(S24PacketBlockAction packetIn) = 0;
    virtual void handleBlockChange(S23PacketBlockChange packetIn) = 0;
    virtual void handleChat(S02PacketChat packetIn) = 0;
    virtual void handleTabComplete(S3APacketTabComplete packetIn) = 0;
    virtual void handleMultiBlockChange(S22PacketMultiBlockChange packetIn) = 0;
    virtual void handleMaps(S34PacketMaps packetIn) = 0;
    virtual void handleConfirmTransaction(S32PacketConfirmTransaction packetIn) = 0;
    virtual void handleCloseWindow(S2EPacketCloseWindow packetIn) = 0;
    virtual void handleWindowItems(S30PacketWindowItems packetIn) = 0;
    virtual void handleOpenWindow(S2DPacketOpenWindow packetIn) = 0;
    virtual void handleWindowProperty(S31PacketWindowProperty packetIn) = 0;
    virtual void handleSetSlot(S2FPacketSetSlot packetIn) = 0;
    virtual void handleCustomPayload(S3FPacketCustomPayload packetIn) = 0;
    virtual void handleDisconnect(S40PacketDisconnect packetIn) = 0;
    virtual void handleUseBed(S0APacketUseBed packetIn) = 0;
    virtual void handleEntityStatus(S19PacketEntityStatus packetIn) = 0;
    virtual void handleEntityAttach(S1BPacketEntityAttach packetIn) = 0;
    virtual void handleExplosion(S27PacketExplosion packetIn) = 0;
    virtual void handleChangeGameState(S2BPacketChangeGameState packetIn) = 0;
    virtual void handleKeepAlive(S00PacketKeepAlive packetIn) = 0;
    virtual void handleChunkData(S21PacketChunkData packetIn) = 0;
    virtual void handleMapChunkBulk(S26PacketMapChunkBulk packetIn) = 0;
    virtual void handleEffect(S28PacketEffect packetIn) = 0;
    virtual void handleJoinGame(S01PacketJoinGame packetIn) = 0;
    virtual void handleEntityMovement(S14PacketEntity packetIn) = 0;
    virtual void handlePlayerPosLook(S08PacketPlayerPosLook packetIn) = 0;
    virtual void handleParticles(S2APacketParticles packetIn) = 0;
    virtual void handlePlayerAbilities(S39PacketPlayerAbilities packetIn) = 0;
    virtual void handlePlayerListItem(S38PacketPlayerListItem packetIn) = 0;
    virtual void handleDestroyEntities(S13PacketDestroyEntities packetIn) = 0;
    virtual void handleRemoveEntityEffect(S1EPacketRemoveEntityEffect packetIn) = 0;
    virtual void handleRespawn(S07PacketRespawn packetIn) = 0;
    virtual void handleEntityHeadLook(S19PacketEntityHeadLook packetIn) = 0;
    virtual void handleHeldItemChange(S09PacketHeldItemChange packetIn) = 0;
    virtual void handleDisplayScoreboard(S3DPacketDisplayScoreboard packetIn) = 0;
    virtual void handleEntityMetadata(S1CPacketEntityMetadata packetIn) = 0;
    virtual void handleEntityVelocity(S12PacketEntityVelocity packetIn) = 0;
    virtual void handleEntityEquipment(S04PacketEntityEquipment packetIn) = 0;
    virtual void handleSetExperience(S1FPacketSetExperience packetIn) = 0;
    virtual void handleUpdateHealth(S06PacketUpdateHealth packetIn) = 0;
    virtual void handleTeams(S3EPacketTeams packetIn) = 0;
    virtual void handleUpdateScore(S3CPacketUpdateScore packetIn) = 0;
    virtual void handleSpawnPosition(S05PacketSpawnPosition packetIn) = 0;
    virtual void handleTimeUpdate(S03PacketTimeUpdate packetIn) = 0;
    virtual void handleUpdateSign(S33PacketUpdateSign packetIn) = 0;
    virtual void handleSoundEffect(S29PacketSoundEffect packetIn) = 0;
    virtual void handleCollectItem(S0DPacketCollectItem packetIn) = 0;
    virtual void handleEntityTeleport(S18PacketEntityTeleport packetIn) = 0;
    virtual void handleEntityProperties(S20PacketEntityProperties packetIn) = 0;
    virtual void handleEntityEffect(S1DPacketEntityEffect packetIn) = 0;
    virtual void handleCombatEvent(S42PacketCombatEvent packetIn) = 0;
    virtual void handleServerDifficulty(S41PacketServerDifficulty packetIn) = 0;
    virtual void handleCamera(S43PacketCamera packetIn) = 0;
    virtual void handleWorldBorder(S44PacketWorldBorder packetIn) = 0;
    virtual void handleTitle(S45PacketTitle packetIn) = 0;
    virtual void handleSetCompressionLevel(S46PacketSetCompressionLevel packetIn) = 0;
    virtual void handlePlayerListHeaderFooter(S47PacketPlayerListHeaderFooter packetIn) = 0;
    virtual void handleResourcePack(S48PacketResourcePackSend packetIn) = 0;
    virtual void handleEntityNBT(S49PacketUpdateEntityNBT packetIn) = 0;
    */

};


#endif //MCCLONE_INETHANDLERPLAYCLIENT_H
