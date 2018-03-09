#pragma once
#include "FileReceive.h"
#include "FileSend.h"


// CFileThread

class CFileThread : public CWinThread
{
	DECLARE_DYNCREATE(CFileThread)

protected:
	CFileThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CFileThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CFileReceiveDlg		m_FileReceiveDlg;
	CFileSendDlg		m_FileSendDlg;

public:
	BOOL m_bSendDlg;
	// 接收文件
	void SetSocket(SOCKET socket, CString strIP, BOOL bFileSendDlg = FALSE);
	// 发送文件
	void SetSocket(CString strFilename, CString strIP, BOOL bFileSendDlg = TRUE);
};


