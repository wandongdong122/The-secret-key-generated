#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "mysql/mysql.h"
#include "SecKeyShm.h"
#include <string>
#include <string.h>
using namespace std;


//class NodeSHMInfo
//{
//public:
//	int status;   // ��ǰ��Կ״̬ 
//	int seckeyID; // ��ԿID
//	char clientID[12]; // �ͻ���ID
//	char serverID[12]; // �����ID
//	char seckey[128]; // ��Կ
//};

class MYSQLOP
{
public:
	MYSQLOP();
	MYSQLOP(const char* IP, const char* user, const char* passwd, const char* db);
				
	~MYSQLOP();
	// m_mysql, m_IP, m_user, m_passwd, m_db, 0, NULL, 0
	// ��ʼ���������������ݿ�
	bool connectDB(const char* IP,const char* user,const char* passwd, const char* db);
	//�õ�keyID
	int getKeyID();
	// ������ԿID
	bool updataKeyID(int keyID);
	// д��ԿID
	bool writeSeckey(NodeSHMInfo *pNode);
	// �ر����ݿ�
	void closeDB();
private:
	// ��ȡ��ǰʱ�䣬����ʽ��Ϊ�ַ���
	string getCurTime();
private:
	//Envi
	MYSQL* m_mysql;
	MYSQL* m_conn;
	const char* m_IP;
	const char* m_user;
	const char* m_passwd;
	const char* m_db;
	NodeSHMInfo m_node;
};