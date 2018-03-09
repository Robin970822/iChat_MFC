// FileReceive.cpp : 实现文件
//

#include "stdafx.h"
#include "FinalProject.h"
#include "FileReceive.h"
#include "afxdialogex.h"


// FileReceive 对话框

IMPLEMENT_DYNAMIC(FileReceive, CDialog)

FileReceive::FileReceive(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_RECEIVE, pParent)
{
	m_ClientSocket = INVALID_SOCKET;
}

FileReceive::~FileReceive()
{
	if (m_ClientSocket != INVALID_SOCKET)
	{
		::closesocket(m_ClientSocket);
	}
}

void FileReceive::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RECEIVE, m_Info);
	DDX_Control(pDX, IDC_PROGRESS_RECEIVE, m_wndProgress);
}

BOOL FileReceive::OnInitDialog()
{
	CDialog::OnInitDialog();

	NETHEADER header;
	Receive((LPSTR)&header, sizeof(header));
	Receive((LPSTR)&m_SendFile, sizeof(m_SendFile));

	SetWindowText(m_strIP);

	return TRUE;
}


BEGIN_MESSAGE_MAP(FileReceive, CDialog)
	ON_BN_CLICKED(IDCANCEL, &FileReceive::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &FileReceive::OnBnClickedOk)
END_MESSAGE_MAP()


// FileReceive 消息处理程序


// 接收线程
UINT FileReceive::ReceiveThread(LPVOID pParam)
{
	FileReceive* pThis = (FileReceive*)pParam;
	// 获取文件大小
	DWORD nLength = pThis->m_SendFile.dwFileLength;

	// 创建文件
	CFile file;
	if (file.Open(pThis->m_strFilePath, 
		CFile::modeCreate | CFile::modeWrite) == FALSE)
	{
		AfxMessageBox(_T("无法写入文件，请检查权限"));
		return 0;
	}
	
	// 设置进度条
	pThis->m_wndProgress.SetRange32(0, nLength);
	pThis->m_Info.SetWindowText(_T("正在接受数据……"));

	// 接收数据
	CHAR szBuffer[4096];
	DWORD nRemain = nLength;
	while (nRemain > 0)
	{
		// 计算接收的数据量
		DWORD nReceive = 4096 < nRemain ? 4096 : nRemain;

		// 接收数据
		pThis->Receive(szBuffer, nReceive);
		// 写入文件
		file.Write(szBuffer, nReceive);
		// 计算剩余数据
		nRemain -= nReceive;

		// 设置进度条
		pThis->m_wndProgress.SetPos(nLength - nRemain);
	}
	pThis->m_Info.SetWindowText(_T("接收数据成功"));
	// 关闭文件
	file.Close();

	return 0;
}

BOOL FileReceive::Receive(LPVOID pData, DWORD nLength)
{
	LPSTR pTemp = (LPSTR)pData;
	int nRemain = nLength;
	while (nRemain > 0)
	{
		int nReceive = recv(m_ClientSocket, pTemp, nRemain, 0);
		if (nReceive == SOCKET_ERROR)
		{
			AfxMessageBox(_T("Failed to recv()"));
			return FALSE;
		}
		nRemain -= nReceive;
		pTemp += nReceive;
	}
	
	return TRUE;
}

void FileReceive::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void FileReceive::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(FALSE, NULL, m_SendFile.szFileName);
	if (dlg.DoModal() != IDOK)
	{
		AfxMessageBox(_T("请选择文件保存路径"));
		return;
	}

	GetDlgItem(IDOK)->EnableWindow(FALSE);

	// 得到路径名称
	m_strFilePath = dlg.GetPathName();

	AfxBeginThread(ReceiveThread, this);

	// CDialog::OnOK();
}
