#include "common_client.h"

common_client::common_client()
{

}

void common_client::run(void)
{
	initNetwork();
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

	dispatcher.addTimer(3 * 1000 * 1000, this);
}

void common_client::handleTimeout(TimerHandle handle, void * pUser)
{
	MemoryStream stream;
	stream << "hello kbe";
	int iRet = (*pEndPointSrv)->send(stream.data(), stream.length());
	printf("send iRet = %d \n", iRet);
}