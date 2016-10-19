#include "common_server.h"

MessageHandlers messageHandlers;

MESSAGE_STREAM(common_server)

MessageHandler *pHelloClientMessageHandler = NULL;

class HelloClientMessageHandler : public MessageHandler
{
public:
	HelloClientMessageHandler()
	{
		onInstall();
	}

	virtual void onInstall()
	{
		name = "HelloClient";
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

		Bundle *pBundle = Bundle::createPoolObject();
		pBundle->newMessage(*pHelloClientMessageHandler);
		(*pBundle) << "Hello Client";
		(*pBundle) << (unsigned short)24;
		pBundle->finiMessage();

		pBundle->newMessage(*pHelloClientMessageHandler);
		(*pBundle) << "Hello Client";
		(*pBundle) << (unsigned short)22;

		pChannel->send(pBundle);
	}
};

common_server::common_server()
:tsk(dispatcher),
netInterface(&dispatcher, 7902, 7902),
tm(dispatcher)
{
	DebugHelper::getSingleton().pDispatcher(&dispatcher);

	pHelloClientMessageHandler = messageHandlers.add("HelloClient", new common_serverArgs_stream, NETWORK_VARIABLE_MESSAGE, new HelloClientMessageHandler);
}

void common_server::run(void)
{
	dispatcher.addTask(&tsk);
	//hTm = dispatcher.addTimer(150 * 1000 * 1000, &tm);
	dispatcher.addTimer(100 * 1000, this, (void *)10000);
	initNetwork();
	dispatcher.processUntilBreak();

	//hTm.cancel();
}

void common_server::initNetwork(void)
{
	static EndPoint::SmartPoolObjectPtr pEndPointSrv = EndPoint::createSmartPoolObj();

	(*pEndPointSrv)->socket(SOCK_STREAM);
	printf("socket good = %d \n", (*pEndPointSrv)->good());
	int iRet = (*pEndPointSrv)->bind(htons(7901), htonl(ADDR_ANY));
	printf("bind iRet = %d \n", iRet);
	iRet = (*pEndPointSrv)->listen();
	printf("bind listen = %d \n", iRet);


	//(*pEndPointSrv)->setnonblocking(true);
	//(*pEndPointSrv)->setnodelay(true);

	EPAccept *pAccept = new EPAccept(dispatcher, *(*pEndPointSrv));
	dispatcher.registerReadFileDescriptor((KBESOCKET)(*(*pEndPointSrv)), pAccept);
}

void common_server::handleTimeout(TimerHandle handle, void * pUser)
{
	unsigned int iUser = reinterpret_cast<unsigned int>(pUser);

	if (iUser == 10000)
	{
		netInterface.processChannels(&messageHandlers);
	}
}