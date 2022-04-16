#include "ClientOperation.h"
#include <string.h>
#include "RequestCodec.h"
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <iostream>
#include <CodecFactory.h>
#include <Codec.h>
#include <RequestFactory.h>
#include <RequestCodec.h>
#include <RespondCodec.h>
#include <RespondFactory.h>

using namespace std;

ClientOperation::ClientOperation(ClientInfo* info)
{
	// 
	memcpy(&m_info, info, sizeof(ClientInfo));

	// 创建共享内存
	m_shm = new SecKeyShm(m_info.shmKey, m_info.maxNode);
}

ClientOperation::~ClientOperation()
{
}

int ClientOperation::secKeyAgree()
{
	
	// 准备要发送的数据，并赋值
	RequestMsg req;
	memset(&req, 0x00, sizeof(RequestMsg));
	req.cmdType = RequestCodec::NewOrUpdate;
	strcpy(req.clientId, m_info.clinetID);
	strcpy(req.serverId, m_info.serverID);

	getRandString(sizeof(req.r1), req.r1);
	// 使用hamc生成哈希值 --- 消息认证码
	char key[64];
	unsigned int len;
	unsigned char md[SHA256_DIGEST_LENGTH];
	memset(key, 0x00, sizeof(key));
	sprintf(key, "@%s+%s@", req.serverId, req.clientId);
	HMAC(EVP_sha256(), key, strlen(key), (unsigned char*)req.r1, strlen(req.r1), md, &len);

	// 将哈希值转换为16进制，并保存到消息验证码中
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		sprintf(&req.authCode[2 * i], "%02x", md[i]);   //  md用authCode传输到服务端
	}
	cout << "key:" << key << endl;
	cout << "r1:" << req.r1 << endl;
	cout << "authCode:" << req.authCode << endl;

	// 将要发送的消息进行编码
	int dataLen;
	char* outData = NULL;
	CodecFactory* factory = new RequestFactory(&req);
	Codec* pCodec = factory->createCodec();
	pCodec->msgEncode(&outData, dataLen);
	delete factory;
	delete pCodec;

	// 连接服务器
	m_socket.connectToHost(m_info.serverIP, m_info.serverPort); 
	// 发送消息给服务端
	m_socket.sendMsg(outData, dataLen);
	// 等待接受服务端的应答
	char* inData;
	m_socket.recvMsg(&inData, dataLen);

	// 解码
	factory = new RespondFactory();
	pCodec = factory->createCodec();
	RespondMsg* pMsg = (RespondMsg*)pCodec->msgDecode(inData, dataLen);
	
	// 判断服务端是否成功
	if (pMsg->rv == -1)
	{
		cout << "密钥协商失败" << endl;
		return -1;
	}
	else
	{
		cout << "密钥协商成功" << endl;
	}

	// 将服务端的r2和客户端的r1拼接生成密钥
	char buf[1024];
	unsigned char md1[SHA_DIGEST_LENGTH];
	memset(md1, 0x00, sizeof(md1));
	char seckey[SHA_DIGEST_LENGTH * 2 + 1];
	memset(buf, 0x00, sizeof(buf));
	memset(seckey, 0x00, sizeof(seckey));
	sprintf(buf, "%s%s", req.r1, pMsg->r2);
	SHA1((unsigned char*)buf, strlen((char*)buf), md1);
	for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
	{
		sprintf(&seckey[i * 2], "%02x", md1[i]);
	}
	cout << "秘钥: " << seckey << endl;

	// 给密钥结构体赋值
	NodeSHMInfo node;
	memset(&node, 0x00, sizeof(NodeSHMInfo));
	node.status = 0;
	strcpy(node.seckey, seckey);
	strcpy(node.clientID, m_info.clinetID);
	strcpy(node.serverID, m_info.serverID);
	node.seckeyID = pMsg->seckeyid;

	//将秘钥信息写入共享内存
	m_shm->shmWrite(&node);

	//关闭网络连接
	m_socket.disConnect();

	//释放资源
	delete factory;
	delete pCodec;
	free(inData);
	return 0;


}

int ClientOperation::secKeyCheck()
{
	return 0;
}

int ClientOperation::secKeyRevoke()
{
	return 0;
}

int ClientOperation::secKeyView()
{
	return 0;
}

void ClientOperation::getRandString(int len, char* randBuf)
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
			randBuf[i] = rand() % 26 + 'A';
			break;
		case 1:
			randBuf[i] = rand() % 26 + 'a';
			break;
		case 2:
			randBuf[i] = rand() % 10 + '0';
			break;
		case 3:
			randBuf[i] = chars[rand() % strlen(chars)];
			break;
		default:
			break;
		}
	}
	randBuf[len - 1] = '\0';
}
