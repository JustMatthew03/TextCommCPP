//
// Put code into a Visual Studio C++ Console Application
// Use PuTTY or alternative telnet application
//

#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	//Init Winsock.
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		cerr << "Can not initialize winsock! quitting." << endl;
		return;
	}

	//Create winsock.
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cerr << "Can not create socket! quitting." << endl;
		return;
	}

	//Bind socket to IP and Port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//allocate for listening
	listen(listening, SOMAXCONN);

	fd_set master;
	FD_ZERO(&master);
	FD_SET(listening, &master);

	while (true)
	{
		fd_set copy = master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET sock = copy.fd_array[i];
			if (sock == listening)
			{
				//accept connection
				SOCKET client = accept(listening, nullptr, nullptr);
				FD_SET(client, &master);

				string welcomeMsg = "Connected to the server.";
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
			}
			else
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				//receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					//drop client
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else
				{
					//send message

					for (int i = 0; i < master.fd_count; i++) {
						SOCKET outSock = master.fd_array[i];
						if (outSock = master.fd_array[i])
						{
							send(outSock, buf, bytesIn, 0);
						}
					}
				}
			}
		}
	}

	//shutdown winsock
	WSACleanup();
}
