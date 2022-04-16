#include "RequestCodec.h"
#include <iostream>
using namespace std;

//�����ʱ��ʹ��
RequestCodec::RequestCodec() : Codec()  // �����б��ʼ������
{

}

//�����ʱ�����
RequestCodec::RequestCodec(RequestMsg * msg)
{
	// ��ֵ����
	memcpy(&m_msg, msg, sizeof(RequestMsg));
}

RequestCodec::~RequestCodec()
{
}

/*
	struct RequestMsg
	{
		//1 ��ԿЭ��  	//2 ��ԿУ��; 	// 3 ��Կע��
		int		cmdType;		// ��������
		char	clientId[12];	// �ͻ��˱��
		char	authCode[65];	// ��֤��
		char	serverId[12];	// �������˱�� 
		char	r1[64];			// �ͻ��������
	};
*/


int RequestCodec::msgEncode(char ** outData, int & len)
{
	writeHeadNode(m_msg.cmdType);
	writeNextNode(m_msg.clientId, strlen(m_msg.clientId)+1);
	writeNextNode(m_msg.authCode, strlen(m_msg.authCode) + 1);
	writeNextNode(m_msg.serverId, strlen(m_msg.serverId) + 1);
	writeNextNode(m_msg.r1, strlen(m_msg.r1) + 1);
	packSequence(outData, len);

	return 0;
}

// ����void*��Ϊ���Ժ��һ����չ����������ṹ������
void * RequestCodec::msgDecode(char * inData, int inLen)
{
	//�����л�
	unpackSequence(inData, inLen);
	readHeadNode(m_msg.cmdType);
	readNextNode(m_msg.clientId);
	readNextNode(m_msg.authCode);
	readNextNode(m_msg.serverId);
	readNextNode(m_msg.r1);

	cout << "����ɹ�" << endl;
	return &m_msg;
}
