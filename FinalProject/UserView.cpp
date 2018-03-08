// UserView.cpp : 实现文件
//

#include "stdafx.h"
#include "FinalProject.h"
#include "UserView.h"

// UserView

IMPLEMENT_DYNCREATE(UserView, CListView)

UserView::UserView()
{

}

UserView::~UserView()
{
}

BEGIN_MESSAGE_MAP(UserView, CListView)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// UserView 诊断

#ifdef _DEBUG
void UserView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void UserView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// UserView 消息处理程序
// 添加用户至用户信息列表
void UserView::AddUser(CString strName, CString strIP)
{
	CListCtrl& ctrl = GetListCtrl();
	// 获取当前列表行数
	int nItem = ctrl.GetItemCount();
	// 如果列表中存在当前用户，则将其heartbeat设为1，表示在线
	if (FindUser(strIP) != -1)
	{
		ctrl.SetItemData(nItem, 1);
		return;
	}
	//AfxMessageBox(strName);
	nItem = ctrl.InsertItem(nItem, NULL);
	ctrl.SetItemText(nItem, 1, strName);
	ctrl.SetItemText(nItem, 2, strIP);
	// 新家用户，heartbeat置为1，表示在线
	ctrl.SetItemData(nItem, 1);
	return;
}

// 在用户信息列表中查询用户
int UserView::FindUser(CString strIP)
{
	CListCtrl &ctrl = GetListCtrl();
	int nCount = ctrl.GetItemCount();

	for (size_t i = 0; i < nCount; i++)
	{
		// 获取IP地址
		CString strItemIP = ctrl.GetItemText(i, 2);
		// 如果查找到该用户
		if (strItemIP.CompareNoCase(strIP) == 0)
		{
			return i;
		}
	}

	// 未查到该用户
	return -1;
}

// 删除用户，当该用户不在线时
void UserView::DeleteUser(CString strIP)
{
	// 查找用户
	int nItem = FindUser(strIP);
	if (nItem == -1)
	{
		AfxMessageBox(strIP + "is not Found");
		return;
	}
	// 删除用户
	CListCtrl& ctrl = GetListCtrl();
	ctrl.DeleteItem(nItem);
	return;
}

void UserView::onDraw(CDC * pDC)
{
	CDocument* pDoc = GetDocument();
	return;
}

// 禁止拖动表头
BOOL UserView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT * pResult)
{
	// 屏蔽两个消息通知码
	NMHEADER* pNMheader = (NMHEADER*)lParam;

	if ((pNMheader->hdr.code == HDN_BEGINTRACKW) |
		(pNMheader->hdr.code == HDN_DIVIDERDBLCLICKW))
	{
		*pResult = TRUE;
		return TRUE;
	}

	return CWnd::OnNotify(wParam, lParam, pResult);
}

// 创建界面
int UserView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// 定义样式
	ModifyStyle(0, LVS_REPORT);

	CListCtrl& ctrl = GetListCtrl();
	ctrl.SetExtendedStyle(LVS_EX_CHECKBOXES);
	ctrl.InsertColumn(0, _T(""), LVCFMT_LEFT, 40);
	ctrl.InsertColumn(1, _T("计算机名称"), LVCFMT_LEFT, 300);
	ctrl.InsertColumn(2, _T("IP地址"), LVCFMT_LEFT, 300);

	SetTimer(2, 10000, NULL);
	return 0;
}

// 处理heartbeat
void UserView::OnTimer(UINT nIDEVENT)
{
	if (nIDEVENT != 2)
	{
		return;
	}

	CListCtrl& ctrl = GetListCtrl();
	int nCount = ctrl.GetItemCount();

	for (size_t i = 0; i < nCount; i++)
	{
		DWORD dwData = ctrl.GetItemData(i);
		if (dwData > 0)
		{// 如果heartbeat值为1，则置为0
			ctrl.SetItemData(i, 0);
		}
		else
		{// 如果heartbeat值为0，则删除
			ctrl.DeleteItem(i);
		}
	}
	CListView::OnTimer(nIDEVENT);
	return;
}
