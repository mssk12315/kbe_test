#ifndef common_client_H_H
#define common_client_H_H

#include "network\event_dispatcher.h"
#include "network/endpoint.h"
#include "common\memorystream.h"

using namespace KBEngine;
using namespace KBEngine::Network;


class common_client : public TimerHandler
{
public:
	common_client();

	virtual void run(void);
	virtual void handleTimeout(TimerHandle handle, void * pUser);

protected:
	void initNetwork(void);

protected:
	EventDispatcher dispatcher;
	EndPoint::SmartPoolObjectPtr pEndPointSrv;
};

#endif