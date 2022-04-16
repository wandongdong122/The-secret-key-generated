#include<iostream>
#include<string>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdio.h>
using namespace std;

int getRandString(char *pRandBuf,int len)
{
	srand(time(NULL));
	int flag;
	char buf[] = "!@#$%^&*()_+-=";
	for (int i = 0; i < len-1; i++)
	{
		flag = rand() % 4;
		switch (flag)
		{
		case 0:
			pRandBuf[i] = rand() % 10 + '0';
			break;
		case 1:
			pRandBuf[i] = rand() % 26 + 'A';
			break;
		case 2:
			pRandBuf[i] = rand() % 26 + 'a';
			break;
		case 3:
			pRandBuf[i] = buf[rand()%strlen(buf)];
			break;
		default:
			break;
		}
	}
}


/*
int main()
{
	char sRandBuf[64];
	int len = sizeof(sRandBuf);

	while (1)
	{
		memset(sRandBuf, 0x00, len);
		getRandString(sRandBuf, len);
		cout << sRandBuf << endl;
		getchar();
	}
	return 0;
}
*/