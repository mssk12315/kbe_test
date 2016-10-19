#ifndef common_client_H_H
#define common_client_H_H

#include "network\event_dispatcher.h"
#include "network\network_interface.h"
#include "network\message_handler.h"
#include "network\interface_defs.h"
#include "network\channel.h"
#include "network\tcp_packet_receiver.h"
#include "network/endpoint.h"
#include "common\memorystream.h"


using namespace KBEngine;
using namespace KBEngine::Network;


class common_client : public TimerHandler, public ChannelTimeOutHandler, public ChannelDeregisterHandler
{
public:
	common_client();
	virtual ~common_client();

	virtual void run(void);
	virtual void handleTimeout(TimerHandle handle, void * pUser);
	virtual void onChannelTimeOut(Network::Channel * pChannel);
	virtual void onChannelDeregister(Network::Channel * pChannel);

protected:
	void initNetwork(void);

protected:
	EventDispatcher dispatcher;
	NetworkInterface netInterface;
	EndPoint::SmartPoolObjectPtr pEndPointSrv;
	Channel *pNetChannel;
	// TCPPacketReceiver
};

#endif