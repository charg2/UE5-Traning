#include "NetworkManager.h"
#include "Sockets.h"

NetworkManager::NetworkManager()
    : IsConnected  {}
    , PacketHandler{}
{
	Socket = ISocketSubsystem::Get( PLATFORM_SOCKETSUBSYSTEM )->CreateSocket(
		NAME_Stream,
		TEXT( "SOCK" ),
		false );

	Thread = FRunnableThread::Create(
		this,
		TEXT( "NETWORK_MANAGER" ),
		0 );

	RecvBuffer = new char[ 1024 ]{};
	SendBuffer = new char[ 1024 ]{};
}

NetworkManager::~NetworkManager()
{
	delete Socket;
	delete Thread;

	delete RecvBuffer;
	delete SendBuffer;
}

bool NetworkManager::Init()
{
    return FRunnable::Init() && Initialize();
}

bool NetworkManager::Initialize()
{
	UE_LOG( LogTemp, Warning, TEXT( "NetworkManager::Initialize Success!" ) );

	return true;
}

uint32 NetworkManager::Run()
{
	return 0;
}

void NetworkManager::Finalize()
{
}

void NetworkManager::TryReconnect()
{
}

void NetworkManager::Connect()
{
	FIPv4Address::Parse( TEXT( "127.0.0.1" ), IPAddress );

	TSharedRef< FInternetAddr > addr = ISocketSubsystem::Get( PLATFORM_SOCKETSUBSYSTEM )->CreateInternetAddr();
	addr->SetIp  ( IPAddress.Value );
	addr->SetPort( 8888            );

	bool connected = Socket->Connect( *addr );
}

void NetworkManager::SendAsync( byte* InBuffer, size_t InSize )
{
}

void NetworkManager::FlushSendPacket()
{
	int byteSent{};
	Socket->Send( (const uint8*)( SendBuffer ), 1024, byteSent );
}

void NetworkManager::SendPacket( IPacket& packet )
{
}

void NetworkManager::RecvAsync()
{
	int bytesRead{};
	Socket->Recv( (uint8*)( RecvBuffer ), 1024, bytesRead );
}
