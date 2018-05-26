#pragma once

#include "Recv.hpp"

class ClientClass;
class IClientNetworkable;

typedef IClientNetworkable* (*CreateEventFn)();
typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);

class ClientClass
{
public:
	CreateClientClassFn      m_pCreateFn;
	CreateEventFn            m_pCreateEventFn;
	char*                    m_pNetworkName;
	RecvTable*               m_pRecvTable;
	ClientClass*             m_pNext;
	ClassId                  m_ClassID;
};