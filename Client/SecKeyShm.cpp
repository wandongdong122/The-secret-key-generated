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
	// ���ӹ����ڴ�
	void* p = mapShm();

	// ��ʼ�������ڴ�
	memset(p, 0x00, sizeof(int) + maxNode * sizeof(NodeSHMInfo));

	// ��maxNodeд�빲���ڴ��ͷ�ĸ��ֽ�
	memcpy(p, &m_maxNodeNum, sizeof(int));

	// �Ͽ��빲���ڴ������
	unmapShm();

}


SecKeyShm::SecKeyShm( char* pathName, int maxNode):
	ShareMemory(pathName, sizeof(int) + maxNode * sizeof(NodeSHMInfo)),m_maxNodeNum(maxNode)
{
	// ���ӹ����ڴ�
	void* p = mapShm();

	// ��ʼ�������ڴ�
	memset(p, 0x00, sizeof(int) + maxNode * sizeof(NodeSHMInfo));

	// ��maxNodeд�빲���ڴ��ͷ�ĸ��ֽ�
	memcpy(p, &m_maxNodeNum, sizeof(int));

	// �Ͽ��빲���ڴ������
	unmapShm();
}

int SecKeyShm::shmWrite(NodeSHMInfo* pNodeInfo)
{
	//���ӹ����ڴ�
	void* p = mapShm();

	//��ͷ4���ֽڻ�ȡmaxNode
	memcpy(&m_maxNodeNum, p, sizeof(int));
	cout << "maxNode:" << m_maxNodeNum << endl;

	// ����ָ�������5���ֽڵ�λ��
	NodeSHMInfo* pNode = (NodeSHMInfo*)((char*)p + sizeof(int));

	// �Ȳ���ԭ�е�,������Կ
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
	// ���û���ҵ�ԭ�еģ�����һ�����п��õ�
	NodeSHMInfo tmp;
	memset(&tmp, 0x00, sizeof(NodeSHMInfo));
	// ���
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

	// �Ͽ�
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
	//���ӹ����ڴ�
	void* p = mapShm();

	//��ͷ4���ֽڻ�ȡmaxNode
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

	//�Ͽ��빲���ڴ�Ĺ���
	unmapShm();

	//û���ҵ��ڵ�
	if (i == m_maxNodeNum)
	{
		cout << "not found" << endl;
		return -1;
	}

	return 0;
}



void SecKeyShm::printShm()
{
	//���ӹ����ڴ�
	void* p = mapShm();

	//��ͷ4���ֽڻ�ȡmaxNode
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