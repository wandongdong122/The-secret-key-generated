#pragma once
#include"ShareMemory.h"

class NodeSHMInfo
{
public:
	int status;   // 当前秘钥状态 
	int seckeyID; // 秘钥ID
	char clientID[12]; // 客户端ID
	char serverID[12]; // 服务端ID
	char seckey[128]; // 秘钥
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
