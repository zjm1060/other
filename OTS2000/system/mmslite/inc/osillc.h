//---------------------------------------------------------------------------
//
// File:         osillc.h
//
// Copyright (c) 1998,1999 SISCO, Inc. All rights reserved.
//
// Description:  Driver external definitions.
//
// Author:       A. Shajenko/Stratos Technology, Inc.
//
// History:
// 13-Sep-98     A. Shajenko     Implemented.
//
//---------------------------------------------------------------------------

#ifndef _OSILLC_H
#define _OSILLC_H

#if defined (_NTDDK_)
#include <devioctl.h>
#else
#include <winioctl.h>
#endif

////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)

//
// standard 802.3 defines (not defined elsewhere)
//
#define ETH_LENGTH_OF_HEADER		14
#define ETH_MINIMUM_LENGTH_OF_DATA	46
#define ETH_MAXIMUM_LENGTH_OF_DATA	1500
#define ETH_LENGTH_OF_FRAME		1514


// max number of packets that will be returned at a time
#define OSILLC_MAX_RXPACKETS	10

typedef struct _OSILLC_PACKET_DATA
{
	ULONG	Length;
	UCHAR	Data[ETH_LENGTH_OF_FRAME];
} OSILLC_PACKET_DATA, * POSILLC_PACKET_DATA;

typedef struct _OSILLC_PACKET_BLOCK
{
	ULONG			NumberOfPackets;
	ULONG			SeqNumber;
	OSILLC_PACKET_DATA	PacketData[OSILLC_MAX_RXPACKETS];
} OSILLC_PACKET_BLOCK, * POSILLC_PACKET_BLOCK;


typedef struct _OSILLC_OID_DATA
{
	ULONG	Oid;
	ULONG	Length;
	ULONG	BytesProcessed;
	ULONG	BytesNeeded;
	ULONG	Status;
	UCHAR	Data[1];
} OSILLC_OID_DATA, * POSILLC_OID_DATA;

typedef struct _OSILLC_GET_STATS
{
	ULONG XmitOk;
	ULONG RcvOk;
	ULONG XmitError;
	ULONG RcvError;
	ULONG RcvUnknown;
	ULONG XmitInd;
	ULONG RcvInd;
	ULONG AllocError;
} OSILLC_GET_STATS, * POSILLC_GET_STATS;

// base for IOCTLs
#define FILE_DEVICE_PROTOCOL			0x8000

#define OSILLC_CONTROL_CODE(request, method) \
		CTL_CODE(FILE_DEVICE_PROTOCOL, request, method, FILE_ANY_ACCESS)

#define IOCTL_OSILLC_QUERY_OID		OSILLC_CONTROL_CODE(0, METHOD_BUFFERED)
#define IOCTL_OSILLC_SET_OID		OSILLC_CONTROL_CODE(1, METHOD_BUFFERED)
#define IOCTL_OSILLC_GET_STATS		OSILLC_CONTROL_CODE(2, METHOD_BUFFERED)
#define IOCTL_OSILLC_GET_MACNAME	OSILLC_CONTROL_CODE(3, METHOD_BUFFERED)
#define IOCTL_OSILLC_RESET		OSILLC_CONTROL_CODE(4, METHOD_BUFFERED)

#define IOCTL_OSILLC_BIND		OSILLC_CONTROL_CODE(10, METHOD_BUFFERED)
#define IOCTL_OSILLC_UNBIND		OSILLC_CONTROL_CODE(11, METHOD_BUFFERED)
#define IOCTL_OSILLC_WRITE		OSILLC_CONTROL_CODE(12, METHOD_BUFFERED)
#define IOCTL_OSILLC_READ		OSILLC_CONTROL_CODE(13, METHOD_BUFFERED)

#pragma pack()

#endif _OSILLC_H

