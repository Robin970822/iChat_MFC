#pragma once

#include "UserView.h"
#include "stdafx.h"
#include "FinalProjectView.h"

class UserServer
{
public:
	UserServer();
	~UserServer();

public:
	// 初始化User服务器
	BOOL Init();
	// 接收数据的线程
	static UINT UserThread(LPVOID pParam);
	// 处理用户广播过来的数据
	BOOL OnUserBroadcast(LPUSERBROADCAST pUserBroadcast, LPCSTR lpSrcIP);
	// 处理聊天数据
	BOOL OnUserChat(LPUSERCHAT pUserChat, LPCSTR lpSrcIP);
	// 用户退出
	BOOL OnUserQuit(LPUSERQUIT pUserQuit, LPCSTR lpSrcIP);
public:
	SOCKET	                m_ServerSocket; // 服务器Socket
	UserView*	            m_pUserView;    // 用户列表
	CFinalProjectView*		m_pFPView;
};

