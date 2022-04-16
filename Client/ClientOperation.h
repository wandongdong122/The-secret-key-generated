#pragma once
#include "TcpSocket.h"
#include "SecKeyShm.h"

class ClientInfo
{
public:
	char clinetID[12];			// �ͻ���ID
	char serverID[12];			// ������ID
	//char authCode[65];		// ��Ϣ��֤��
	char serverIP[32];			// ������IP
	unsigned short serverPort;	// �������˿�
	int maxNode;				// �����ڴ�ڵ����
	int shmKey;					// �����ڴ��Key
};


class ClientOperation
{
public:
	ClientOperation(ClientInfo* info);
	~ClientOperation();
	
	// ��ԿЭ��
	int secKeyAgree();
	// ��ԿУ��
	int secKeyCheck();
	// ��Կע��
	int secKeyRevoke();
	// ��Կ�鿴
	int secKeyView();

private:
	void getRandString(int len, char* randBuf);
private:
	ClientInfo m_info;
	TcpSocket m_socket;
	SecKeyShm* m_shm;
};