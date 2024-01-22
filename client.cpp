#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "Packet.h"

using namespace std;

#pragma comment(lib, "ws2_32")


int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(ServerSockAddr.sin_addr.s_addr));
	ServerSockAddr.sin_port = htons(5001);

	connect(ServerSocket, (struct sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr));

	while (true)
	{
		char Buffer[1024] = { 0, };
		int RecvByte = recv(ServerSocket, Buffer, 4, MSG_WAITALL);
		if (RecvByte <= 0)
		{
			break;
		}
		Header PacketHeader;
		memcpy(&PacketHeader, Buffer, sizeof(Header));
		Data Packet;


		
		///////////////////Calculate////////////////////////

		if (PacketHeader.PacketType == (unsigned short)EPacketType::Calculate)
		{
			char* DataBuffer = new char[PacketHeader.Length];
			int RecvByte = recv(ServerSocket, DataBuffer, PacketHeader.Length, MSG_WAITALL);
			if (RecvByte <= 0)
			{
				break;
			}

			memcpy(&Packet, DataBuffer, sizeof(Packet));


			//memcpy(&FirstNumber, &Buffer[0], sizeof(int));
			//memcpy(&SecondNumber, &Buffer[4], sizeof(int));
			//Operator = Buffer[8];
			long long Result = 0;

			cout << Packet.FirstNumber << " ";


			switch (Packet.Operator)
			{
			case 0:
				Result = Packet.FirstNumber + Packet.SecondNumber;
				cout << " + ";
				break;
			case 1:
				Result = Packet.FirstNumber - Packet.SecondNumber;
				cout << " - ";
				break;
			case 2:
				Result = Packet.FirstNumber * Packet.SecondNumber;
				cout << " * ";
				break;
			case 3:
				Result = Packet.FirstNumber / Packet.SecondNumber;
				cout << " / ";
				break;
			case 4:
				Result = Packet.FirstNumber % Packet.SecondNumber;
				cout << " % ";
				break;
			default:
				Result = Packet.FirstNumber + Packet.SecondNumber;
				cout << " + ";
				break;
			}
			cout << Packet.SecondNumber << " = ";

			cout << Result << endl;

			char Message[8] = { 0, };
			memcpy(Message, &Result, sizeof(Result));

			send(ServerSocket, Message, (u_int)sizeof(Message), 0);
			delete[] DataBuffer;
		}
		else if (PacketHeader.PacketType == (unsigned short)EPacketType::Image)
		{
			FILE* OutputFile = fopen("chichichi.jpg", "wb");

			char* FileBuffer = new char[PacketHeader.Length];



			char Buffer[1024] = { 0, };

			int RecvByte = recv(ServerSocket, FileBuffer, PacketHeader.Length, MSG_WAITALL);
			if (RecvByte <= 0)
			{
				break;
			}
			size_t WriteSize = fwrite(FileBuffer, sizeof(char), RecvByte, OutputFile);


			fclose(OutputFile);
			delete[] FileBuffer;
		}

		

	}
	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}