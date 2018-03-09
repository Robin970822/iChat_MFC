#pragma once
#include "FileClient.h"


// FileSend 对话框

class FileSend : public CDialog
{
	DECLARE_DYNAMIC(FileSend)

public:
	FileSend(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FileSend();

private:
	CStatic		   m_Info;				// 发送信息
	CProgressCtrl  m_wndSendProgress;	// 发送进度条
	FileClient     m_FileCilent;		// 文件客户端
	BOOL		   m_bSend;			// 收发标志

public:
	CString	       m_strDesIP;			// 目的IP
	CString        m_strFilename;		// 传输的文件


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SEND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	static UINT SendThread(LPVOID pParam);
	void Send();
};
