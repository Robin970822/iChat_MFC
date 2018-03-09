// FileThread.cpp : 实现文件
//

#include "stdafx.h"
#include "FinalProject.h"
#include "FileThread.h"


// CFileThread

IMPLEMENT_DYNCREATE(CFileThread, CWinThread)

CFileThread::CFileThread()
{
	m_bSendDlg = FALSE;
}

CFileThread::~CFileThread()
{
}

BOOL CFileThread::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	if (m_bSendDlg == FALSE)
	{
		m_pMainWnd = &m_FileReceiveDlg;
		m_FileReceiveDlg.DoModal();
	}
	else
	{
		m_pMainWnd = &m_FileSendDlg;
		m_FileSendDlg.DoModal();
	}
	
	return TRUE;
}

int CFileThread::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

// 接收文件
void CFileThread::SetSocket(SOCKET socket, CString strIP, BOOL bFileSendDlg)
{
	m_FileReceiveDlg.m_ClientSocket = socket;
	m_FileReceiveDlg.m_strIP = strIP;
	m_bSendDlg = bFileSendDlg;
}

// 发送文件
void CFileThread::SetSocket(CString strFilename, CString strIP, BOOL bFileSendDlg)
{
	m_FileSendDlg.m_strFilename = strFilename;
	m_FileSendDlg.m_strDesIP = strIP;
	m_bSendDlg = bFileSendDlg;
}

BEGIN_MESSAGE_MAP(CFileThread, CWinThread)
END_MESSAGE_MAP()


// CFileThread 消息处理程序
