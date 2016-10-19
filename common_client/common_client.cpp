#include "common_client.h"

MessageHandlers messageHandlers;

MESSAGE_STREAM(common_client)

class HelloServerMessageHandler : public MessageHandler
{
public:
	HelloServerMessageHandler()
	{
		onInstall();
	}

	virtual void onInstall()
	{
		name = "HelloServer";
		msgID = 101;
		msgLen = NETWORK_VARIABLE_MESSAGE;
	}

	virtual void handle(Channel* pChannel, MemoryStream& s)
	{
		char strHello[128] = { 0 };
		unsigned short iFlag = 0;

		s >> strHello;
		s >> iFlag;

		printf("[ ] recv from %s data %s %d \n", pChannel->c_str(), strHello, iFlag);
	}
};

MessageHandler *pHelloServerMessageHandler = NULL;

common_client::common_client()
:netInterface(&dispatcher)
{
	DebugHelper::getSingleton().pDispatcher(&dispatcher);

	netInterface.pChannelTimeOutHandler(this);
	netInterface.pChannelDeregisterHandler(this);

	pNetChannel = Network::Channel::createPoolObject();
	pNetChannel->pNetworkInterface(&netInterface);

	pHelloServerMessageHandler = messageHandlers.add("HelloServer", new common_clientArgs_stream, NETWORK_VARIABLE_MESSAGE, new HelloServerMessageHandler);
}

common_client::~common_client()
{
	if (pNetChannel)
	{
		if (!pNetChannel->isDestroyed())
		{
			pNetChannel->destroy();
		}

		Network::Channel::reclaimPoolObject(pNetChannel);
		pNetChannel = NULL;
	}
}

void common_client::run(void)
{
	initNetwork();
	dispatcher.addTimer(100 * 1000, this, (void *)10000);
	dispatcher.processUntilBreak();
}

void common_client::initNetwork(void)
{
	pEndPointSrv = EndPoint::createSmartPoolObj();

	(*pEndPointSrv)->socket(SOCK_STREAM);
	printf("socket good = %d \n", (*pEndPointSrv)->good());
	(*pEndPointSrv)->addr(Address("127.0.0.1", 7901));
	int iRet = (*pEndPointSrv)->connect(false);
	printf("connect iRet = %d WSAGetLastError = %d \n", iRet, WSAGetLastError());

	EndPoint *pEndPoint = EndPoint::createPoolObject();
	pEndPoint->socket(SOCK_STREAM);
	pEndPoint->addr(Address("127.0.0.1", 7902));
	iRet = pEndPoint->connect();
	printf("connect iRet = %d WSAGetLastError = %d \n", iRet, WSAGetLastError());

	pNetChannel->pEndPoint(pEndPoint);
	netInterface.registerChannel(pNetChannel);
	dispatcher.registerReadFileDescriptor(*pEndPoint, new TCPPacketReceiver(*pEndPoint, netInterface));

	dispatcher.addTimer(3 * 1000 * 1000, this);
}

void common_client::handleTimeout(TimerHandle handle, void * pUser)
{
	unsigned int iUser = reinterpret_cast<unsigned int>(pUser);

	if (iUser == 10000)
	{
		netInterface.processChannels(&messageHandlers);
		return;
	}

	MemoryStream stream;
	stream << "hello kbe";
	int iRet = (*pEndPointSrv)->send(stream.data(), stream.length());
	printf("send iRet = %d \n", iRet);

	if (pNetChannel != NULL)
	{
		Bundle *pBundle = Bundle::createPoolObject();
		pBundle->newMessage(*pHelloServerMessageHandler);
		*pBundle << "Hello Server";
		*pBundle << (unsigned short)25;
		pNetChannel->send(pBundle);
	}
}

void common_client::onChannelTimeOut(Network::Channel * pChannel)
{
	printf("onChannelTimeOut isCondemn = %d isDestroyed = %d \n", pChannel->isCondemn(), pChannel->isDestroyed());
}

void common_client::onChannelDeregister(Network::Channel * pChannel)
{
	printf("onChannelDeregister isCondemn = %d isDestroyed = %d \n", pChannel->isCondemn(), pChannel->isDestroyed());
	if (pChannel == pNetChannel)
	{
		pNetChannel = NULL;
	}
}