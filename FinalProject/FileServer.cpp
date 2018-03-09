#include "stdafx.h"
#include "FileServer.h"
#include "FileThread.h"
#include <winsock2.h>

#pragma comment(lib,"WS2_32.lib")

CFileServer::CFileServer()
{
}


CFileServer::~CFileServer()
{
}

BOOL CFileServer::Init()
{
	// 初始化WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 0);
	::WSAStartup(sockVersion, &wsaData);
	// 创建socket
	m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ServerSocket == INVALID_SOCKET)
	{
		AfxMessageBox(_T("Failed socket() for File Server"));
		return FALSE;
	}

	// 绑定IP端口
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_FILESERVICE);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 绑定
	bind(m_ServerSocket, (struct sockaddr*)&addr, sizeof(addr));
	// 监听
	listen(m_ServerSocket, 5);
	// 创建等候客户端的线程
	AfxBeginThread(FileThread, this);

	return TRUE;
}

UINT CFileServer::FileThread(LPVOID pParam)
{
	CFileServer* pThis = (CFileServer*)pParam;

	while (true)
	{
		// 接收
		struct sockaddr_in sin;
		int nLength = sizeof(sin);
		SOCKET ClientSocket = accept(pThis->m_ServerSocket,
			(SOCKADDR*)&sin, &nLength);

		if (ClientSocket == INVALID_SOCKET)
		{
			// TODO

		}

		LPSTR pszIP = inet_ntoa(sin.sin_addr);
		pThis->OnConnect(ClientSocket, pszIP);
	}	
	return 0;
}

void CFileServer::OnConnect(SOCKET ClientSocket, LPSTR pszIP)
{
	// 创建界面线程
	CFileThread* pFileThread = (CFileThread*)AfxBeginThread(RUNTIME_CLASS(CFileThread,
		THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED));

	// 设置参数
	pFileThread->SetSocket(ClientSocket, pszIP);

	// 开始线程
	pFileThread->ResumeThread();
}
