#include"SecKeyShm.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <memory.h>

using namespace std;

SecKeyShm::SecKeyShm(int key):ShareMemory(key)
{
}

//SecKeyShm::~SecKeyShm()
//{
//
//}

SecKeyShm::SecKeyShm( char* pathName): ShareMemory(pathName)
{

}

SecKeyShm::SecKeyShm(int key, int maxNode):
	ShareMemory(key,sizeof(int)+maxNode*sizeof(NodeSHMInfo)),m_maxNodeNum(maxNode)
{
	// 连接共享内存
	void* p = mapShm();

	// 初始化共享内存
	memset(p, 0x00, sizeof(int) + maxNode * sizeof(NodeSHMInfo));

	// 将maxNode写入共享内存的头四个字节
	memcpy(p, &m_maxNodeNum, sizeof(int));

	// 断开与共享内存的连接
	unmapShm();

}


SecKeyShm::SecKeyShm( char* pathName, int maxNode):
	ShareMemory(pathName, sizeof(int) + maxNode * sizeof(NodeSHMInfo)),m_maxNodeNum(maxNode)
{
	// 连接共享内存
	void* p = mapShm();

	// 初始化共享内存
	memset(p, 0x00, sizeof(int) + maxNode * sizeof(NodeSHMInfo));

	// 将maxNode写入共享内存的头四个字节
	memcpy(p, &m_maxNodeNum, sizeof(int));

	// 断开与共享内存的连接
	unmapShm();
}

int SecKeyShm::shmWrite(NodeSHMInfo* pNodeInfo)
{
	//连接共享内存
	void* p = mapShm();

	//从头4个字节获取maxNode
	memcpy(&m_maxNodeNum, p, sizeof(int));
	cout << "maxNode:" << m_maxNodeNum << endl;

	// 定义指向数组第5个字节的位置
	NodeSHMInfo* pNode = (NodeSHMInfo*)((char*)p + sizeof(int));

	// 先查找原有的,更新密钥
	int i = 0;
	for (i=0; i < m_maxNodeNum; i++)
	{
		if (strcmp(pNodeInfo->clientID, pNode[i].clientID) == 0 &&
			strcmp(pNodeInfo->serverID, pNode[i].serverID) == 0)
		{
			memcpy(&pNode[i], pNodeInfo, sizeof(NodeSHMInfo));
			break;
		}
	}
	// 如果没有找到原有的，则找一个空闲可用的
	NodeSHMInfo tmp;
	memset(&tmp, 0x00, sizeof(NodeSHMInfo));
	// 如果
	if (i == m_maxNodeNum)
	{
		for (i = 0; i < m_maxNodeNum; i++)
		{
			if (memcmp(&pNode[i], &tmp, sizeof(NodeSHMInfo)) == 0)
			{
				memcpy(&pNode[i], pNodeInfo, sizeof(NodeSHMInfo));
				break;
			}
		}
	}

	// 断开
	unmapShm();

	if (i == m_maxNodeNum)
	{
		cout << "no space to use" << endl;
		return -1;
	}


	return 0;
}

int SecKeyShm::shmRead(char* clientID, char* serverID, NodeSHMInfo* pNodeSHMInfo)
{
	//连接共享内存
	void* p = mapShm();

	//从头4个字节获取maxNode
	memcpy(&m_maxNodeNum, p, sizeof(int));
	cout << "maxNode:" << m_maxNodeNum << endl;

	NodeSHMInfo* pNode = (NodeSHMInfo*)((char*)p + sizeof(int));

	int i = 0;
	for (i = 0; i < m_maxNodeNum; i++)
	{
		if (strcmp(clientID, pNode[i].clientID) == 0 &&
			strcmp(serverID, pNode[i].serverID) == 0)
		{
			memcpy(pNodeSHMInfo, &pNode[i], sizeof(NodeSHMInfo));
			break;
		}
	}

	//断开与共享内存的关联
	unmapShm();

	//没有找到节点
	if (i == m_maxNodeNum)
	{
		cout << "not found" << endl;
		return -1;
	}

	return 0;
}



void SecKeyShm::printShm()
{
	//连接共享内存
	void* p = mapShm();

	//从头4个字节获取maxNode
	memcpy(&m_maxNodeNum, p, sizeof(int));
	cout << "maxNode:" << m_maxNodeNum << endl;

	NodeSHMInfo* pNode = (NodeSHMInfo*)((char*)p + sizeof(int));

	int i = 0;
	for (i = 0; i < m_maxNodeNum; i++)
	{
		cout << "-------" << i << "-------" << endl;
		cout << "status:" << pNode[i].status << endl;
		cout << "seckeyID:" << pNode[i].seckeyID << endl;
		cout << "clientID:" << pNode[i].clientID << endl;
		cout << "serverID:" << pNode[i].serverID << endl;
		cout << "seckey:" << pNode[i].seckey << endl;
	}

	return;
}