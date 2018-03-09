#pragma once



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
};


