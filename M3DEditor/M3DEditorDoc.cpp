
// M3DEditorDoc.cpp: CM3DEditorDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "M3DEditor.h"
#endif

#include "M3DEditorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CM3DEditorDoc

IMPLEMENT_DYNCREATE(CM3DEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CM3DEditorDoc, CDocument)
END_MESSAGE_MAP()


// CM3DEditorDoc 构造/析构

CM3DEditorDoc::CM3DEditorDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CM3DEditorDoc::~CM3DEditorDoc()
{
}

BOOL CM3DEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CM3DEditorDoc 序列化

void CM3DEditorDoc::Serialize(CArchive& ar)
{
	CString filePath = ar.GetFile()->GetFilePath();
	if (ar.IsStoring())
	{
		char fn[1024] = { 0 };
		wcstombs(fn, filePath, 1024);
		ar.GetFile()->Close();
		PIEditor_SaveScene(fn);
		ar.GetFile()->Open(filePath, CFile::modeRead);
	}
	else
	{
		char fn[1024] = { 0 };
		wcstombs(fn, filePath, 1024);
		PIEditor_LoadScene(fn);
		PIEditor_ExitGame();
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CM3DEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CM3DEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CM3DEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CM3DEditorDoc 诊断

#ifdef _DEBUG
void CM3DEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CM3DEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CM3DEditorDoc 命令


BOOL CM3DEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}


BOOL CM3DEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	return CDocument::OnSaveDocument(lpszPathName);
}


BOOL CM3DEditorDoc::SaveModified()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDocument::SaveModified();
}


void CM3DEditorDoc::ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault)
{
	// TODO: 在此添加专用代码和/或调用基类

	CDocument::ReportSaveLoadException(lpszPathName, e, bSaving, nIDPDefault);
}
