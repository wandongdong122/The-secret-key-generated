#include <iostream>
#include "ServerOperation.h"
#include <string.h>

using namespace std;

int main()
{
	ServerInfo info;
	strcpy(info.serverID, "0001");
	info.maxnode = 10;
	info.shmkey = 0x12340000;
	strcpy(info.dbUse, "root");
	strcpy(info.dbPasswd, "123456");
	strcpy(info.dbSID, "SECMNG");
	info.sPort = 9898;

	ServerOperation server(&info);
	server.startWork();
	
}