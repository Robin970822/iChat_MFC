#include "stdafx.h"
#include "FileClient.h"
#include <winsock2.h>

#pragma comment(lib,"WS2_32.lib")

CFileClient::CFileClient()
{
	m_ClientSocket = INVALID_SOCKET;
}


CFileClient::~CFileClient()
{
	if (m_ClientSocket != INVALID_SOCKET)
	{
		::closesocket(m_ClientSocket);
		::WSACleanup();
	}
}

BOOL CFileClient::Init()
{
	// 初始化WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 0);
	::WSAStartup(sockVersion, &wsaData);
	// 创建socket
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ClientSocket == INVALID_SOCKET)
	{
		AfxMessageBox(_T("Failed socket() for File Client"));
		return FALSE;
	}
	return TRUE;
}

BOOL CFileClient::ConnectServer(LPCSTR lpServerIP)
{
	// 连接服务器
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_FILESERVICE);
	addr.sin_addr.S_un.S_addr = inet_addr(lpServerIP);
	if (connect(m_ClientSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("File connect()"));
		return FALSE;
	}

	return TRUE;
}

BOOL CFileClient::SendFile(LPCSTR pszFilePath, CProgressCtrl * pWndProgress)
{
	// 发送数据头
	NETHEADER header;
	header.dwVersion = 1;
	header.dwCmdID = NETCMDID_SENDFILE;
	header.dwDataLength = sizeof(SENDFILE);
	SendData(&header, sizeof(header));

	// 发送文件信息
	CFile file;
	if (file.Open(pszFilePath, CFile::modeRead) == FALSE)
	{
		AfxMessageBox(_T("Failed to open file"));
		return FALSE;
	}

	SENDFILE sendFile;
	strcpy(sendFile.szFileName, file.GetFileName());
	sendFile.dwFileLength = file.GetLength();
	SendData(&sendFile, sizeof(sendFile));

	if (pWndProgress != NULL)
	{
		pWndProgress->SetRange32(0, sendFile.dwFileLength);
	}
	// 发送文件数据
	CHAR szBuf[4096];
	DWORD nRemain = file.GetLength();
	while (nRemain > 0)
	{
		// 计算发送量
		DWORD nSend = 4096 < nRemain ? 4096 : nRemain;

		// 读取数据
		file.Read(szBuf, nSend);

		// 发送数据
		SendData(szBuf, nSend);

		// 计算剩余量
		nRemain -= nSend;

		// 设置进度条位置
		if (pWndProgress != NULL)
		{
			pWndProgress->SetPos(sendFile.dwFileLength - nRemain);
		}
	}

	// 关闭文件
	file.Close();
	return TRUE;
}

BOOL CFileClient::SendData(LPVOID pData, DWORD nLength)
{
	LPSTR pTemp = (LPSTR)pData;
	int nRemain = nLength;

	while (nRemain > 0)
	{
		int nSend = send(m_ClientSocket, pTemp, nRemain, 0);
		if (nSend == SOCKET_ERROR)
		{
			AfxMessageBox(_T("Failed send()"));
			return FALSE;
		}
		nRemain -= nSend;

		// 添加offset
		pTemp += nSend;
	}

	return TRUE;
}
