#pragma once 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

const char RandX = 'x';

class ShareMemory
{
public:
	// ͨ��keyֵ��ȡ�����ڴ�ID
	ShareMemory(int key);
	// ͨ��·����ȡ�����ڴ�ID
	ShareMemory(const char* pathName);
	// ͨ��keyֵ��ʼ�������ڴ�
	ShareMemory(int key, int shmSize);
	// ͨ��·����ʼ�������ڴ�
	ShareMemory(const char* pathName, int shmSize);

	void* mapShm();

	int unmapShm();

	int delShm();

	~ShareMemory();

private:
	int getShmId(key_t key, int shmSize, int flag);

private:
	int m_shmID;
	void* m_shmAddr;
};