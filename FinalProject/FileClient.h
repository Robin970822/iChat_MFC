#pragma once
class CFileClient
{
public:
	CFileClient();
	~CFileClient();
public:
	// 初始化文件客户端
	BOOL Init();
	// 连接服务器
	BOOL ConnectServer(LPCSTR lpServerIP);
	// 发送文件
	BOOL SendFile(LPCSTR pszFilePath, CProgressCtrl* pWndProgress = NULL);
	// 发送数据
	BOOL SendData(LPVOID pData, DWORD nLength);

public:
	SOCKET m_ClientSocket;  // 客户端socket
};

