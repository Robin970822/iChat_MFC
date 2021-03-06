#include "stdafx.h"
#include "UserServer.h"

#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "WS2_32.lib")


CUserServer::CUserServer()
{
	m_pUserView = NULL;
	m_ServerSocket = INVALID_SOCKET;
}


CUserServer::~CUserServer()
{
	if (m_ServerSocket != INVALID_SOCKET)
	{
		::closesocket(m_ServerSocket);
		::WSACleanup();
	}
}

// 初始化服务器
BOOL CUserServer::Init()
{
	// 初始化WS2_32.dll
	WSADATA wsData;
	WORD socketVersion = MAKEWORD(2, 0);
	::WSAStartup(socketVersion, &wsData);

	// 创建UDP服务器
	m_ServerSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	// 异常监测
	if (m_ServerSocket == INVALID_SOCKET)
	{
		AfxMessageBox("Failed socket()");
		return FALSE;
	}
	// 绑定端口
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_USERSERVICE);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	BOOL bBind = bind(m_ServerSocket, (struct sockaddr*)&addr, sizeof(addr));
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

UINT CUserServer::UserThread(LPVOID pParam)
{
	CUserServer* pThis = (CUserServer*)pParam;
	// 接收客户端数据
	while (true)
	{
		UDPPACKET packet;
		sockaddr_in addr;
		int nLength = sizeof(addr);
		// 判断异常
		BOOL bRecvfrom = recvfrom(pThis->m_ServerSocket, (char*)&packet, sizeof(packet), 0, 
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

BOOL CUserServer::OnUserBroadcast(LPUSERBROADCAST pUserBroadcast, LPCSTR lpSrcIP)
{
	if (m_pUserView != NULL)
	{
		//AfxMessageBox(pUserBroadcast->szName);
		m_pUserView->AddUser(pUserBroadcast->szName, lpSrcIP);
	}
	return TRUE;
}

BOOL CUserServer::OnUserChat(LPUSERCHAT pUserChat, LPCSTR lpSrcIP)
{
	if (pUserChat != NULL)
	{
		m_pFPView->AddChat(pUserChat->szChat, lpSrcIP);
	}
	return TRUE;
}

BOOL CUserServer::OnUserQuit(LPUSERQUIT pUserQuit, LPCSTR lpSrcIP)
{
	if (m_pUserView != NULL)
	{
		m_pUserView->DeleteUser(lpSrcIP);
	}
	return TRUE;
}
