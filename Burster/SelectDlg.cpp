// EraseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Burster.h"
#include "SelectDlg.h"
#include "afxdialogex.h"


// CSelectDlg 对话框

IMPLEMENT_DYNAMIC(CSelectDlg, CDialogEx)

CSelectDlg::CSelectDlg(const vector<stMember*>& cur, int& index, CString* select, CString hint, CString verify, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ERASE, pParent),
	m_Index(index),
	m_Hint(hint),
	m_Verify(verify),
	m_Select(select)
{
	for (int i = 0; i < (int)cur.size(); ++i)
		m_CurMember.push_back((*cur[i]));
}

CSelectDlg::CSelectDlg(const vector<stMember>& cur, int& index, CString* select, CString hint, CString verify, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ERASE, pParent),
	m_CurMember(cur),
	m_Index(index),
	m_Hint(hint),
	m_Verify(verify),
	m_Select(select)
{

}



CSelectDlg::~CSelectDlg()
{
}

void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//初始化
BOOL CSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CStatic* hint = (CStatic*)GetDlgItem(IDC_STATIC);
	hint->SetWindowText(m_Hint);

	CButton* btn = (CButton*)GetDlgItem(IDC_BUTTON1);
	btn->SetWindowText(m_Verify);

	//设置删除按钮禁用
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);

	//复制一份待分组成员
	CListBox* list = (CListBox*)GetDlgItem(IDC_LIST1);
	for (int i = 0; i < (int)m_CurMember.size(); ++i)
		list->AddString(m_CurMember[i].name);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSelectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSelectDlg::OnBnClickedButton2)
	ON_LBN_SELCHANGE(IDC_LIST1, &CSelectDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CSelectDlg 消息处理程序


//确认
void CSelectDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CListBox* list = (CListBox*)GetDlgItem(IDC_LIST1);
	m_Index = list->GetCurSel();
	if (m_Select)
	{
		list->GetText(m_Index, *m_Select);
	}
	SendMessage(WM_CLOSE);
}

//取消
void CSelectDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessage(WM_CLOSE);
}

//listbox选中事件
void CSelectDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
}
