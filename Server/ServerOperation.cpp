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
	// 给服务端数据赋值 
	memcpy(&m_info, info, sizeof(ServerInfo));
	// 创建共享内存
	m_shm = new SecKeyShm(m_info.shmkey, m_info.maxnode);

	// 连接服务器
	m_mysql.connectDB(info->dbIP,info->dbUse,info->dbPasswd,info->dbSID);


}

ServerOperation::~ServerOperation()
{
}


void ServerOperation::startWork()
{
	//	cout << "server Start work " <<endl;
		// 包括创建socket、设端口复用、绑定、监听
	m_server.setListen(m_info.sPort);
	// 创建线程-----高并发
	pthread_t threadID;
	while (1)
	{
		if (m_stop)  // 信息触发
		{
			break;
		}

		//		cout << "create pthread" <<endl; 
				// accept新的客户端链接
		m_client = m_server.acceptConn();

		// 创建子线程
		pthread_create(&threadID, NULL, working, this);
		// 设置线程为分离属性
		pthread_detach(threadID);

		// 将线程ID与通信socket进行绑定
		m_listSocket.insert(make_pair(threadID, m_client));
	}
	m_mysql.closeDB();
}

int ServerOperation::secKeyAgree(RequestMsg* reqMsg, char** outData, int& outLen)
{
	// 验证消息认证码
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
	//将生成的消息认证码和客户端的r1的消息认证做比对
	if (strcmp(authCode, reqMsg->authCode) != 0)
	{
		//		cout << "authCode error" << endl;
		return -1;
	}

	//生成随机字符串r2
	RespondMsg rspMsg;
	memset(&rspMsg, 0x00, sizeof(rspMsg));
	getRandString(sizeof(rspMsg.r2), rspMsg.r2);

	// 将r1与r2拼接生成秘钥
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

	//给应答结构体赋值
	rspMsg.seckeyid = 1;  //获取秘钥ID
	rspMsg.rv = 0;
	strcpy(rspMsg.serverId, m_info.serverID);
	strcpy(rspMsg.clientId, reqMsg->clientId);

	//将要发送给客户端的应答结构体进行编码
	int dataLen;
	char* sendData = NULL;
	CodecFactory* factory = new RespondFactory(&rspMsg);
	Codec* pCodec = factory->createCodec();
	pCodec->msgEncode(&sendData, dataLen);
	delete factory;
	//	delete pCodec;

	*outData = sendData;
	outLen = dataLen;


	//写秘钥信息到共享内存
	NodeSHMInfo node;
	memset(&node, 0x00, sizeof(NodeSHMInfo));
	node.status = 0;
	strcpy(node.seckey, seckey);
	strcpy(node.clientID, rspMsg.clientId);
	strcpy(node.serverID, m_info.serverID);
	node.seckeyID = rspMsg.seckeyid;

	//将秘钥信息写入共享内存
	m_shm->shmWrite(&node);

	// 写秘钥信息到数据库
	m_mysql.writeSeckey(&node);

	//关闭连接
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
	// 设置随机种子
	srand(time(NULL));
	// 随机字符串: A-Z, a-z, 0-9, 特殊字符(!@#$%^&*()_+=)
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
	// 线程ID
	pthread_t threadID = pthread_self();
	// 通过map容器找到与线程ID对应的socket
	TcpSocket* socket = op->m_listSocket[threadID];
	//	cout << "socket = " << socket <<endl;
		// 接收客户端发来的数据
	char* inData;
	int dataLen;
	socket->recvMsg(&inData, dataLen);

	// 解码
	CodecFactory* factory = new RequestFactory();
	Codec* pCode = factory->createCodec();
	RequestMsg* pMsg = (RequestMsg*)pCode->msgDecode(inData, dataLen);
	delete factory;
	// 判断客户端ID是否合法

	// 判断客户端要请求什么服务
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
