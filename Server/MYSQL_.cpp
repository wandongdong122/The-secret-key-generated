#include "MYSQL_.h"



MYSQLOP::MYSQLOP()
{
}

MYSQLOP::MYSQLOP(const char* IP, const char* user, const char* passwd, const char* db)
			:m_IP(IP), m_user(user), m_passwd(passwd), m_db(db) 
{

}

MYSQLOP::~MYSQLOP()
{
}

bool MYSQLOP::connectDB(const char* IP, const char* user, const char* passwd,const char* db)
{
	// 初始化
	m_mysql = mysql_init(NULL);
	if (m_mysql == NULL) {
		cout << "Mysql init error" << endl;
		return -1;
	}
	cout << "Mysql init OK" << endl;

	// 连接Mysql数据库
	m_conn = mysql_real_connect(m_mysql, IP, user, passwd, db, 0, NULL, 0);
	if (m_conn==NULL)
	{
		cout << "mysql_real_connect error:" << mysql_error(m_mysql) << endl;
		return -1;
	}
	cout << "connect mysql OK" << endl;

	return true;
}

int MYSQLOP::getKeyID()
{
	// 查询数据库中的keysn表格来获取密钥ID
	char sSQL[255] = "select ikeysn from keysn for update";
	int ret = mysql_query(m_conn, sSQL);
	if (ret != 0)
	{
		cout << "mysql_query error" << mysql_error(m_mysql) << endl;
	}
	cout << "mysql_query OK" << endl;

	// 获取结果集
	MYSQL_RES* results = mysql_store_result(m_conn);
	if (results == NULL)
	{
		cout << "mysql_store_result error" << mysql_error(m_mysql) << endl;
		return -1;
	}
	cout << "mysql_store_result ok" << endl;

	// 该表只有一条记录
	int keyID = -1;
	MYSQL_ROW row;
	if (row = mysql_fetch_row(results))
	{
		keyID = atoi(row[0]);
	}

	// 释放结果集
	mysql_free_result(results);
	return keyID;
}

// 密钥ID在插入时会自动更新，也可以手动更新
bool MYSQLOP::updataKeyID(int keyID)
{

	char order[] = "update keysn set ikeysn =";
	char sSQL[255];
	sprintf(sSQL,"%s%d",order,keyID);
	// 设置自动提交
	//mysql_autocommit(m_mysql, true);
	// 执行数据更新
	int ret = mysql_query(m_conn, sSQL);
	if (ret != 0)
	{
		cout << "mysql_query error" << mysql_error(m_mysql) << endl;
		return false;
	}
	cout << "mysql_query OK" << endl;

	return true;
}

bool MYSQLOP::writeSeckey(NodeSHMInfo* pNode)
{
	int keyID = this->getKeyID();
	// 组织待插入的sql语句
	char sql[1024] = { 0 };
	char sdata[30];
	strcpy(sdata,getCurTime().data());
	sprintf(sql, "Insert Into seckeyinfo(clientid, serverid, keyid, createtime, state, seckey) \
					value('%s', '%s', %d, '%s' , %d, '%s')",
					pNode->clientID, pNode->serverID, this->getKeyID(),sdata, 0, pNode->seckey);
	cout << "insert sql:" << sql << endl;

	// 设置自动提交
	/*mysql_autocommit(m_mysql, true);*/

	// 执行数据更新
	int ret = mysql_query(m_conn, sql);
	if (ret != 0)
	{
		cout << "mysql_query error:" << mysql_error(m_mysql) << endl;
		return false;
	}
	cout << "mysql_query OK" << endl;
	this->updataKeyID(keyID + 1);
	return true;
}

void MYSQLOP::closeDB()
{

	//关闭数据库连接
	mysql_close(m_conn);
}

string MYSQLOP::getCurTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	return tmp;
}
