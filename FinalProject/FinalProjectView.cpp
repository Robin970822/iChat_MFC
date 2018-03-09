
// FinalProjectView.cpp : CFinalProjectView 类的实现
//

#include "stdafx.h"
#include "FinalProject.h"

#include "FinalProjectView.h"
#include "MainFrm.h"
#include "FileThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFinalProjectView

IMPLEMENT_DYNCREATE(CFinalProjectView, CFormView)

BEGIN_MESSAGE_MAP(CFinalProjectView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CFinalProjectView::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CFinalProjectView::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CFinalProjectView::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()

// CFinalProjectView 构造/析构

CFinalProjectView::CFinalProjectView()
	: CFormView(IDD_FINALPROJECT_FORM)
{
	// TODO: 在此处添加构造代码
	m_pUserClient = NULL;

}

CFinalProjectView::~CFinalProjectView()
{
}

void CFinalProjectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHAT, m_wndChatList);
	DDX_Control(pDX, IDC_EDIT_CHAT, m_wndChatEdit);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_wndChatBtn);
}

void CFinalProjectView::AddChat(CString strChat, CString strIP)
{
	//AfxMessageBox(strChat);
	CString strData = strIP + _T(":") + strChat;
	int nItem = m_wndChatList.AddString(strData);
	m_wndChatList.SetCurSel(nItem);
}

BOOL CFinalProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CFinalProjectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_wndFont.DeleteObject();
	m_wndFont.CreatePointFont(130, _T("微软雅黑"));
	m_wndChatEdit.SetFont(&m_wndFont);
}


// CFinalProjectView 诊断

#ifdef _DEBUG
void CFinalProjectView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFinalProjectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

#endif //_DEBUG


// CFinalProjectView 消息处理程序


void CFinalProjectView::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pUserClient == NULL)
	{
		return;
	}
	CString strData = _T("");
	m_wndChatEdit.GetWindowText(strData);

	// 清空
	m_wndChatEdit.SetSel(0, -1);
	m_wndChatEdit.Clear();

	// 发送的消息不能为空
	if (strData.IsEmpty())
	{
		AfxMessageBox(_T("发送的信息不能为空！"));
		return;
	}

	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CUserView* pUserView = (CUserView*)pMain->m_Splitter.GetPane(0, 0);

	CListCtrl& ctrl = pUserView->GetListCtrl();
	int nCount = ctrl.GetItemCount();

	bool flag = false;
	for (size_t i = 0; i < nCount; i++)
	{
		if (ctrl.GetCheck(i))
		{
			flag = true;
			//AfxMessageBox(strData);
			CString strIP = ctrl.GetItemText(i, 2);
			//AfxMessageBox(strIP);
			m_pUserClient->SendChat(strData, strIP);
		}
	}

	if (flag == true)
	{
		AddChat(strData, _T("My word"));
	}
	else
	{
		AfxMessageBox(_T("请选择发送的对象！"));
	}
}


void CFinalProjectView::OnBnClickedButtonFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() != IDOK)
	{
		AfxMessageBox(_T("请选择文件"));
		return;
	}
	CString strFilePath = dlg.GetPathName();

	// 获取用户视图
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CUserView* pUserView = (CUserView*)pMain->m_Splitter.GetPane(0, 0);

	CListCtrl& ctrl = pUserView->GetListCtrl();
	int nCount = ctrl.GetItemCount();

	for (size_t i = 0; i < nCount; i++)
	{
		if (ctrl.GetCheck(i))
		{
			CString strIP = ctrl.GetItemText(i, 2);

			// 创建界面线程
			CFileThread* pFileThread = (CFileThread*)AfxBeginThread(RUNTIME_CLASS(CFileThread,
				THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED));

			// 设置参数
			pFileThread->SetSocket(strFilePath, strIP, TRUE);

			// 开始线程
			pFileThread->ResumeThread();
		}
	}
}


void CFinalProjectView::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CUserView* pUserView = (CUserView*)pMain->m_Splitter.GetPane(0, 0);

	CListCtrl& ctrl = pUserView->GetListCtrl();
	int nCount = ctrl.GetItemCount();
	for (size_t i = 0; i < nCount; i++)
	{
		ctrl.DeleteItem(i);
	}
	pMain->m_UserClient.Broadcast();

}
