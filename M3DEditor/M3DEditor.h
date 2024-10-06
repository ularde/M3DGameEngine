
// M3DEditor.h: M3DEditor 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号
#include "ModuleLoader.h"

inline HWND gHWND;
inline HDC gHDC;


// CM3DEditorApp:
// 有关此类的实现，请参阅 M3DEditor.cpp
//

class CM3DEditorApp : public CWinAppEx
{
public:
	CM3DEditorApp() noexcept;

public:
	CDocument* OpenScene(LPCTSTR fn) { return OpenDocumentFile(fn); }

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern CM3DEditorApp theApp;
