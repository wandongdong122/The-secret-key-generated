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
	// ��ʼ��
	m_mysql = mysql_init(NULL);
	if (m_mysql == NULL) {
		cout << "Mysql init error" << endl;
		return -1;
	}
	cout << "Mysql init OK" << endl;

	// ����Mysql���ݿ�
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
	// ��ѯ���ݿ��е�keysn�������ȡ��ԿID
	char sSQL[255] = "select ikeysn from keysn for update";
	int ret = mysql_query(m_conn, sSQL);
	if (ret != 0)
	{
		cout << "mysql_query error" << mysql_error(m_mysql) << endl;
	}
	cout << "mysql_query OK" << endl;

	// ��ȡ�����
	MYSQL_RES* results = mysql_store_result(m_conn);
	if (results == NULL)
	{
		cout << "mysql_store_result error" << mysql_error(m_mysql) << endl;
		return -1;
	}
	cout << "mysql_store_result ok" << endl;

	// �ñ�ֻ��һ����¼
	int keyID = -1;
	MYSQL_ROW row;
	if (row = mysql_fetch_row(results))
	{
		keyID = atoi(row[0]);
	}

	// �ͷŽ����
	mysql_free_result(results);
	return keyID;
}

// ��ԿID�ڲ���ʱ���Զ����£�Ҳ�����ֶ�����
bool MYSQLOP::updataKeyID(int keyID)
{

	char order[] = "update keysn set ikeysn =";
	char sSQL[255];
	sprintf(sSQL,"%s%d",order,keyID);
	// �����Զ��ύ
	//mysql_autocommit(m_mysql, true);
	// ִ�����ݸ���
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
	// ��֯�������sql���
	char sql[1024] = { 0 };
	char sdata[30];
	strcpy(sdata,getCurTime().data());
	sprintf(sql, "Insert Into seckeyinfo(clientid, serverid, keyid, createtime, state, seckey) \
					value('%s', '%s', %d, '%s' , %d, '%s')",
					pNode->clientID, pNode->serverID, this->getKeyID(),sdata, 0, pNode->seckey);
	cout << "insert sql:" << sql << endl;

	// �����Զ��ύ
	/*mysql_autocommit(m_mysql, true);*/

	// ִ�����ݸ���
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

	//�ر����ݿ�����
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
