#include "common_server.h"

common_server::common_server()
:tsk(dispatcher),
tm(dispatcher)
{

}

void common_server::run(void)
{
	dispatcher.addTask(&tsk);
	hTm = dispatcher.addTimer(150 * 1000 * 1000, &tm);
	initNetwork();
	dispatcher.processUntilBreak();

	hTm.cancel();
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