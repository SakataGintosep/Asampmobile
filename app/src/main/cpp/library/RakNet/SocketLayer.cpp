/**
* @file
* @brief SocketLayer class implementation 
*
 * This file is part of RakNet Copyright 2003 Rakkarsoft LLC and Kevin Jenkins.
 *
 * Usage of Raknet is subject to the appropriate licence agreement.
 * "Shareware" Licensees with Rakkarsoft LLC are subject to the
 * shareware license found at
 * http://www.rakkarsoft.com/shareWareLicense.html which you agreed to
 * upon purchase of a "Shareware license" "Commercial" Licensees with
 * Rakkarsoft LLC are subject to the commercial license found at
 * http://www.rakkarsoft.com/sourceCodeLicense.html which you agreed
 * to upon purchase of a "Commercial license"
 * Custom license users are subject to the terms therein.
 * All other users are
 * subject to the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * Refer to the appropriate license agreement for distribution,
 * modification, and warranty rights.
*/
#include "SocketLayer.h"
#include <assert.h>
#include "MTUSize.h"

//#include "main.h"
#include "SAMP/samp_netencr.h"

#ifdef _WIN32
#include <process.h>
#define COMPATIBILITY_2_RECV_FROM_FLAGS 0
typedef int socklen_t;
#elif defined(_COMPATIBILITY_2)
#include "Compatibility2Includes.h"
#else
#define COMPATIBILITY_2_RECV_FROM_FLAGS 0
#include <string.h> // memcpy
#include <unistd.h>
#include <fcntl.h>
#endif

#include "ExtendedOverlappedPool.h"
#ifdef __USE_IO_COMPLETION_PORTS
#include "AsynchronousFileIO.h"
#endif

#ifdef _MSC_VER
#pragma warning( push )
#endif

bool SocketLayer::socketLayerStarted = false;
#ifdef _WIN32
WSADATA SocketLayer::winsockInfo;
#endif
SocketLayer SocketLayer::I;

#ifdef _WIN32
extern void __stdcall ProcessNetworkPacket( const unsigned int binaryAddress, const unsigned short port, const char *data, const int length, RakPeer *rakPeer );
#else
extern void ProcessNetworkPacket(const unsigned int binaryAddress, const unsigned short port,
		const char* data, const int length, RakPeer* rakPeer);
#endif

#ifdef _WIN32
extern void __stdcall ProcessPortUnreachable( const unsigned int binaryAddress, const unsigned short port, RakPeer *rakPeer );
#else
extern void ProcessPortUnreachable(const unsigned int binaryAddress, const unsigned short port,
		RakPeer* rakPeer);
#endif

#ifdef _DEBUG
#include <stdio.h>
#endif

SocketLayer::SocketLayer()
{
	if (socketLayerStarted == false) {
#ifdef _WIN32

		if ( WSAStartup( MAKEWORD( 2, 2 ), &winsockInfo ) != 0 )
		{
#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
			DWORD dwIOError = GetLastError();
			LPVOID messageBuffer;
			FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
				( LPTSTR ) & messageBuffer, 0, NULL );
			// something has gone wrong here...
			printf( "WSAStartup failed:Error code - %d\n%s", dwIOError, messageBuffer );
			//Free the buffer.
			LocalFree( messageBuffer );
#endif
		}

#endif
		socketLayerStarted = true;
	}
}

SocketLayer::~SocketLayer()
{
	if (socketLayerStarted == true) {
#ifdef _WIN32
		WSACleanup();
#endif

		socketLayerStarted = false;
	}
}

SOCKET SocketLayer::Connect(SOCKET writeSocket, unsigned int binaryAddress, unsigned short port)
{
	assert(writeSocket != INVALID_SOCKET);
	sockaddr_in connectSocketAddress;

	connectSocketAddress.sin_family = AF_INET;
	connectSocketAddress.sin_port = htons(port);
	connectSocketAddress.sin_addr.s_addr = binaryAddress;

	if (connect(writeSocket, (struct sockaddr*) &connectSocketAddress, sizeof(struct sockaddr))
			!= 0) {
#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
		DWORD dwIOError = GetLastError();
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) &messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "WSAConnect failed:Error code - %d\n%s", dwIOError, messageBuffer );
		//Free the buffer.
		LocalFree( messageBuffer );
#endif
	}

	return writeSocket;
}

#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
SOCKET SocketLayer::CreateBoundSocket(unsigned short port, bool blockingSocket,
		const char* forceHostAddress)
{
	SOCKET listenSocket;
	sockaddr_in listenerSocketAddress;
	int ret;

#ifdef __USE_IO_COMPLETION_PORTS

	if ( blockingSocket == false ) 
		listenSocket = WSASocket( AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	else
#endif

	listenSocket = socket(AF_INET, SOCK_DGRAM, 0);

	if (listenSocket == INVALID_SOCKET) {
#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
		DWORD dwIOError = GetLastError();
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) & messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "socket(...) failed:Error code - %d\n%s", dwIOError, messageBuffer );
		//Free the buffer.
		LocalFree( messageBuffer );
#endif

		return INVALID_SOCKET;
	}

	int sock_opt = 1;

	if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &sock_opt, sizeof(sock_opt))
			== -1) {
#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
		DWORD dwIOError = GetLastError();
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) & messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "setsockopt(SO_REUSEADDR) failed:Error code - %d\n%s", dwIOError, messageBuffer );
		//Free the buffer.
		LocalFree( messageBuffer );
#endif
	}

	// This doubles the max throughput rate
	sock_opt = 1024 * 256;
	setsockopt(listenSocket, SOL_SOCKET, SO_RCVBUF, (char*) &sock_opt, sizeof(sock_opt));

	// This doesn't make much difference: 10% maybe
	sock_opt = 1024 * 16;
	setsockopt(listenSocket, SOL_SOCKET, SO_SNDBUF, (char*) &sock_opt, sizeof(sock_opt));

#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
	// If this assert hit you improperly linked against WSock32.h
	assert(IP_DONTFRAGMENT==14);
#endif

	// TODO - I need someone on dialup to test this with :(
	// Path MTU Detection
	/*
	if ( setsockopt( listenSocket, IPPROTO_IP, IP_DONTFRAGMENT, ( char * ) & sock_opt, sizeof ( sock_opt ) ) == -1 )
	{
#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
		DWORD dwIOError = GetLastError();
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) & messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "setsockopt(IP_DONTFRAGMENT) failed:Error code - %d\n%s", dwIOError, messageBuffer );
		//Free the buffer.
		LocalFree( messageBuffer );
#endif
	}
	*/

#ifndef _COMPATIBILITY_2
	//Set non-blocking
#ifdef _WIN32
	unsigned long nonblocking = 1;
// http://www.die.net/doc/linux/man/man7/ip.7.html
	if ( ioctlsocket( listenSocket, FIONBIO, &nonblocking ) != 0 )
	{
		assert( 0 );
		return INVALID_SOCKET;
	}
#else
	if (fcntl(listenSocket, F_SETFL, O_NONBLOCK) != 0) {
		assert(0);
		return INVALID_SOCKET;
	}
#endif
#endif

	// Set broadcast capable
	if (setsockopt(listenSocket, SOL_SOCKET, SO_BROADCAST, (char*) &sock_opt, sizeof(sock_opt))
			== -1) {
#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
		DWORD dwIOError = GetLastError();
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) & messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "setsockopt(SO_BROADCAST) failed:Error code - %d\n%s", dwIOError, messageBuffer );
		//Free the buffer.
		LocalFree( messageBuffer );
#endif

	}

	// Listen on our designated Port#
	listenerSocketAddress.sin_port = htons(port);

	// Fill in the rest of the address structure
	listenerSocketAddress.sin_family = AF_INET;

	if (forceHostAddress && forceHostAddress[0]) {
		listenerSocketAddress.sin_addr.s_addr = inet_addr(forceHostAddress);
	}
	else {
		listenerSocketAddress.sin_addr.s_addr = INADDR_ANY;
	}

	// bind our name to the socket
	ret = bind(listenSocket, (struct sockaddr*) &listenerSocketAddress, sizeof(struct sockaddr));

	if (ret == SOCKET_ERROR) {
#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
		DWORD dwIOError = GetLastError();
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) & messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "bind(...) failed:Error code - %d\n%s", dwIOError, messageBuffer );
		//Free the buffer.
		LocalFree( messageBuffer );
#endif

		return INVALID_SOCKET;
	}

	return listenSocket;
}

#if !defined(_COMPATIBILITY_1) && !defined(_COMPATIBILITY_2)
const char* SocketLayer::DomainNameToIP(const char* domainName)
{
	struct hostent* phe = gethostbyname(domainName);

	if (phe == 0 || phe->h_addr_list[0] == 0) {
		//cerr << "Yow! Bad host lookup." << endl;
		return 0;
	}

	struct in_addr addr;

	memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));

	return inet_ntoa(addr);
}
#endif

void SocketLayer::Write(const SOCKET writeSocket, const char* data, const int length)
{
#ifdef _DEBUG
	assert( writeSocket != INVALID_SOCKET );
#endif
#ifdef __USE_IO_COMPLETION_PORTS

	ExtendedOverlappedStruct* eos = ExtendedOverlappedPool::Instance()->GetPointer();
	memset( &( eos->overlapped ), 0, sizeof( OVERLAPPED ) );
	memcpy( eos->data, data, length );
	eos->length = length;

	//AsynchronousFileIO::Instance()->PostWriteCompletion(ccs);
	WriteAsynch( ( HANDLE ) writeSocket, eos );
#else

	send(writeSocket, data, length, 0);
#endif
}

// Start an asynchronous read using the specified socket.
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
bool SocketLayer::AssociateSocketWithCompletionPortAndRead(SOCKET readSocket,
		unsigned int binaryAddress, unsigned short port, RakPeer* rakPeer)
{
#ifdef __USE_IO_COMPLETION_PORTS
	assert( readSocket != INVALID_SOCKET );

	ClientContextStruct* ccs = new ClientContextStruct;
	ccs->handle = ( HANDLE ) readSocket;

	ExtendedOverlappedStruct* eos = ExtendedOverlappedPool::Instance()->GetPointer();
	memset( &( eos->overlapped ), 0, sizeof( OVERLAPPED ) );
	eos->binaryAddress = binaryAddress;
	eos->port = port;
	eos->rakPeer = rakPeer;
	eos->length = MAXIMUM_MTU_SIZE;

	bool b = AsynchronousFileIO::Instance()->AssociateSocketWithCompletionPort( readSocket, ( DWORD ) ccs );

	if ( !b )
	{
		ExtendedOverlappedPool::Instance()->ReleasePointer( eos );
		delete ccs;
		return false;
	}

	BOOL success = ReadAsynch( ( HANDLE ) readSocket, eos );

	if ( success == FALSE )
		return false;

#endif

	return true;
}

int SocketLayer::RecvFrom(const SOCKET s, RakPeer* rakPeer, int* errorCode)
{
	int len;
	char data[MAXIMUM_MTU_SIZE];
	sockaddr_in sa;

	const socklen_t len2 = sizeof(struct sockaddr_in);
	sa.sin_family = AF_INET;

#ifdef _DEBUG
	data[ 0 ] = 0;
	len = 0;
	sa.sin_addr.s_addr = 0;
#endif

	if (s == INVALID_SOCKET) {
		*errorCode = SOCKET_ERROR;
		return SOCKET_ERROR;
	}

	len = recvfrom(s, data, MAXIMUM_MTU_SIZE, COMPATIBILITY_2_RECV_FROM_FLAGS, (sockaddr*) &sa,
			(socklen_t*) &len2);

	if (len < 1 && len != -1) { // thanks to n3ptun0
		return 1;
	}

	// if (len>0)
	//  printf("Got packet on port %i\n",ntohs(sa.sin_port));

	if (len == 0) {
#ifdef _DEBUG
		printf( "Error: recvfrom returned 0 on a connectionless blocking call\non port %i.  This is a bug with Zone Alarm.  Please turn off Zone Alarm.\n", ntohs( sa.sin_port ) );
		assert( 0 );
#endif

		*errorCode = SOCKET_ERROR;
		return SOCKET_ERROR;
	}

	if (len != SOCKET_ERROR) {
#ifndef RAKSAMP_CLIENT
		if(*reinterpret_cast<DWORD *>(data) == 'PMAS')
		{
			handleQueries(s, len2, sa, data);
			return 1;
		}

		unKyretardizeDatagram((unsigned char *)data, len, iPort, 0);

#else

#endif

		unsigned short portnum;
		portnum = ntohs(sa.sin_port);
#ifndef RAKSAMP_CLIENT
		ProcessNetworkPacket( sa.sin_addr.s_addr, portnum, (char *)decrBuffer, len - 1, rakPeer );
#else
		ProcessNetworkPacket(sa.sin_addr.s_addr, portnum, data, len, rakPeer);
#endif
		return 1;
	}
	else {
		*errorCode = 0;

#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)

		DWORD dwIOError = WSAGetLastError();

		if ( dwIOError == WSAEWOULDBLOCK )
		{
			return SOCKET_ERROR;
		}
		if ( dwIOError == WSAECONNRESET )
		{
#if defined(_DEBUG)
//			printf( "A previous send operation resulted in an ICMP Port Unreachable message.\n" );
#endif


			unsigned short portnum=0;
			ProcessPortUnreachable(sa.sin_addr.s_addr, portnum, rakPeer);
			// *errorCode = dwIOError;
			return SOCKET_ERROR;
		}
		else
		{
#if defined(_DEBUG)
			if ( dwIOError != WSAEINTR )
			{
				LPVOID messageBuffer;
				FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
					( LPTSTR ) & messageBuffer, 0, NULL );
				// something has gone wrong here...
				printf( "recvfrom failed:Error code - %d\n%s", dwIOError, messageBuffer );

				//Free the buffer.
				LocalFree( messageBuffer );
			}
#endif
		}
#endif
	}

	return 0; // no data
}

#ifdef _MSC_VER
#pragma warning( disable : 4702 ) // warning C4702: unreachable code
#endif
int SocketLayer::SendTo(SOCKET s, const char* data, int length, unsigned int binaryAddress,
		unsigned short port)
{
	if (s == INVALID_SOCKET) {
		return -1;
	}

	int len;
	sockaddr_in sa;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = binaryAddress;
	sa.sin_family = AF_INET;

#ifdef RAKSAMP_CLIENT

	kyretardizeDatagram((unsigned char*) data, length, port, 0);

#endif
	do {
		// TODO - use WSASendTo which is faster.
#ifdef RAKSAMP_CLIENT
		len = sendto(s, (char*) encrBuffer, length + 1, 0, (const sockaddr*) &sa,
				sizeof(struct sockaddr_in));
#else
		len = sendto( s, (char *)data, length, 0, ( const sockaddr* ) & sa, sizeof( struct sockaddr_in ) );
#endif
	}
	while (len == 0);

	if (len != SOCKET_ERROR) {
		return 0;
	}

#if defined(_WIN32)

	DWORD dwIOError = WSAGetLastError();

	if ( dwIOError == WSAECONNRESET )
	{
#if defined(_DEBUG)
		printf( "A previous send operation resulted in an ICMP Port Unreachable message.\n" );
#endif

	}
	else if ( dwIOError != WSAEWOULDBLOCK )
	{
#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) & messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "sendto failed:Error code - %d\n%s", dwIOError, messageBuffer );

		//Free the buffer.
		LocalFree( messageBuffer );
#endif

	}

	return dwIOError;
#endif

	return 1; // error
}

int SocketLayer::SendTo(SOCKET s, const char* data, int length, char ip[16], unsigned short port)
{
	unsigned int binaryAddress;
	binaryAddress = inet_addr(ip);
	return SendTo(s, data, length, binaryAddress, port);
}

#if !defined(_COMPATIBILITY_1) && !defined(_COMPATIBILITY_2)
void SocketLayer::GetMyIP(char ipList[10][16])
{
	char ac[80];

	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
		DWORD dwIOError = GetLastError();
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) & messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "gethostname failed:Error code - %d\n%s", dwIOError, messageBuffer );
		//Free the buffer.
		LocalFree( messageBuffer );
#endif

		return;
	}

	struct hostent* phe = gethostbyname(ac);

	if (phe == 0) {
#if defined(_WIN32) && !defined(_COMPATIBILITY_1) && defined(_DEBUG)
		DWORD dwIOError = GetLastError();
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) & messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "gethostbyname failed:Error code - %d\n%s", dwIOError, messageBuffer );

		//Free the buffer.
		LocalFree( messageBuffer );
#endif

		return;
	}

	for (int i = 0; phe->h_addr_list[i] != 0 && i < 10; ++i) {

		struct in_addr addr;

		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		//cout << "Address " << i << ": " << inet_ntoa(addr) << endl;
		strcpy(ipList[i], inet_ntoa(addr));
	}
}
#endif

unsigned short SocketLayer::GetLocalPort(SOCKET s)
{
	sockaddr_in sa;
	socklen_t len = sizeof(sa);
	if (getsockname(s, (sockaddr*) &sa, &len) != 0) {
		return 0;
	}
	return ntohs(sa.sin_port);
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
