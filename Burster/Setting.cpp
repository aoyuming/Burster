// Setting.cpp : 实现文件
//

#include "stdafx.h"
#include "Burster.h"
#include "Setting.h"
#include "afxdialogex.h"


// CSetting 对话框

IMPLEMENT_DYNAMIC(CSetting, CDialogEx)

CSetting::CSetting(UpdateMode& mode, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SET, pParent),
	m_Mode(mode)
{

}

CSetting::~CSetting()
{
}

void CSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetting::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSetting 消息处理程序


void CSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Mode = (UpdateMode)((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	//((CBursterDlg*)(AfxGetApp()->GetMainWnd()))->m_pThread = AfxBeginThread(downRemoteFile, this);
	CDialogEx::OnOK();
}


void CSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(0, _T("总是检查更新"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(1, _T("每周检查更新"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(2, _T("从不检查更新"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(m_Mode);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSetting::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Mode = (UpdateMode)((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	CDialogEx::OnClose();
}
