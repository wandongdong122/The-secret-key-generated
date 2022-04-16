#pragma once
#include "RequestCodec.h"
#include "SecKeyShm.h"
#include "TcpServer.h"
#include <map>
#include <pthread.h>
#include "TcpSocket.h"
#include <iostream>
#include <utility>
#include "CodecFactory.h"
#include "RespondFactory.h"
#include "RespondCodec.h"
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <MYSQL_.h>



class ServerInfo
{
public: 
	char            dbIP[16];       // ������IP
	char			serverID[12];	// �������˱��
	char			dbUse[24]; 		// ���ݿ��û���
	char			dbPasswd[24]; 	// ���ݿ�����
	char			dbSID[24]; 		// ���ݿ�sid

	unsigned short 	sPort;			// �������󶨵Ķ˿�
	int				maxnode;		// �����ڴ���������� �ͻ���Ĭ��1��
	int 			shmkey;			// �����ڴ�keyid ���������ڴ�ʱʹ��	 
};

class ServerOperation
{
public:
	ServerOperation(ServerInfo* info);
	~ServerOperation();

	// ��������ʼ����
	void startWork();
	// ��ԿЭ��
	int secKeyAgree(RequestMsg* reqmsg, char** outData, int& outLen);
	// ��ԿУ��
	int secKeyCheck();
	// ��Կע��
	int secKeyRevoke();
	// ��Կ�鿴
	int secKeyView();

	friend void* working(void* arg);
	// �̻߳ص�Ҳ��ʹ�þ�̬��Ա����
	static void* wrokingHard(void* arg);
	static void catchSignal(int num);

private:
	void getRandString(int len, char* randBuf);

private:
	ServerInfo m_info;
	SecKeyShm* m_shm;
	TcpServer m_server;
	TcpSocket* m_client;
	MYSQLOP m_mysql;
	std::map<pthread_t, TcpSocket*> m_listSocket;
	static bool m_stop;
};

void* working(void* arg);
