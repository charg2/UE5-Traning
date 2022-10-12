#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Core/Public/HAL/Runnable.h"
#include "SocketSubsystem.h"
#include "Networking/Public/Networking.h"


using byte = char;
class FSocket;
class IPacket;
class FRunnableThread;

class Session
{
    uint64 Id;
};

class NetworkManager final : public FRunnable
{
    NetworkManager();
    virtual ~NetworkManager() override;

    virtual bool Init() override final;

    bool Initialize();
    void Finalize();
    virtual uint32 Run();

    void TryReconnect();
    void Connect();

    void SendAsync( byte* InBuffer, size_t InSize );
    void FlushSendPacket();
    void SendPacket( IPacket& InPacket );

    void RecvAsync();

private:
    FRunnableThread* Thread;
    FSocket*         Socket;
    FIPv4Address     IPAddress;
    char*            RecvBuffer;
    char*            SendBuffer;
    bool             IsConnected;
    char*            PacketHandler;
};

