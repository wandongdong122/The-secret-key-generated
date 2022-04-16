#pragma once 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

const char RandX = 'x';

class ShareMemory
{
public:
	// 通过key值获取共享内存ID
	ShareMemory(int key);
	// 通过路径获取共享内存ID
	ShareMemory(const char* pathName);
	// 通过key值初始化共享内存
	ShareMemory(int key, int shmSize);
	// 通过路径初始化共享内存
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