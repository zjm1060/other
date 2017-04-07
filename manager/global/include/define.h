#ifndef DEFINE_H
#define DEFINE_H

#define SAFE_DELETE_CLASS(p)	\
if (p != 0)		\
{				\
	delete p;	\
	p=0;		\
}

#define  SAFE_DELETE_ARRAY(p)	\
if (p != 0)		\
{				\
	delete[] p;	\
	p=0;		\
}

#define SAFE_HIDE_WIDGET(p)	\
{	\
	if (p != NULL)	\
	{	\
		p->hide();	\
	}	\
}

#define SAFE_CLOSE_FILE(p)	\
{	\
	if (p != NULL)	\
	{	\
		p->close();	\
		SAFE_DELETE_CLASS(p)	\
	}	\
}

#define SAFE_RELEASE_COM(p)	\
{	\
	if (p !=NULL)	\
	{	\
		p->Release();	\
		p=NULL;	\
	}	\
}

#define Q_ASSERT_NO_NULL(p) Q_ASSERT(p!=NULL)

#define CONVERT_TO_VARIANT(p)	\
{	\
	QVariant value;		\
	value.setValue(p);	\
	return value;		\
}

#ifdef Q_OS_WIN
#pragma comment(lib,"Ws2_32.lib")
#define _inet_aton(str, addr) (addr)->s_addr=inet_addr(str)
#else
typedef int SOCKET;
#define _inet_aton(str, addr) inet_aton(str, addr)
#define closesocket(fd) close(fd)
#endif

#define DATA_STREAM_VERSION QDataStream::Qt_4_3

#define MATH_PI	3.1415

#endif	//DEFINE_H
