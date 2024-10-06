// EnvironmentEditor.cpp: 实现文件
//

#include "pch.h"
#include "M3DEditor.h"
#include "EnvironmentEditor.h"
#include "afxdialogex.h"


// CEnvironmentEditor 对话框

IMPLEMENT_DYNAMIC(CEnvironmentEditor, CDialogEx)

CEnvironmentEditor::CEnvironmentEditor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENV_EDITOR, pParent)
{

}

CEnvironmentEditor::~CEnvironmentEditor()
{
}

void CEnvironmentEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEnvironmentEditor, CDialogEx)
END_MESSAGE_MAP()


// CEnvironmentEditor 消息处理程序
