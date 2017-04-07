#include "stdafx.h"

#include "RawSocket.h"
#ifdef _UNICODE
#include <winnls.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//*****************************************************************
void CWizSyncSocket::Init(SOCKET h)
{
	m_hSocket = h;
}
//*****************************************************************
void CWizSyncSocket::Close()
{
	if(m_hSocket == INVALID_SOCKET)
		return;
	::closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
}
//*****************************************************************
int CWizSyncSocket::SetIntOption(int level, int optname, int val)
{
	return ::setsockopt(m_hSocket, level, optname, (char *)&val, sizeof(val));
}
//*****************************************************************
BOOL CWizSyncSocket::InitializeSocket(int nPort,int nType)
{
	// Socket must be created with socket()
	ASSERT(m_hSocket != INVALID_SOCKET);
	// Make up address
	SOCKADDR_IN	SockAddr;
	memset(&SockAddr,0, sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = INADDR_ANY;
	SockAddr.sin_port   = ::htons((u_short)nPort);

	// Bind to the address and port
	if(Bind((SOCKADDR*)&SockAddr))
	{
		SetIntOption(SOL_SOCKET, SO_DONTLINGER,1);

		if(nType == SOCK_STREAM)
		{
			SetIntOption(SOL_SOCKET, SO_KEEPALIVE,1);
			if (Listen(5))
				return TRUE;
		}
		else
			return TRUE;
	}
	
	return FALSE;
}
//*****************************************************************
BOOL CWizSyncSocket::Create(int nPort,int nType)
{
	// creates a socket
	if(nType == SOCK_DGRAM)
	{
		m_hSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
		if (m_hSocket == INVALID_SOCKET)
			return FALSE;
		
		if (!InitializeSocket(nPort,nType))
		{
			Close();
			return FALSE;
		}
	}

	if(nType == SOCK_STREAM)
	{
		m_hSocket = ::socket(PF_INET, nType, 0);
		if (m_hSocket == INVALID_SOCKET)
			return FALSE;

		if(nPort)
		{
			// Bind to the port
			if (!InitializeSocket(nPort,nType))
			{
				Close();
				return FALSE;
			}
		}
	}
	return TRUE;
}
//*****************************************************************
// Create an invalid socket
CWizSyncSocket::CWizSyncSocket(SOCKET h /* = INVALID_SOCKET */)
{
	Init(h);
}
//*****************************************************************
// Create a listening socket
CWizSyncSocket::CWizSyncSocket(int nPort,int nType)
{
	Init(INVALID_SOCKET);
	if(!Create(nPort,nType))
		throw XCannotCreate();
}
//*****************************************************************
CWizSyncSocket::~CWizSyncSocket()
{
	Close();
}
//*****************************************************************
// Waits for pending connections on the port, 
// creates a new socket with the same properties as this 
// and returns a handle to the new socket
SOCKET CWizSyncSocket::Accept()
{
	SOCKET h = ::accept(m_hSocket, NULL, NULL);
	return h;
}

SOCKET CWizSyncSocket::Accept(LPSOCKADDR psa)
{
	int nLengthAddr = sizeof(SOCKADDR_IN);
	SOCKET h = ::accept(m_hSocket, psa, &nLengthAddr);
	return h;
}

BOOL CWizSyncSocket::Bind(LPSOCKADDR psa)
{
	if(m_hSocket==INVALID_SOCKET)		
		return FALSE;

	if(::bind(m_hSocket, psa, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) 
	{
		return FALSE;
	}
	return TRUE;
}

//*****************************************************************
// Cerates a socket and establishes a connection to a peer
// on lpszHostAddress:nHostPort 
BOOL CWizSyncSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort )
{
	ASSERT(lpszHostAddress != NULL);
	// Create ? socket
	if (m_hSocket == INVALID_SOCKET)
	{
		m_hSocket = ::socket(PF_INET, SOCK_STREAM, 0);
		if (m_hSocket == INVALID_SOCKET)
			return FALSE;
	}

	// Fill address machinery of sockets.
	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	LPSTR lpszAscii = (LPSTR)lpszHostAddress;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszAscii);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}
	sockAddr.sin_port = htons((u_short)nHostPort);
	// connects to peer
	int r = ::connect(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	if (r != SOCKET_ERROR)
	{
		ASSERT(r == 0);
		return TRUE;
	}

	int e = ::WSAGetLastError();
	ASSERT(e != WSAEWOULDBLOCK);

	return FALSE;
}

BOOL CWizSyncSocket::Listen(int queue_length)
{
	if(m_hSocket==INVALID_SOCKET)	
		return FALSE;
	if(listen(m_hSocket, queue_length) == SOCKET_ERROR) 
	{
		return FALSE;
	}
	return TRUE;
}

//*****************************************************************
CWizReadWriteSocket::CWizReadWriteSocket(int nPort,int nType)
: CWizSyncSocket(nPort,nType) 
{

}

CWizReadWriteSocket::CWizReadWriteSocket(SOCKET h /* = INVALID_SOCKET */)
{
	Init(h);
}

CWizReadWriteSocket::~CWizReadWriteSocket()
{
	Close();
}

//超时是-1,断开是0,出错是-2
int CWizReadWriteSocket::Recv(char* pch, const int nSize, const int nSecs)
{
	if(m_hSocket==INVALID_SOCKET)		return 0;
	
	if(nSecs>=0)
	{
		FD_SET fd = {1, m_hSocket};
		TIMEVAL tv = {nSecs, 0};
		if(select(0, &fd, NULL, NULL, &tv) == 0) 
		{
			return -1;
		}
	}
	
	int nBytesRead;
	if((nBytesRead = recv(m_hSocket, pch, nSize, 0))==SOCKET_ERROR) 
	{
		return -2;
	}
	return nBytesRead;
}

//超时是-1,断开是0,出错是-2
int CWizReadWriteSocket::RecvFrom(char* pch, const int nSize, 
		const int nSecs,LPSOCKADDR psa,int * fromlen)
{
	if(m_hSocket==INVALID_SOCKET)		return 0;
	
	if(nSecs>=0)
	{
		FD_SET fd = {1, m_hSocket};
		TIMEVAL tv = {nSecs, 0};
		if(select(0, &fd, NULL, NULL, &tv) == 0) 
		{
			return -1;
		}
	}
	
	int nBytesRead;
	if((nBytesRead = recvfrom(m_hSocket, pch, nSize, 0, psa, fromlen))==SOCKET_ERROR) 
	{
		return -2;
	}
	return nBytesRead;
}

int	CWizReadWriteSocket::Read(char* pch, const int nSize, const int nSecs) 
{
	int nBytesRead = 0;
	int nBytesThisTime;
	char* pch1 = pch;

	do 
	{
		nBytesThisTime = Recv(pch1, nSize - nBytesRead, nSecs);
		if(nBytesThisTime<=0)
		{
			return nBytesThisTime;
		}
		nBytesRead += nBytesThisTime;
		pch1 += nBytesThisTime;
	} while(nBytesRead < nSize);

	return nBytesRead;
}

//*****************************************************************
int CWizReadWriteSocket::Send(char* pch, const int nSize, const int nSecs)
{
	if(m_hSocket==INVALID_SOCKET)		return 0;
	// returned value will be less than nSize if client cancels the reading
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, NULL, &fd, NULL, &tv) == 0) 
	{
		return -1;
	}
	int nBytesSend;
	if((nBytesSend = send(m_hSocket, pch, nSize, 0))==SOCKET_ERROR) 
	{
		return -2;
	}
	return nBytesSend;
}

int CWizReadWriteSocket::SendTo(char* pch, const int nSize, 
		const int nSecs,LPSOCKADDR psa,int tolen)
{
	if(m_hSocket==INVALID_SOCKET)		return 0;
	// returned value will be less than nSize if client cancels the reading
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, NULL, &fd, NULL, &tv) == 0) 
	{
		return -1;
	}

	int nBytesSend;
	if((nBytesSend = sendto(m_hSocket, pch, nSize, 0, psa, tolen))==SOCKET_ERROR) 
	{
		return -2;
	}
	return nBytesSend;
}

int CWizReadWriteSocket::SendTo_CE(char* pch, const int nSize, 
		const int nSecs,LPSOCKADDR psa,int tolen)
{
	int nBytesSend;

	if(nSize>200)
	{
		nBytesSend = SendTo(pch, 200, nSecs, psa, tolen);

		if(nBytesSend != 200)
			return -2;

		nBytesSend = SendTo(pch+200 , nSize-200, nSecs, psa, tolen);

		if(nBytesSend != nSize-200)
			return -2;
	
		return nBytesSend;
	}
	else
		return SendTo(pch, nSize, nSecs, psa, tolen);
}

// write raw data
int	CWizReadWriteSocket::Write(char* pch, const int nSize, const int nSecs) 
{
	int nBytesSend = 0;
	int nBytesThisTime;
	char* pch1 = pch;

	do 
	{
		nBytesThisTime = Send(pch1, nSize-nBytesSend, nSecs);
		if(nBytesThisTime<=0)
		{
			int err = WSAGetLastError();
			return nBytesThisTime;
		}
		nBytesSend += nBytesThisTime;
		pch1 += nBytesThisTime;
	} while(nBytesSend < nSize);

	return nBytesSend;	
}
//*****************************************************************
// Reads UNICODE string from socket.
// Converts string from ANSI to UNICODE.
int CWizReadWriteSocket::ReadString(WCHAR* lpszString, int nMaxLen)
{
	// read string length
	u_long nt_Len;
	if (Read((char *)&nt_Len, sizeof(nt_Len),3) < sizeof(nt_Len))
		return 0;
	int Len = ntohl(nt_Len);
	if (Len == 0 || Len >= nMaxLen)
		return 0;

	static const int BUFF_SIZE = 2000;
	if (Len >= BUFF_SIZE)
		return 0;
	char buff[BUFF_SIZE];
	// Read ANSI string to the buffer
	if (Read(buff, Len,3) <= 0)
		return 0;
	buff[Len] = 0;

	// Convert ANSI string to the UNICODE
	VERIFY(::MultiByteToWideChar(CP_ACP, 0, buff, Len, lpszString, nMaxLen*sizeof(lpszString[0])));
	return Len;
}
//*****************************************************************
// Reads ANSI string from socket.
int CWizReadWriteSocket::ReadString(char* lpszString, int nMaxLen)
{
	// read string length
	u_long nt_Len;
	if (Read((char *)&nt_Len, sizeof(nt_Len),3) < sizeof(nt_Len))
		return 0;
	int Len = ntohl(nt_Len);
	if (Len == 0 || Len >= nMaxLen)
		return 0;

	// Read ANSI string
	if (Read(lpszString, Len,3) <= 0)
		return 0;
	lpszString[Len] = 0;
	return Len;
}
//*****************************************************************
inline int Length(const char* p)
{
	return strlen(p);
}
//*****************************************************************
inline int Length(const WCHAR* p)
{
	return wcslen(p);
}
//*****************************************************************
// Writes UNICODE string to socket,
// converts UNICODE string to ANSI.
BOOL CWizReadWriteSocket::WriteString (const WCHAR*  lpszString, int nLen /* = -1*/)
{
	if (nLen < 0)
		nLen = Length(lpszString);
	static const int BUFF_SIZE = 2000;
	if (nLen >= BUFF_SIZE*sizeof(lpszString) + sizeof(u_long))
		return FALSE;
	char buff[BUFF_SIZE];
	u_long nt_Len = htonl(nLen);
	int nSize = sizeof(nt_Len);
	memcpy(buff, &nt_Len, nSize);
	// To make one call less, the length of the string
	// copied to the buffer before the string itself
	// and the buffer sent once.
	char* ptr = buff + nSize;
	if (nLen > 0)
		{
		// Convert ANSI to UNICODE
		int s = WideCharToMultiByte(CP_ACP, 0, lpszString, nLen, ptr, BUFF_SIZE - sizeof(u_long), NULL, NULL);
		ASSERT(s > 0);
		nSize += s;
		}
	return Write(buff, nSize, 3);
}
//*****************************************************************
// Writes ANSI string to socket.
BOOL CWizReadWriteSocket::WriteString (const char* lpszString, int nLen /* = -1*/)
{
	if (nLen < 0)
		nLen = strlen(lpszString);
	static const int BUFF_SIZE = 2000;
	if (nLen >= BUFF_SIZE*sizeof(lpszString) + sizeof(u_long))
		return FALSE;
	char buff[BUFF_SIZE];
	u_long nt_Len = htonl(nLen);
	int nSize = sizeof(nt_Len);
	memcpy(buff, &nt_Len, nSize);
	// To make one call less, the length of the string
	// copied to the buffer before the string itself
	// and the buffer sent once.
	char* ptr = buff + nSize;
	if (nLen > 0)
	{
		memcpy(ptr, lpszString, nLen);
		nSize += nLen;
	}
	return Write(buff, nSize, 3);
}
//*****************************************************************

BOOL CWizSyncSocket::GetHostByName(SOCKADDR_IN& sock,
								const char* pchName,
								const USHORT ushPort /* = 0 */)
{
	hostent* pHostEnt = gethostbyname(pchName);
	if(pHostEnt != NULL)
	{
		ULONG* pulAddr = (ULONG*) pHostEnt->h_addr_list[0];
		sock.sin_family = AF_INET;
		sock.sin_port = htons(ushPort);
		sock.sin_addr.s_addr = *pulAddr; // address is already in network byte order
		return TRUE;
	}
	return FALSE;	
}

BOOL CWizSyncSocket::GetHostName(LPTSTR lpszAddress, size_t nAddrSize, UINT& rSocketPort)
{
	if (nAddrSize < 1)
		return FALSE;

	*lpszAddress = TCHAR(0);
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	int r;

	while ((r = ::getsockname(m_hSocket, (SOCKADDR*)&sockAddr, &nSockAddrLen)) == SOCKET_ERROR)
		{
		const int e = ::WSAGetLastError();
		if (e != WSAEINPROGRESS)
			return FALSE;
		}

	ASSERT(r == 0);
	rSocketPort = ::ntohs(sockAddr.sin_port);

	char    szName[64];
	struct  hostent *h;
	DWORD	dwMyAddress;

	int r1;
	while((r1 = ::gethostname(szName,sizeof(szName))) == SOCKET_ERROR)
		{
		const int e = ::WSAGetLastError();
		if (e != WSAEINPROGRESS)
			return FALSE;
		}

	ASSERT(r1 == 0);
	h = (struct hostent *) ::gethostbyname(szName);
	memcpy(&dwMyAddress,h->h_addr_list[0],sizeof(DWORD));
	if (dwMyAddress == INADDR_NONE)
		return FALSE;
	struct   in_addr     tAddr;
	memcpy(&tAddr,&dwMyAddress,sizeof(DWORD));
	char    *ptr = ::inet_ntoa(tAddr);

#ifdef _UNICODE
	return ::MultiByteToWideChar(CP_ACP, 0, ptr, -1, lpszAddress, nAddrSize*sizeof(lpszAddress[0]));
#else
	if (size_t(lstrlen(ptr)) >= nAddrSize)
		return FALSE;
	lstrcpy(lpszAddress, ptr);
#endif

	return TRUE;
}
//*****************************************************************
BOOL CWizSyncSocket::GetPeerName(LPTSTR lpszAddress, size_t nAddrSize, UINT& rPeerPort)
{
	if (nAddrSize < 1)
		return FALSE;
	*lpszAddress = TCHAR(0);
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	int r;
	while ((r = ::getpeername(m_hSocket, (SOCKADDR*)&sockAddr, &nSockAddrLen)) == SOCKET_ERROR)
	{
		const int e = ::WSAGetLastError();
		if (e != WSAEINPROGRESS)
			return FALSE;
	}

	ASSERT(r == 0);
	rPeerPort = ntohs(sockAddr.sin_port);
	char * pAddr = inet_ntoa(sockAddr.sin_addr);
	int len = strlen(pAddr);
#ifdef _UNICODE
	char buff[100];
	if (len >= 100 || len >= int(nAddrSize))
		return FALSE;
	memcpy(buff, pAddr,100);
	return ::MultiByteToWideChar(CP_ACP, 0, buff, len, lpszAddress, nAddrSize*sizeof(lpszAddress[0]));
#else
	if (size_t(len) >= nAddrSize)
		return FALSE;
	memcpy(lpszAddress, pAddr,len + 1);
#endif
	return TRUE;
}
