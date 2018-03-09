
// FinalProjectView.h : CFinalProjectView 类的接口
//

#pragma once
#include "stdafx.h"
#include "UserClient.h"
#include "Resource.h"


class CFinalProjectView : public CFormView
{
protected: // 仅从序列化创建
	CFinalProjectView();
	DECLARE_DYNCREATE(CFinalProjectView)

public:
	enum{ IDD = IDD_FINALPROJECT_FORM };

	CListBox m_wndChatList;
	CEdit	 m_wndChatEdit;
	CButton  m_wndChatBtn;
	CFont    m_wndFont;

// 特性
public:
	CUserClient* m_pUserClient;

// 操作
public:
	void AddChat(CString strChat, CString strIP);

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CFinalProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonRefresh();
};

