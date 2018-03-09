
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "UserClient.h"
#include "UserServer.h"
#include "FileServer.h"

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
	CMainFrame();
	// 分割主窗体
	CSplitterWnd m_Splitter;
	// 客户端
	CUserClient m_UserClient;
	// 服务端
	CUserServer m_UserServer;
	// 文件服务器
	CFileServer m_FileServer;

// 操作
public:
	void Initialize();

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

};


