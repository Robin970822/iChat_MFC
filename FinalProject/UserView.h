#pragma once
#include "stdafx.h"


// UserView 视图

class UserView : public CListView
{
	DECLARE_DYNCREATE(UserView)

protected:
	UserView();           // 动态创建所使用的受保护的构造函数
	virtual ~UserView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

// 操作
public:
	// 添加用户
	void AddUser(CString strName, CString strIP);
	// 查找用户
	int FindUser(CString strIP);
	// 删除用户
	void DeleteUser(CString strIP);

// override
protected:
	virtual void onDraw(CDC* pDC);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	
protected:
	// 创建选项栏目
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// 判断各线程的heartbeat
	afx_msg void OnTimer(UINT nIDEVENT);
};


