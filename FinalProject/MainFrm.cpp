
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "FinalProject.h"
#include "FinalProjectView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("未能创建工具栏\n");
	//	return -1;      // 未能创建
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("未能创建状态栏\n");
	//	return -1;      // 未能创建
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//// TODO: 如果不需要可停靠工具栏，则删除这三行
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);
	Initialize();


	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	m_UserClient.Broadcast();
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnClose()
{
	m_UserClient.Qiut();
	CFrameWnd::OnClose();
}

void CMainFrame::Initialize()
{
	if (m_UserServer.Init() != TRUE) {
		AfxMessageBox(_T("用户创建服务端失败！"));
	}
	if (m_UserClient.Init() != TRUE) {
		AfxMessageBox(_T("用户创建客户端失败！"));
	}

	m_UserClient.Broadcast();

	SetTimer(1, 1000 * 10, NULL);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.cx = 802;
	cs.cy = 800;

	cs.style = cs.style & (~WS_THICKFRAME);
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~(WS_MAXIMIZEBOX); // 禁用最大化
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void * pExtra, AFX_CMDHANDLERINFO * pHandlerInfo)
{
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext * pContext)
{
	// 切分窗口， 上面部分为UserView, 下面部分为CFianlProjectView
	m_Splitter.CreateStatic(this, 2, 1);
	m_Splitter.CreateView(0, 0, RUNTIME_CLASS(UserView), CSize(1000, 300), pContext);
	m_Splitter.CreateView(1, 0, RUNTIME_CLASS(CFinalProjectView), CSize(1000, 400), pContext);

	// 获取UserView地址
	m_UserServer.m_pUserView = (UserView*)m_Splitter.GetPane(0, 0);

	// 获取ChatView地址
	CFinalProjectView* pCFinalProjectView = (CFinalProjectView*)m_Splitter.GetPane(1, 0);

	m_UserServer.m_pFPView = pCFinalProjectView;

	pCFinalProjectView->m_pUserClient = &m_UserClient;

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

