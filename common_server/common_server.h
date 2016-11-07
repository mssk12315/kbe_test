#ifndef common_server_H_H
#define common_server_H_H

#include "network\event_dispatcher.h"
#include "network\network_interface.h"
#include "network\message_handler.h"
#include "network\interface_defs.h"
#include "network/endpoint.h"
#include "common\memorystream.h"

using namespace KBEngine;
using namespace KBEngine::Network;

class PrintTask : public Task
{
public:
	PrintTask(EventDispatcher &dispatcher) :m_dispatcher(dispatcher)
	{
		m_times = 0;
	}

	virtual bool process()
	{
		// printf("PrintTask process time %d \n", ++m_times);

		return m_times < 100;
	}

private:
	int m_times;
	EventDispatcher &m_dispatcher;
};

class CloseTimer : public TimerHandler
{
public:
	CloseTimer(EventDispatcher &dispatcher) :m_dispatcher(dispatcher)
	{

	}
	virtual void handleTimeout(TimerHandle handle, void * pUser)
	{
		m_dispatcher.breakProcessing();
	}

private:
	EventDispatcher &m_dispatcher;
};

class EPRecv : public InputNotificationHandler
{
public:
	EPRecv(EndPoint &ep) :m_ep(ep)
	{

	}

	virtual int handleInputNotification(int fd)
	{
		MemoryStream stream;
		stream.data_resize(MemoryStream::DEFAULT_SIZE);
		int iRet = m_ep.recv(stream.data() + stream.wpos(), stream.size() - stream.wpos());
		if (iRet > 0)
		{
			stream.wpos(stream.wpos() + iRet);
			char strRecv[1024] = { 0 };
			stream >> strRecv;
			printf("recv from %s data %s \n", strRecv, m_ep.addr().c_str());
		}

		return 0;
	}

private:
	EndPoint &m_ep;
};

class EPAccept : public InputNotificationHandler
{
public:
	EPAccept(EventDispatcher &dispatcher, EndPoint &ep) :m_dispatcher(dispatcher), m_ep(ep)
	{

	}

	virtual int handleInputNotification(int fd)
	{
		EndPoint *pAccept = m_ep.accept(NULL, NULL, false);
		if (pAccept != NULL)
		{
			printf("accept addr %s \n", pAccept->addr().c_str());
			EPRecv *pRecv = new EPRecv(*pAccept);

			m_dispatcher.registerReadFileDescriptor((KBESOCKET)(*pAccept), pRecv);
		}

		return 0;
	}

private:
	EventDispatcher &m_dispatcher;
	EndPoint &m_ep;
};

class common_server : public TimerHandler
{
public:
	common_server();

	virtual void run(void);
	virtual void handleTimeout(TimerHandle handle, void * pUser);
	
	void runScript(void);

protected:
	void initNetwork(void);
	void initScript(void);

protected:
	EventDispatcher dispatcher;
	NetworkInterface netInterface;
	PrintTask tsk;
	CloseTimer tm;
	TimerHandle hTm;
};

#endif