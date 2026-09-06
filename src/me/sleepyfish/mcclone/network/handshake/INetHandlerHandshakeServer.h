//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_INETHANDLERHANDSHAKESERVER_H
#define MCCLONE_INETHANDLERHANDSHAKESERVER_H

#include "../INetHandler.h"

/**
 * @author SleepyFish
 */
class INetHandlerHandshakeServer : public INetHandler {

public:

    virtual ~INetHandlerHandshakeServer() = default;

    // virtual void processHandshake(C00Handshake packetIn) = 0;

};


#endif //MCCLONE_INETHANDLERHANDSHAKESERVER_H
