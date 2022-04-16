#include<iostream>
#include<ClientOperation.h>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>

using namespace std;
int usage();

int main()
{
	// 定义一个发送给服务端的消息
	ClientInfo info;
	memset(&info, 0x00, sizeof(info));
	strcpy(info.clinetID, "1111");
	strcpy(info.serverID, "0001");
	strcpy(info.serverIP, "172.17.27.167");
	info.maxNode = 1;
	info.serverPort = 8888;
	info.shmKey = 0x12345678;
	ClientOperation client(&info);

	int nSel;
	while (1)
	{
		nSel = usage();
		switch (nSel) {
		case 0:
			exit(0);
			break;
		case 1:
			client.secKeyAgree();
			break;
		case 2:
			client.secKeyCheck();
			break;
		case 3:
			client.secKeyRevoke();
			break;
		case 4:
			client.secKeyView();
			break;
		default:
			break;

		}
	}
}

int usage()
{
	int nSel = -1;
	printf("\n  /*************************************************************/");
	printf("\n  /*************************************************************/");
	printf("\n  /*     1.密钥协商                                            */");
	printf("\n  /*     2.密钥校验                                          */");
	printf("\n  /*     3.密钥注销                                            */");
	printf("\n  /*     4.密钥查看                                           */");
	printf("\n  /*     0.退出系统                                        */");
	printf("\n  /*************************************************************/");
	printf("\n  /*************************************************************/");
	printf("\n\n  选择:");

	scanf("%d", &nSel);
	while (getchar() != '\n');

	return nSel;
}