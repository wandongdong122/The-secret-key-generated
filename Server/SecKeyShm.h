#pragma once
#include"ShareMemory.h"

class NodeSHMInfo
{
public:
	int status;   // ��ǰ��Կ״̬ 
	int seckeyID; // ��ԿID
	char clientID[12]; // �ͻ���ID
	char serverID[12]; // �����ID
	char seckey[128]; // ��Կ
};

class SecKeyShm :public ShareMemory
{
public:
	SecKeyShm(int key);
	SecKeyShm(char* pathName);
	SecKeyShm(int key, int maxNode);
	SecKeyShm(char* pathName, int maxNode);
	int shmWrite(NodeSHMInfo* pNodeInfo);
	int shmRead(char* clientID, char* serverID, NodeSHMInfo* pNodeSHMInfo);

	void printShm();
private:
	int m_maxNodeNum;
};
