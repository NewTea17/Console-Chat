#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

#pragma warning(disable: 4996)

SOCKET Connection;

void ClientHandler();

int main(int argc, char* argv[])
{
	WSAData wsaData;
	WORD dllVersion = MAKEWORD(2, 1);

	if (WSAStartup(dllVersion, &wsaData) != 0)
	{
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int size = sizeof(addr);

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		std::cout << "Error: Cann't connect to server" << std::endl;
		return 1;
	}

	std::cout << "Connected!" << std::endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	std::string message;
	while (true)
	{
		std::getline(std::cin, message);
		int msgSize = message.size();

		send(Connection, (char*)&msgSize, sizeof(int), NULL);

		send(Connection, message.c_str(), msgSize, NULL);
		Sleep(10);
	}

	system("pause");

	return 0;
}

void ClientHandler()
{
	int msgSize;
	while (true)
	{
		recv(Connection, (char*)&msgSize, sizeof(int), NULL);

		char* msg = new char[msgSize + 1];
		msg[msgSize] = '\0';

		recv(Connection, msg, msgSize, NULL);

		std::cout << msg << std::endl;

		delete[] msg;
	}
}
