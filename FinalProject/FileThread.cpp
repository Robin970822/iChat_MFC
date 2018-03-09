// FileThread.cpp : 实现文件
//

#include "stdafx.h"
#include "FinalProject.h"
#include "FileThread.h"


// CFileThread

IMPLEMENT_DYNCREATE(CFileThread, CWinThread)

CFileThread::CFileThread()
{
}

CFileThread::~CFileThread()
{
}

BOOL CFileThread::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	return TRUE;
}

int CFileThread::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CFileThread, CWinThread)
END_MESSAGE_MAP()


// CFileThread 消息处理程序
