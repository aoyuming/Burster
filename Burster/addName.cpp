// addName.cpp : 实现文件
//

#include "stdafx.h"
#include "Burster.h"
#include "addName.h"
#include "afxdialogex.h"


// addName 对话框

IMPLEMENT_DYNAMIC(addName, CDialogEx)

addName::addName(CString& name, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADD_NAME, pParent),
	m_Name(name)
{
}

addName::~addName()
{
}

void addName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(addName, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &addName::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &addName::OnBnClickedButton2)
END_MESSAGE_MAP()


// addName 消息处理程序


//确定
void addName::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(str);
	if (str != _T(""))
	{
		m_Name = str;
		SendMessage(WM_CLOSE);
	}
}

//取消
void addName::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessage(WM_CLOSE);
}
