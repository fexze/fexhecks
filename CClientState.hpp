#pragma once

#include <cstdint>

typedef enum
{
	NA_NULL = 0,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
}   netadrtype_t;

typedef struct netadr_s
{
public:
	netadrtype_t	type;
	unsigned char	ip[4];
	unsigned short	port;
}   netadr_t;

class INetMessage;
class IDemoRecorder;
class INetMessageBinder;
class INetChannelHandler;

class INetChannel : public INetChannelInfo
{
public:
	virtual	                        ~INetChannel(void) {};
	virtual void				    SetDataRate(float rate) = 0;
	virtual bool				    RegisterMessage(INetMessageBinder* msg) = 0;
	virtual bool				    UnregisterMessage(INetMessageBinder* msg) = 0;
	virtual bool				    StartStreaming(unsigned int challengeNr) = 0;
	virtual void				    ResetStreaming(void) = 0;
	virtual void				    SetTimeout(float seconds, bool bForceExact = false) = 0;
	virtual void				    SetDemoRecorder(IDemoRecorder* recorder) = 0;
	virtual void				    SetChallengeNr(unsigned int chnr) = 0;
	virtual void				    Reset(void) = 0;
	virtual void				    Clear(void) = 0;
	virtual void				    Shutdown(const char* reason) = 0;
	virtual void				    ProcessPlayback(void) = 0;
	virtual bool				    ProcessStream(void) = 0;
	virtual void				    ProcessPacket(struct netpacket_s* packet, bool bHasHeader) = 0;
	virtual bool	                SendNetMsg(INetMessage& msg, bool bForceReliable = false, bool bVoice = false) = 0;
	virtual bool				    SendData(bf_write& msg, bool bReliable = true) = 0;
	virtual bool				    SendFile(const char* filename, unsigned int transferID, bool isReplayDemo) = 0;
	virtual void				    DenyFile(const char* filename, unsigned int transferID, bool isReplayDemo) = 0;
	virtual void	                RequestFile_OLD(const char* filename, unsigned int transferID) = 0;
	virtual void	                SetChoked(void) = 0;
	virtual int		                SendDatagram(bf_write* data) = 0;
	virtual bool	                Transmit(bool onlyReliable = false) = 0;
	virtual const netadr_t&         GetRemoteAddress(void) const = 0;
	virtual INetChannelHandler*     GetMsgHandler(void) const = 0;
	virtual int				        GetDropNumber(void) const = 0;
	virtual int				        GetSocket(void) const = 0;
	virtual unsigned int	        GetChallengeNr(void) const = 0;
	virtual void				    GetSequenceData(int& nOutSequenceNr, int& nInSequenceNr, int& nOutSequenceNrAck) = 0;
	virtual void				    SetSequenceData(int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck) = 0;
	virtual void				    UpdateMessageStats(int msggroup, int bits) = 0;
	virtual bool				    CanPacket(void) const = 0;
	virtual bool				    IsOverflowed(void) const = 0;
	virtual bool				    IsTimedOut(void) const = 0;
	virtual bool				    HasPendingReliableData(void) = 0;
	virtual void				    SetFileTransmissionMode(bool bBackgroundMode) = 0;
	virtual void				    SetCompressionMode(bool bUseCompression) = 0;
	virtual unsigned int            RequestFile(const char* filename, bool isReplayDemoFile) = 0;
	virtual float				    GetTimeSinceLastReceived(void) const = 0;
	virtual void				    SetMaxBufferSize(bool bReliable, int nBytes, bool bVoice = false) = 0;
	virtual bool				    IsNull() const = 0;
	virtual int				        GetNumBitsWritten(bool bReliable) = 0;
	virtual void				    SetInterpolationAmount(float flInterpolationAmount) = 0;
	virtual void				    SetRemoteFramerate(float flFrameTime, float flFrameTimeStdDeviation) = 0;
	virtual void				    SetMaxRoutablePayloadSize(int nSplitSize) = 0;
	virtual int				        GetMaxRoutablePayloadSize() = 0;
	virtual bool				    SetActiveChannel(INetChannel* pNewChannel) = 0;
	virtual void				    AttachSplitPlayer(int nSplitPlayerSlot, INetChannel* pChannel) = 0;
	virtual void				    DetachSplitPlayer(int nSplitPlayerSlot) = 0;
	virtual bool				    IsRemoteDisconnected() const = 0;
	virtual bool				    WasLastMessageReliable() const = 0;

	char         pad_0000[20];
	bool         m_bProcessingMessages;
	bool         m_bShouldDelete;
	char         pad_0016[2];
	int32_t      m_nOutSequenceNr;
	int32_t      m_nInSequenceNr;
	int32_t      m_nOutSequenceNrAck;
	int32_t      m_nOutReliableState;
	int32_t      m_nInReliableState;
	int32_t      m_nChokedPackets;
	char         pad_0030[1044];
};

class CEventInfo
{
public:
	uint16_t          classID;
	char              pad_0002[2];
	float             fire_delay;
	char              pad_0008[4];
	ClientClass*      pClientClass;
	void*             pData;
	char              pad_0014[48];
};

class CClientState
{
public:
    void ForceFullUpdate()
    {
		m_nDeltaTick = -1;
    }

	char              pad_0000[148];
	INetChannel*      m_NetChannel;
	char              pad_0098[8];
	uint32_t          m_nChallengeNr;
	char              pad_00A4[100];
	uint32_t          m_nSignonState;
	char              pad_010C[8];
	float             m_flNextCmdTime;
	uint32_t          m_nServerCount;
	uint32_t          m_nCurrentSequence;
	char              pad_0120[84];
	uint32_t          m_nDeltaTick;
	bool              m_bPaused;
	char              pad_0179[7];
	uint32_t          m_nViewEntity;
	uint32_t          m_nPlayerSlot;
	char              m_szLevelName[260];
	char              m_szLevelNameShort[40];
	char              m_szGroupName[40];
	char              pad_02DC[52];
	uint32_t          m_nMaxClients;
	char              pad_0314[18820];
	float             m_flLastServerTickTime;
	bool              insimulation;
	char              pad_4C9D[3];
	uint32_t          oldtickcount;
	float             m_tickRemainder;
	float             m_frameTime;
	uint32_t          lastoutgoingcommand;
	uint32_t          chokedcommands;
	uint32_t          last_command_ack;
	uint32_t          command_ack;
	uint32_t          m_nSoundSequence;
	char              pad_4CC0[80];
	Vector            viewangles;
	char              pad_4D1C[208];
	CEventInfo*       events;
};

class INetMessage
{
public:
	virtual	                  ~INetMessage() {};
	virtual void	          SetNetChannel(INetChannel* netchan) = 0;
	virtual void	          SetReliable(bool state) = 0;
	virtual bool	          Process(void) = 0;
	virtual	bool	          ReadFromBuffer(bf_read& buffer) = 0;
	virtual	bool	          WriteToBuffer(bf_write& buffer) = 0;
	virtual bool	          IsReliable(void) const = 0;
	virtual int			      GetType(void) const = 0;
	virtual int			      GetGroup(void) const = 0;
	virtual const char*       GetName(void) const = 0;
	virtual INetChannel*      GetNetChannel(void) const = 0;
	virtual const char*       ToString(void) const = 0;
};

template<typename T>
class CNetMessagePB : public INetMessage, public T {};

class CCLCMsg_Move
{
private:
	char __PAD0[0x8];

public:
	int numBackupCommands;
	int numNewCommands;
};

using CCLCMsg_Move_t = CNetMessagePB<CCLCMsg_Move>;
static_assert(FIELD_OFFSET(CClientState, m_NetChannel) == 0x0094, "Wrong struct offset");
static_assert(FIELD_OFFSET(CClientState, viewangles) == 0x4D10, "Wrong struct offset");