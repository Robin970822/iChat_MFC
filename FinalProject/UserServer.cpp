#include "stdafx.h"
#include "UserServer.h"

#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "WS2_32.lib")


UserServer::UserServer()
{
	m_pUserView = NULL;
	m_serverSocket = NULL;
}


UserServer::~UserServer()
{
}

// 初始化服务器
BOOL UserServer::Init()
{
	// 初始化WS2_32.dll
	WSADATA wsData;
	WORD socketVersion = MAKEWORD(2, 0);
	::WSAStartup(socketVersion, &wsData);

	// 创建UDP服务器
	m_serverSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	// 异常监测
	if (m_serverSocket == INVALID_SOCKET)
	{
		AfxMessageBox("Failed socket()");
		return FALSE;
	}
	// 绑定端口
	struct sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_USERSERVICE);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	BOOL bBind = bind(m_serverSocket, (struct sockaddr*)&addr, sizeof(addr));
	if (bBind == SOCKET_ERROR)
	{
		AfxMessageBox("Failed bind()");
		::WSACleanup();
		return FALSE;
	}
	
	// 创建服务器线程
	AfxBeginThread(UserThread, this);

	return TRUE;
}

UINT UserServer::UserThread(LPVOID pParam)
{
	UserServer* pThis = (UserServer*)pParam;
	// 接收客户端数据
	while (true)
	{
		UDPPACKET packet = { 0 };
		sockaddr_in addr = { 0 };
		int nLength = sizeof(addr);
		// 判断异常
		BOOL bRecvfrom = recvfrom(pThis->m_serverSocket, (char*)&packet, sizeof(packet), 0, 
			(struct sockaddr*)&addr, &nLength);
		if (bRecvfrom == SOCKET_ERROR)
		{
			AfxMessageBox("Failed recvfrom()");
		}
		
		// 转换地址为char*
		char* pszIP = inet_ntoa(addr.sin_addr);

		switch (packet.header.dwCmdID)
		{
		case NETCMDID_SENDFILE: // 发送文件
			break;
		case NETCMDID_USERBROADCAST: // 广播
			//AfxMessageBox("User Broadcast");
			pThis->OnUserBroadcast((LPUSERBROADCAST)&packet, pszIP);
			break;
		case NETCMDID_USERCHAT: // 聊天
			//AfxMessageBox("User Chat");
			pThis->OnUserChat((LPUSERCHAT)&packet, pszIP);
			break;
		case NETCMDID_USERQUIT: // 退出
			//AfxMessageBox("User Quit");
			pThis->OnUserQuit((LPUSERQUIT)&packet, pszIP);
		default:
			break;
		}
	}
	::WSACleanup();
	return 0;
}

BOOL UserServer::OnUserBroadcast(LPUSERBROADCAST pUserBroadcast, LPCSTR lpSrcIP)
{
	if (m_pUserView != NULL)
	{
		//AfxMessageBox(pUserBroadcast->szName);
		m_pUserView->AddUser(pUserBroadcast->szName, lpSrcIP);
	}
	return TRUE;
}

BOOL UserServer::OnUserChat(LPUSERCHAT pUserChat, LPCSTR lpSrcIP)
{
	if (pUserChat != NULL)
	{
		m_pFPView->AddChat(pUserChat->szChat, lpSrcIP);
	}
	return TRUE;
}

BOOL UserServer::OnUserQuit(LPUSERQUIT pUserQuit, LPCSTR lpSrcIP)
{
	if (m_pUserView != NULL)
	{
		m_pUserView->DeleteUser(lpSrcIP);
	}
	return TRUE;
}
