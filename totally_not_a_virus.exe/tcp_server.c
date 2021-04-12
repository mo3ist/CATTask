#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)	// Disable warnings for depricated stuff

// NOTE: change this to what suits you better, it's the maximum user input length.
#define MAX_BUFFER 8

int error() {
	printf("Failed with error code %d.\n", WSAGetLastError());
	ExitProcess(EXIT_FAILURE);
}

int main(int argc, char** argv) {
	WSADATA wsa;
	SOCKET server_sock, client_sock;
	struct sockaddr_in server_sa, client_sa;
	int sizeof_client_sa;
	const char* IP = "127.0.0.1";
	const int PORT = 420;
	const int MAX_CONN = 1;
	char recv_buffer[MAX_BUFFER];
	int sizeof_recv_buffer;
	int sizeof_sent;
	char* message;

	// HINT
	puts("Hi there :) 'scanf' is overrated, have you ever heard of 'netcat'?");

	// Initializing the library
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return error();

	// Creating the TCP socket
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == INVALID_SOCKET)
		return error();

	// Binding socket to IP and PORT
	server_sa.sin_addr.s_addr = inet_addr(IP);
	server_sa.sin_family = AF_INET;
	server_sa.sin_port = htons(PORT);

	if (bind(server_sock, (struct sockaddr*)&server_sa, sizeof(server_sa)) == SOCKET_ERROR)
		return error();

	// Listen for MAX_CONN connections
	listen(server_sock, MAX_CONN);

	// Accept the connection
	sizeof_client_sa = sizeof(client_sa);
	client_sock = accept(server_sock, (struct sockaddr*)&client_sa, &sizeof_client_sa);
	if (client_sock == INVALID_SOCKET)
		return error();


	// Close the server socket
	closesocket(server_sock);

	// Send a message
	message = "Good job :D\n";
	sizeof_sent = send(client_sock, message, strlen(message), 0);
	if (sizeof_sent == SOCKET_ERROR)
		return error();

	do
	{
		sizeof_recv_buffer = recv(client_sock, recv_buffer, MAX_BUFFER, 0);
		if (sizeof_recv_buffer == SOCKET_ERROR)
			return error();

		recv_buffer[sizeof_recv_buffer - 1] = '\0';
		
		// NOTE: 'recv_buffer' is the user input. 'sizeof_recv_buffer' includes the null bit.

	} 	while (sizeof_recv_buffer > 0);
	
	closesocket(client_sock);
	puts("Goodbye! Press any key to exit...");
	getchar();
	ExitProcess(EXIT_SUCCESS);
}