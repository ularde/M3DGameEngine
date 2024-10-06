
// M3DEditorView.cpp: CM3DEditorView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "M3DEditor.h"
#endif

#include "M3DEditorDoc.h"
#include "M3DEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CM3DEditorView

IMPLEMENT_DYNCREATE(CM3DEditorView, CView)

BEGIN_MESSAGE_MAP(CM3DEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
//	ON_WM_KEYDOWN()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
ON_WM_INPUT()
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEWHEEL()
ON_WM_MOUSEHWHEEL()
ON_WM_XBUTTONUP()
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONUP()
ON_WM_XBUTTONDOWN()
END_MESSAGE_MAP()

// CM3DEditorView 构造/析构

CM3DEditorView::CM3DEditorView() noexcept
{
	// TODO: 在此处添加构造代码

}

CM3DEditorView::~CM3DEditorView()
{
}

BOOL CM3DEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

// CM3DEditorView 绘图

void CM3DEditorView::OnDraw(CDC* /*pDC*/)
{
	CM3DEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

void CM3DEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CM3DEditorView 诊断

#ifdef _DEBUG
void CM3DEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CM3DEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CM3DEditorDoc* CM3DEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CM3DEditorDoc)));
	return (CM3DEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CM3DEditorView 消息处理程序


void CM3DEditorView::OnInitialUpdate()
{
	PIEditor_SetHWND(GetSafeHwnd());
	static bool initialized = false;
	CView::OnInitialUpdate();
	if (!initialized) {
		HDC hDC = ::GetDC(GetSafeHwnd());
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			24,
			0,0,0,0,0,0,
			0,
			0,
			0,0,0,0,
			8,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0,0,0
		};
		int pf;
		pf = ChoosePixelFormat(hDC, &pfd);
		if (!pf) {
			MessageBox(L"Failed to get a proper pixel format.", L"M3D Editor", MB_ICONERROR);
		}
		else {
			SetPixelFormat(hDC, pf, &pfd);
			auto hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);
			PIEditor_SetGetProcAddress((void*)wglGetProcAddress);
			PICreateMainInterface(M3D_WinEditor);
			gHWND = GetSafeHwnd();
			gHDC = ::GetDC(GetSafeHwnd());
			CRect rect;
			GetClientRect(&rect);
			PIEditor_SubmitFramebufferSizeInput(rect.Width(), rect.Height());
			initialized = true;
		}
	}
}


void CM3DEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	PIEditor_SubmitFramebufferSizeInput(cx, cy);
}


void CM3DEditorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CView::OnPaint()
}


BOOL CM3DEditorView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CView::OnCommand(wParam, lParam);
}


BOOL CM3DEditorView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CM3DEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!PIEditor_GetCursorDisabled()) {
		PIEditor_SubmitCursorPosInput(point.x, point.y);
	}
	CView::OnMouseMove(nFlags, point);
}


//void CM3DEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CView::OnKeyDown(nChar, nRepCnt, nFlags);
//}


void CM3DEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PIEditor_SubmitKeyboardInput(nChar, 1);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CM3DEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PIEditor_SubmitKeyboardInput(nChar, 0);
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CM3DEditorView::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static double vx = 0.0, vy = 0.0;
	UINT size = 0;
	GetRawInputData(hRawInput, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[size];
	if (lpb != NULL) {
		GetRawInputData(hRawInput, RID_INPUT, lpb, &size, sizeof(RAWINPUTHEADER));
		RAWINPUT* raw = (RAWINPUT*)lpb;
		if (raw->header.dwType == RIM_TYPEMOUSE) {
			vx += (double)raw->data.mouse.lLastX;
			vy += (double)raw->data.mouse.lLastY;
			PIEditor_SubmitCursorPosInput(vx, vy);
		}
	}
	CView::OnRawInput(nInputcode, hRawInput);
}


void CM3DEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//PIEditor_SubmitMouseButtonInput(0, 1);
	ProcessMouseButtonInput();
	CView::OnLButtonDown(nFlags, point);
}


void CM3DEditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ProcessMouseButtonInput();
	//PIEditor_SubmitMouseButtonInput(1, 1);
	CView::OnRButtonDown(nFlags, point);
}


void CM3DEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ProcessMouseButtonInput();
	//PIEditor_SubmitMouseButtonInput(0, 0);
	CView::OnLButtonUp(nFlags, point);
}

void CM3DEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	ProcessMouseButtonInput();
	//PIEditor_SubmitMouseButtonInput(1, 0);
	//CView::OnRButtonUp(nFlags, point);
	//The menu shouldn't be shown here.
	//OnContextMenu(this, point);
}

BOOL CM3DEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	switch (zDelta)
	{
	case -120:
		PIEditor_SubmitScrollOffset(0.0, -1.0);
		break;
	case 120:
		PIEditor_SubmitScrollOffset(0.0, 1.0);
		break;
	default:
		break;
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CM3DEditorView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CM3DEditorView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	UINT uMsg = pMsg->message;
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_XBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	{
		int button = 0, action = 0;
		if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP)
		{
			button = 0;
		}
		else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP)
		{
			button = 1;
		}
		//else if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP)
		//	button = GLFW_MOUSE_BUTTON_MIDDLE;
		//else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
		//	button = GLFW_MOUSE_BUTTON_4;
		//else
		//	button = GLFW_MOUSE_BUTTON_5;

		if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN ||
			uMsg == WM_MBUTTONDOWN || uMsg == WM_XBUTTONDOWN)
		{
			action = 1;
		}
		else
		{
			action = 0;
		}
		PIEditor_SubmitMouseButtonInput(button, action);
		break;
	}
	default:
		break;
	}
	return CView::PreTranslateMessage(pMsg);
}


void CM3DEditorView::OnXButtonUp(UINT nFlags, UINT nButton, CPoint point)
{
	// 该功能要求使用 Windows 2000 或更高版本。
	// 符号 _WIN32_WINNT 和 WINVER 必须 >= 0x0500。
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ProcessMouseButtonInput();
	CView::OnXButtonUp(nFlags, nButton, point);
}


//void CM3DEditorView::ProcessMouseButtonInput()
//{
//	// TODO: 在此处添加实现代码.
//}


void CM3DEditorView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ProcessMouseButtonInput();
	CView::OnMButtonDown(nFlags, point);
}


void CM3DEditorView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ProcessMouseButtonInput();
	CView::OnMButtonUp(nFlags, point);
}


void CM3DEditorView::OnXButtonDown(UINT nFlags, UINT nButton, CPoint point)
{
	// 该功能要求使用 Windows 2000 或更高版本。
	// 符号 _WIN32_WINNT 和 WINVER 必须 >= 0x0500。
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ProcessMouseButtonInput();
	CView::OnXButtonDown(nFlags, nButton, point);
}


void CM3DEditorView::ProcessMouseButtonInput()
{
	// TODO: 在此处添加实现代码.
}
