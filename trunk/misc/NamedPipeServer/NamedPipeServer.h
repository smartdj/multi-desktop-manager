#include <string>
#include <windows.h>
#ifndef _NAMEDPIPEWRAPPER_H_
#include "NamedPipeWrapper.h"
#endif
#include <strsafe.h>
#define BUFSIZE  1024
class NamedPipeServer
{
private:
	std::string pipeName;
	
public:
	NamedPipeServer(std::string pipename)
	{
		this->pipeName = pipename;
	}
	void Listen()
	{
		// The main loop creates an instance of the named pipe and 
		// then waits for a client to connect to it. When the client 
		// connects, a thread is created to handle communications 
		// with that client, and this loop is free to wait for the
		// next client connect request. It is an infinite loop.
		 
		for (;;) 
		{ 
			unsigned long dwThreadId;
			
			HANDLE hPipe = INVALID_HANDLE_VALUE;
			printf( "\nPipe Server: Main thread awaiting client connection on %s\n", pipeName.c_str());
			hPipe = CreateNamedPipeA(
			  pipeName.c_str(),             // pipe name 
			  PIPE_ACCESS_DUPLEX,       // read/write access 
			  PIPE_TYPE_MESSAGE |       // message type pipe 
			  PIPE_READMODE_MESSAGE |   // message-read mode 
			  PIPE_WAIT,                // blocking mode 
			  PIPE_UNLIMITED_INSTANCES, // max. instances  
			  BUFSIZE,                  // output buffer size 
			  BUFSIZE,                  // input buffer size 
			  0,                        // client time-out 
			  NULL);                    // default security attribute 

			if (hPipe == INVALID_HANDLE_VALUE) 
			{
				printf("CreateNamedPipe failed, GLE=%d.\n", GetLastError()); 
				continue;
			}

			// Wait for the client to connect; if it succeeds, 
			// the function returns a nonzero value. If the function
			// returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 

			BOOL fConnected = ConnectNamedPipe(hPipe, NULL) ?  TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 
			HANDLE hThread = NULL;
			if (fConnected) 
			{ 
				printf("Client connected, creating a processing thread.\n"); 

				// Create a thread for this client. 
				hThread = CreateThread( 
				NULL,              // no security attribute 
				0,                 // default stack size 
				(LPTHREAD_START_ROUTINE)InstanceThread,    // thread proc
				(LPVOID) hPipe,    // thread parameter 
				0,                 // not suspended 
				&dwThreadId);      // returns thread ID 

				if (hThread == NULL) 
				{
					printf("CreateThread failed, GLE=%d.\n", GetLastError()); 
					continue ;
				}
				else 
				{
					CloseHandle(hThread); 
				}
			} 
			else 
			{
				// The client could not connect, so close the pipe. 
				CloseHandle(hPipe); 
			}
		} 
	}
	static int WINAPI InstanceThread(LPVOID lpvParam)
	{
		// This routine is a thread processing function to read from and reply to a client
		// via the open pipe connection passed from the main loop. Note this allows
		// the main loop to continue executing, potentially creating more threads of
		// of this procedure to run concurrently, depending on the number of incoming
		// client connections.

		int cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0; 
		BOOL fSuccess = FALSE;
		HANDLE hPipe  = NULL;

		// Do some extra error checking since the app will keep running even if this
		// thread fails.

		if (lpvParam == NULL)
		{
			printf( "\nERROR - Pipe Server Failure:\n");
			printf( "   InstanceThread got an unexpected NULL value in lpvParam.\n");
			printf( "   InstanceThread exitting.\n");
			return (DWORD)-1;
		}

		// Print verbose messages. In production code, this should be for debugging only.
		printf("InstanceThread created, receiving and processing messages.\n");

		// The thread's parameter is a handle to a pipe object instance. 

		hPipe = (HANDLE) lpvParam; 
		//Read Write.
		NamedPipeWrapper wrapper = NamedPipeWrapper(hPipe);
		std::string msg = wrapper.Read();

		wrapper.Write(msg);
		// Flush the pipe to allow the client to read the pipe's contents 
		// before disconnecting. Then disconnect the pipe, and close the 
		// handle to this pipe instance. 

		FlushFileBuffers(hPipe); 
		DisconnectNamedPipe(hPipe); 
		CloseHandle(hPipe); 

		printf("InstanceThread exitting.\n");
	}
};