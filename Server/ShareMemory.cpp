#include "ShareMemory.h"

ShareMemory::ShareMemory(int key):m_shmAddr(NULL)
{
	m_shmID = getShmId(key, 0, 0);
}

ShareMemory::ShareMemory(const char* pathName) : m_shmAddr(NULL)
{
	key_t key = ftok(pathName, RandX);
	m_shmID = getShmId(key, 0, 0);
}

ShareMemory::ShareMemory(int key, int shmSize) :m_shmAddr(NULL)
{
	m_shmID = getShmId(key, shmSize, IPC_CREAT | 0755);
}

ShareMemory::ShareMemory(const char* pathName, int shmSize) : m_shmAddr(NULL)
{
	key_t key = ftok(pathName, RandX);
	m_shmID = getShmId(key, shmSize, IPC_CREAT | 0755);
}

void* ShareMemory::mapShm()
{
	m_shmAddr = shmat(m_shmID, NULL, 0);
	if (m_shmAddr == (void*)-1)
	{
		return NULL;
	}
	return m_shmAddr;
}

int ShareMemory::unmapShm()
{
	int ret = shmdt(m_shmAddr);
	return ret;
}

int ShareMemory::delShm()
{
	int ret = shmctl(m_shmID, IPC_RMID, NULL);
	return ret;
}

ShareMemory::~ShareMemory()
{
}

int ShareMemory::getShmId(key_t key, int shmSize, int flag)
{
	m_shmID = shmget(key, shmSize, flag);
	if (m_shmID < 0)
	{
		return -1;
	}
	return m_shmID;
}
