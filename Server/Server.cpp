#include <iostream>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

#pragma warning(disable: 4996)


SOCKET Connections[100];
int Counter = 0;

void ClientHandler(int index);

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

	SOCKET listener = socket(AF_INET, SOCK_STREAM, NULL);

	bind(listener, (SOCKADDR*)&addr, sizeof(addr));

	listen(listener, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(listener, (SOCKADDR*)&addr, &size);

		if (newConnection == NULL)
		{
			std::cout << "Error #2" << std::endl;
		}
		else
		{
			std::cout << "Client connected" << std::endl;

			std::string msg = "Hello. It's my first network chat";
			int msgSize = msg.size();

			send(newConnection, (char*)&msgSize, sizeof(int), NULL);
			send(newConnection, msg.c_str(), msgSize, NULL);

			Connections[i] = newConnection;
			Counter++;

			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}


	return 0;
}

void ClientHandler(int index)
{
	int msgSize;

	while (true)
	{
		recv(Connections[index], (char*)&msgSize, sizeof(int), NULL);

		char* msg = new char[msgSize + 1];

		msg[msgSize] = '\0';
		recv(Connections[index], msg, msgSize, NULL);

		for (int i = 0; i < Counter; i++)
		{
			if (i == index)
			{
				continue;
			}

			send(Connections[i], (char*)&msgSize, sizeof(int), NULL);
			send(Connections[i], msg, msgSize, NULL);
		}

		delete[] msg;
	}
}
