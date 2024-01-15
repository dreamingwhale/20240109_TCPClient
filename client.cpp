#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	HOSTENT* ServerInfo;
	ServerInfo = gethostbyname("www.naver.com");
	if (ServerInfo)
	{
		if (ServerInfo->h_addrtype == AF_INET)
		{
			cout << ServerInfo->h_name << endl;
			cout << ServerInfo->h_aliases << endl;
			int i = 0;
			IN_ADDR IP;
			while (ServerInfo->h_addr_list[i] != 0)
			{
				//4자리씩 IP의 주소를 잘라서 저장
				IP.s_addr = *(u_long*)ServerInfo->h_addr_list[i];
				//u_long temp = *(u_long*)ServerInfo->h_addr_list[i];
				printf("ip: %s\n", inet_ntoa(IP));
				i++;
			}
			/*
			for (int i = 0; i < ServerInfo->h_length; i++)
			{
				IP.s_addr = *(u_long*)ServerInfo->h_addr_list[i];
				printf("ip: %s\n", inet_ntoa(IP));
			}
			*/
		}
	}
	char IPAddress[] = "223.100.100.101";
	ServerInfo = gethostbyaddr(IPAddress, strlen(IPAddress), AF_INET);
	if (ServerInfo)
	{
		if (ServerInfo->h_addrtype == AF_INET)
		{
			cout << ServerInfo->h_name << endl;
		}
	}


	WSACleanup();


	return 0;
}