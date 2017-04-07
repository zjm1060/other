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

#define Q_ARG_PAIR(TYPE, T1, T2, data) QArgument<TYPE<T1, T2> >(#TYPE"<"#T1", "#T2">", data)

#define Q_ASSERT_NO_NULL(p) Q_ASSERT(p!=NULL)

#define CONVERT_TO_VARIANT(p)	\
{	\
	QVariant value;		\
	value.setValue(p);	\
	return value;		\
}

#define SWAP_4_BYTES(bytes) ((bytes&0xff)<<24)+((bytes&0xff00)<<8)+((bytes&0xff0000)>>8)+((bytes&0xff000000)>>24)

#ifdef Q_OS_WIN
#pragma comment(lib,"Ws2_32.lib")
#define _inet_aton(str, addr) (addr)->s_addr=inet_addr(str)
#else
typedef int SOCKET;
#define _inet_aton(str, addr) inet_aton(str, addr)
#define closesocket(fd) close(fd)
#endif

#ifdef Q_OS_SOLARIS
#define __FUNCTION__ ""
#endif

#define DATA_STREAM_VERSION QDataStream::Qt_4_3

#define Q_WRITE_ONLY_DATA_STREAM(stream, buffer)	\
	QByteArray buffer;	\
	QDataStream stream(&buffer, QIODevice::WriteOnly);	\
	stream.setVersion(DATA_STREAM_VERSION);

#define Q_WRITE_ONLY_DATA_STREAM_DEFINED(stream, buffer)	\
	QDataStream stream(&buffer, QIODevice::WriteOnly|QIODevice::Append);	\
	stream.setVersion(DATA_STREAM_VERSION);

#define Q_IO_DEVICE_DATA_STREAM(stream, device)	\
	QDataStream stream(&device);	\
	stream.setVersion(DATA_STREAM_VERSION);

#define Q_READ_ONLY_DATA_STREAM(stream, buffer)	\
	QDataStream stream(buffer);	\
	stream.setVersion(DATA_STREAM_VERSION);

#define MATH_PI	3.1415

#define PROTOCAL_HEADER "IWHR"
#define PROTOCAL_HEAT_BEAT_V1	0x010000
#define PROTOCAL_HEAT_BEAT_VER	0x010000
#define PROTOCAL_VER	0x010000

#define INI_FILE	"/ini/manager.ini"

#define TEMP_FILE_SUFFIX	".!syn"

#define TRANSFER_SETTING_FILE	"/ini/transfer.xml"
#define CONFIGURE_SETTING_FILE	"/ini/configure.xml"

#define H9000_DEF_PATH			"def"
#define H9000_DSP_PATH			"dsp"
#define H9000_DATABASE_PATH		"gen"
#define H9000_DAT_PATH			"dat"
#define H9000_TMP_PATH			"tmp"
#define H9000_IEC104_PATH		"iec104"
#define H9000_VOICE_PATH		"wav"
#define H9000_DATABASE_FILE		"DATABASE.OUT"

#define H9000_FUNCTION_UPDATE_DATABASE_FILE		"UpdateDatabaseFile"

#endif	//DEFINE_H
