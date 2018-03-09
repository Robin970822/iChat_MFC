#pragma once
#include "stdafx.h"
#include "UserView.h"

class CUserClient
{
public:
	CUserClient();
	~CUserClient();

public:
	// 初始化客户端
	BOOL Init();
	// 广播数据
	BOOL Broadcast();
	// 发送数据
	BOOL SendData(CHAR* psxData, UINT nlength, LPCSTR lpDesIP = NULL);
	// 正常关闭退出程序
	BOOL Qiut();
	// 发送信息
	BOOL SendChat(CString strData, CString strDesIP);

public:
	SOCKET m_ClientSocket;  // 客户端Socket
};

