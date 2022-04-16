#include "ServerOperation.h"
#include "RequestFactory.h"

using namespace std;

bool ServerOperation::m_stop = false;
void ServerOperation::catchSignal(int num)
{
	m_stop = true;
}

ServerOperation::ServerOperation(ServerInfo* info)
{
	// ����������ݸ�ֵ 
	memcpy(&m_info, info, sizeof(ServerInfo));
	// ���������ڴ�
	m_shm = new SecKeyShm(m_info.shmkey, m_info.maxnode);

	// ���ӷ�����
	m_mysql.connectDB(info->dbIP,info->dbUse,info->dbPasswd,info->dbSID);


}

ServerOperation::~ServerOperation()
{
}


void ServerOperation::startWork()
{
	//	cout << "server Start work " <<endl;
		// ��������socket����˿ڸ��á��󶨡�����
	m_server.setListen(m_info.sPort);
	// �����߳�-----�߲���
	pthread_t threadID;
	while (1)
	{
		if (m_stop)  // ��Ϣ����
		{
			break;
		}

		//		cout << "create pthread" <<endl; 
				// accept�µĿͻ�������
		m_client = m_server.acceptConn();

		// �������߳�
		pthread_create(&threadID, NULL, working, this);
		// �����߳�Ϊ��������
		pthread_detach(threadID);

		// ���߳�ID��ͨ��socket���а�
		m_listSocket.insert(make_pair(threadID, m_client));
	}
	m_mysql.closeDB();
}

int ServerOperation::secKeyAgree(RequestMsg* reqMsg, char** outData, int& outLen)
{
	// ��֤��Ϣ��֤��
	char key[1024];
	unsigned int len;
	unsigned char md[SHA256_DIGEST_LENGTH];
	char authCode[SHA256_DIGEST_LENGTH * 2 + 1] = { 0 };

	memset(key, 0x00, sizeof(key));
	sprintf(key, "@%s+%s@", reqMsg->serverId, reqMsg->clientId);
	HMAC(EVP_sha256(), key, strlen(key), (unsigned char*)reqMsg->r1, strlen(reqMsg->r1), md, &len);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		sprintf(&authCode[2 * i], "%02x", md[i]);
	}
	//�����ɵ���Ϣ��֤��Ϳͻ��˵�r1����Ϣ��֤���ȶ�
	if (strcmp(authCode, reqMsg->authCode) != 0)
	{
		//		cout << "authCode error" << endl;
		return -1;
	}

	//��������ַ���r2
	RespondMsg rspMsg;
	memset(&rspMsg, 0x00, sizeof(rspMsg));
	getRandString(sizeof(rspMsg.r2), rspMsg.r2);

	// ��r1��r2ƴ��������Կ
	char buf[64];
	unsigned char md1[SHA_DIGEST_LENGTH];
	char seckey[SHA_DIGEST_LENGTH * 2 + 1];
	memset(buf, 0x00, sizeof(buf));
	memset(seckey, 0x00, sizeof(seckey));
	sprintf(buf, "%s%s", reqMsg->r1, rspMsg.r2);
	SHA1((unsigned char*)buf, strlen((char*)buf), md1);

	for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
	{
		sprintf(&seckey[i * 2], "%02x", md1[i]);
	}

	cout << "seckey: " << seckey << endl;

	//��Ӧ��ṹ�帳ֵ
	rspMsg.seckeyid = 1;  //��ȡ��ԿID
	rspMsg.rv = 0;
	strcpy(rspMsg.serverId, m_info.serverID);
	strcpy(rspMsg.clientId, reqMsg->clientId);

	//��Ҫ���͸��ͻ��˵�Ӧ��ṹ����б���
	int dataLen;
	char* sendData = NULL;
	CodecFactory* factory = new RespondFactory(&rspMsg);
	Codec* pCodec = factory->createCodec();
	pCodec->msgEncode(&sendData, dataLen);
	delete factory;
	//	delete pCodec;

	*outData = sendData;
	outLen = dataLen;


	//д��Կ��Ϣ�������ڴ�
	NodeSHMInfo node;
	memset(&node, 0x00, sizeof(NodeSHMInfo));
	node.status = 0;
	strcpy(node.seckey, seckey);
	strcpy(node.clientID, rspMsg.clientId);
	strcpy(node.serverID, m_info.serverID);
	node.seckeyID = rspMsg.seckeyid;

	//����Կ��Ϣд�빲���ڴ�
	m_shm->shmWrite(&node);

	// д��Կ��Ϣ�����ݿ�
	m_mysql.writeSeckey(&node);

	//�ر�����
//	socket->disConnect();

	return 0;
}

int ServerOperation::secKeyCheck()
{
	return 0;
}

int ServerOperation::secKeyRevoke()
{
	return 0;
}

int ServerOperation::secKeyView()
{
	return 0;
}

void* ServerOperation::wrokingHard(void* arg)
{
	return nullptr;
}


void ServerOperation::getRandString(int len, char* randBuf)
{
	int flag = -1;
	// �����������
	srand(time(NULL));
	// ����ַ���: A-Z, a-z, 0-9, �����ַ�(!@#$%^&*()_+=)
	char chars[] = "!@#$%^&*()_+=";
	for (int i = 0; i < len - 1; ++i)
	{
		flag = rand() % 4;
		switch (flag)
		{
		case 0:
			randBuf[i] = 'Z' - rand() % 26;
			break;
		case 1:
			randBuf[i] = 'z' - rand() % 26;
			break;
		case 3:
			randBuf[i] = rand() % 10 + '0';
			break;
		case 2:
			randBuf[i] = chars[rand() % strlen(chars)];
			break;
		default:
			break;
		}
	}
	randBuf[len - 1] = '\0';
}

void* working(void* arg)
{
	ServerOperation* op = (ServerOperation*)arg;
	// �߳�ID
	pthread_t threadID = pthread_self();
	// ͨ��map�����ҵ����߳�ID��Ӧ��socket
	TcpSocket* socket = op->m_listSocket[threadID];
	//	cout << "socket = " << socket <<endl;
		// ���տͻ��˷���������
	char* inData;
	int dataLen;
	socket->recvMsg(&inData, dataLen);

	// ����
	CodecFactory* factory = new RequestFactory();
	Codec* pCode = factory->createCodec();
	RequestMsg* pMsg = (RequestMsg*)pCode->msgDecode(inData, dataLen);
	delete factory;
	// �жϿͻ���ID�Ƿ�Ϸ�

	// �жϿͻ���Ҫ����ʲô����
	char* outData;

	switch (pMsg->cmdType)
	{
	case RequestCodec::NewOrUpdate:
		//		cout << "client cmd is seckeyAgree<<endl" <<endl;
		op->secKeyAgree(pMsg, &outData, dataLen);
		break;

	case RequestCodec::Check:
		op->secKeyCheck();
		break;
	case RequestCodec::Revoke:
		op->secKeyRevoke();
		break;
	case RequestCodec::View:
		op->secKeyView();
		break;

	default:
		break;
	}
	socket->sendMsg(outData, dataLen);
	free(outData);
	op->m_listSocket.erase(threadID);
	socket->disConnect();
	return nullptr;
}
