#pragma once


// FileReceive 对话框

class FileReceive : public CDialog
{
	DECLARE_DYNAMIC(FileReceive)

public:
	FileReceive(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FileReceive();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RECEIVE };
#endif

private:
	CStatic       m_Info;				// 接收信息
	CProgressCtrl m_wndProgress;		// 进度条
	SENDFILE      m_SendFile;			// 接收文件
	CString       m_strFilePath;		// 文件路径

public:
	SOCKET        m_ClientSocket;
	CString       m_strIP;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	// 接收线程
	static UINT ReceiveThread(LPVOID pParam);
	// 数据接收
	BOOL Receive(LPVOID pData, DWORD nLength);

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
