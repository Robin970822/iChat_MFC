#pragma once

#include "stdafx.h"

#define PORT_USERSERVICE 8989
#define PORT_FILESERVICE 8990

// 数据包的包头
typedef struct _tagNetHeader
{
	unsigned long  dwVersion;
	unsigned long  dwCmdID;
	unsigned long  dwDataLength;
}NETHEADER, *LPNETHEADER;

// UDP数据报
typedef struct _tagUDPPacket
{
	NETHEADER	header;
	char		szData[256];
}UDPPACKET, *LPUDPPACKET;

// 用户广播数据包
#define NETCMDID_USERBROADCAST 1
typedef struct _tagUserBroadcast
{
	NETHEADER	header;
	char		szName[128];
	//CHAR		szSign[128];
}USERBROADCAST, *LPUSERBROADCAST;

// 用户正常退出数据包
#define NETCMDID_USERQUIT 2
typedef struct _tagUserQuit
{
	NETHEADER	header;
	unsigned long		dwExitCode;
}USERQUIT, *LPUSERQUIT;

// 聊天数据
#define NETCMDID_USERCHAT 3
typedef struct _tagUserChat
{
	NETHEADER	header;
	char		szChat[256];
}USERCHAT, *LPUSERCHAT;

// 发送文件  
#define  NETCMDID_SENDFILE  4
typedef struct  _tagSendFile
{
	char  szFileName[256];
	unsigned long dwFileLength;
}SENDFILE, *LPSENDFILE;
