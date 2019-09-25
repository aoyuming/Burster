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
	ON_EN_UPDATE(IDC_EDIT1, &addName::OnEnUpdateEdit1)
END_MESSAGE_MAP()


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

//添加的名字更改
void addName::OnEnUpdateEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	/*CString name;
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(name);
	if (name.GetLength() > 8)
	{
		CString temp = name.Left(8);
		int drLen = temp.GetLength();
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(name.Left(8));
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetSel(drLen, drLen, TRUE);
	}*/
}
