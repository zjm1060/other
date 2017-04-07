///////////////////////////////////////////////////////////////////
// Header File RawSocket.h
// class CWizSyncSocket & CWizReadWriteSocket
//
// 23/07/1996 14:54                             Author: Poul A. Costinsky
///////////////////////////////////////////////////////////////////

#ifndef __CWizRawSocket_H
#define __CWizRawSocket_H

/////////////////////////////////////////////////////////////////////////////

class CSockAddr : public sockaddr_in 
{
public:
	// constructors
	CSockAddr()
		{ sin_family = AF_INET;
		  sin_port = 0;
		  sin_addr.s_addr = 0; } // Default
	CSockAddr(const SOCKADDR& sa) { memcpy(this, &sa, sizeof(SOCKADDR)); }
	CSockAddr(const SOCKADDR_IN& sin) { memcpy(this, &sin, sizeof(SOCKADDR_IN)); }
	CSockAddr(const ULONG ulAddr, const USHORT ushPort = 0) // parms are host byte ordered
		{ sin_family = AF_INET;
		  sin_port = htons(ushPort);
	      sin_addr.s_addr = htonl(ulAddr); }
	CSockAddr(const char* pchIP, const USHORT ushPort = 0) // dotted IP addr string
		{ sin_family = AF_INET;
		  sin_port = htons(ushPort);
		  sin_addr.s_addr = inet_addr(pchIP); } // already network byte ordered
	// Return the address in dotted-decimal format
	CString DottedDecimal()
		{ return inet_ntoa(sin_addr); } // constructs a new CString object
	// Get port and address (even though they're public)
	USHORT Port() const
		{ return ntohs(sin_port); }
	ULONG IPAddr() const
		{ return ntohl(sin_addr.s_addr); }
	// operators added for efficiency
	const CSockAddr& operator=(const SOCKADDR& sa)
		{ memcpy(this, &sa, sizeof(SOCKADDR));
		  return *this; }
	const CSockAddr& operator=(const SOCKADDR_IN& sin)
		{ memcpy(this, &sin, sizeof(SOCKADDR_IN));
		  return *this; }
	operator SOCKADDR()
		{ return *((LPSOCKADDR) this); }
	operator LPSOCKADDR()
		{ return (LPSOCKADDR) this; }
	operator LPSOCKADDR_IN()
		{ return (LPSOCKADDR_IN) this; }
};

// class CWizSyncSocket
// Simple encapsulation of the SOCKET handle and 
// simple operations with it.
class CWizSyncSocket
{
public:
	SOCKET	m_hSocket;			// socket handle

	// Exception class if createsocket fails in constructor 
	struct XCannotCreate {};
	// Constructors
	CWizSyncSocket(int nPort,int nType);	// Connects for listening 
											// on the port nPort
	CWizSyncSocket(SOCKET h = INVALID_SOCKET); 
								
	// Destructor
	~CWizSyncSocket();

	BOOL	Create(int nPort,int nType);	// Creates listening socket
	void	Close();			// Closes socket
	SOCKET	Accept();			// waits to accept connection
								// and returns a descriptor 
								// for the accepted packet
	SOCKET	Accept(LPSOCKADDR psa);
	BOOL	Bind(LPSOCKADDR psa);
	BOOL	Connect(LPCTSTR lpszHostAddress, UINT nHostPort );
	BOOL	Listen(int queue_length);
							// establishes a connection to a peer
	SOCKET	H() const { return m_hSocket; }
	// returns a handle of the socket
	
	BOOL GetHostByName(SOCKADDR_IN& sock,const char* pchName, 
		const USHORT ushPort = 0);
	BOOL GetHostName(LPTSTR lpszAddress, size_t nAddrSize, UINT& rSocketPort);
	BOOL GetPeerName(LPTSTR lpszAddress, size_t nAddrSize, UINT& rPeerPort);
	
protected:
	void Init(SOCKET h);		// initialize data members
	BOOL InitializeSocket(int nPort,int nType); 
								// associates a local address 
								// with a socket and establishes
								// a socket to listen for incoming connection
	int	 SetIntOption(int level, int optname, int val);
								// sets a socket option
};

/////////////////////////////////////////////////////////////////////////////
// class CWizReadWriteSocket
// Class provides synchronous I/O for the socket
class CWizReadWriteSocket : public CWizSyncSocket
{	
	public:
		struct X {};
		struct XRead	: public X {};
		struct XWrite	: public X {};
	public:
		// constructor usually receives a handle from Accept
		CWizReadWriteSocket(
			int nPort, int nType);
		CWizReadWriteSocket(SOCKET h);
		~CWizReadWriteSocket();

		int Recv(char* pch, const int nSize, const int nSecs);
		int RecvFrom(char* pch, const int nSize, const int nSecs,LPSOCKADDR psa,int* fromlen);
		int	Read(char* pch, const int nSize, const int nSecs); 
		int Send(char* pch, const int nSize, const int nSecs);
		int SendTo(char* pch, const int nSize, const int nSecs,LPSOCKADDR psa,int tolen);
		int SendTo_CE(char* pch, const int nSize, const int nSecs,LPSOCKADDR psa,int tolen);
		int	Write(char* pch, const int nSize, const int nSecs);
		// Read/write strings. nLen is in characters,
		// not bytes.
		BOOL ReadString  (char*  lpszString, int nLen);
		BOOL ReadString  (WCHAR* lpszString, int nLen);
		// If nLen == -1, the actual length is calculated
		// assuming lpszString is zero-terminated.
		BOOL WriteString (const char* lpszString, int nLen = -1);
		BOOL WriteString (const WCHAR* lpszString, int nLen = -1);
};

/////////////////////////////////////////////////////////////////////////////
template<class T>
BOOL	RawRead(CWizReadWriteSocket& rSocket, T& data)
{
	return (rSocket.Read (&data, sizeof(T)) == sizeof(T));
}

template<class T>
BOOL	RawWrite(CWizReadWriteSocket& rSocket, const T& data)
{
	return (rSocket.Write (&data, sizeof(T)) == sizeof(T));
}

/////////////////////////////////////////////////////////////////////////////
template<class T>
void	RawReadX(CWizReadWriteSocket& rSocket, T& data)
{
	if (!RawRead(rSocket, data))
		throw CWizReadWriteSocket::XRead();
}

template<class T>
void	RawWriteX(CWizReadWriteSocket& rSocket, const T& data)
{
	if (!RawWrite(rSocket, data))
		throw CWizReadWriteSocket::XWrite();
}

/////////////////////////////////////////////////////////////////////////////
#endif // __CWizRawSocket_H


