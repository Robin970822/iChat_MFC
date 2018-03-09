#pragma once
class CFileServer
{
public:
	CFileServer();
	~CFileServer();

public:
	// 初始化文件服务器
	BOOL Init();
	// 接收客户端连接的线程
	static UINT FileThread(LPVOID pParam);
	// 处理连接
	void OnConnect(SOCKET ClientSocket, LPSTR pszIP);

public:
	SOCKET m_ServerSocket;
};

