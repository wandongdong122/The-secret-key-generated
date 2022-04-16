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
//	int status;   // 当前秘钥状态 
//	int seckeyID; // 秘钥ID
//	char clientID[12]; // 客户端ID
//	char serverID[12]; // 服务端ID
//	char seckey[128]; // 秘钥
//};

class MYSQLOP
{
public:
	MYSQLOP();
	MYSQLOP(const char* IP, const char* user, const char* passwd, const char* db);
				
	~MYSQLOP();
	// m_mysql, m_IP, m_user, m_passwd, m_db, 0, NULL, 0
	// 初始化环境，连接数据库
	bool connectDB(const char* IP,const char* user,const char* passwd, const char* db);
	//得到keyID
	int getKeyID();
	// 更新秘钥ID
	bool updataKeyID(int keyID);
	// 写秘钥ID
	bool writeSeckey(NodeSHMInfo *pNode);
	// 关闭数据库
	void closeDB();
private:
	// 获取当前时间，并格式化为字符串
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