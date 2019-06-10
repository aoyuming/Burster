// EditMenber.cpp : 实现文件
//

#include "stdafx.h"
#include "Burster.h"
#include "EditMember.h"
#include "afxdialogex.h"
#include <vector>


// CEditMenber 对话框

IMPLEMENT_DYNAMIC(CEditMember, CDialogEx)

CEditMember::CEditMember(CString& name, CString& money, int nSum, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EDIT_MENBER, pParent),
	m_sName(name),
	m_sMoney(money), 
	m_Sum(nSum),
	m_Select(LOSE)
{

}

CEditMember::~CEditMember()
{
}

void CEditMember::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_Name_Edit);
	DDX_Control(pDX, IDC_EDIT3, m_Money_Edit);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
}


BEGIN_MESSAGE_MAP(CEditMember, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CEditMember::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEditMember::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_RADIO1, &CEditMember::OnBnClickedRadio1_lose)
	ON_BN_CLICKED(IDC_RADIO2, &CEditMember::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CEditMember::OnBnClickedRadio3)
	ON_CBN_CLOSEUP(IDC_COMBO1, &CEditMember::OnCbnCloseupCombo1)
END_MESSAGE_MAP()

//初始化
BOOL CEditMember::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_Money_Edit.EnableWindow(FALSE);
	m_Money_Edit.SetWindowText(m_sMoney);
	m_Name_Edit.SetWindowText(m_sName);
	m_Money_Edit.GetWindowText(m_TempMoney);

	std::vector<CButton*> vect;
	vect.push_back((CButton*)GetDlgItem(IDC_RADIO1));
	vect.push_back((CButton*)GetDlgItem(IDC_RADIO2));
	vect.push_back((CButton*)GetDlgItem(IDC_RADIO3));

	int money = _ttoi(m_sMoney);
	CString flag = "";
	int index = 0;
	if (money == 0)
	{
		index = 2;
		m_Select = NO_WIN_LOSE;
		m_ComboBox.EnableWindow(FALSE);
	}
	else if (money < 0)
	{
		index = 0;
		flag = "-";
		m_Select = LOSE;
	}
	else if (money > 0)
	{
		index = 1;
		flag = "+";
		m_Select = WIN;
	}

	vect[index]->SetCheck(TRUE);

	//设置下拉框
	CString moeny;
	for (int i = 0; i < 30; ++i)
	{
		moeny.Format(_T("%d"), (i + 1) * m_Sum);
		m_ComboBox.InsertString(i, flag + moeny);
	}
	
	return TRUE;
}

// CEditMenber 消息处理程序
//确定
void CEditMember::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Name_Edit.GetWindowText(m_sName);
	m_Money_Edit.GetWindowText(m_sMoney);
	SendMessage(WM_CLOSE);
}

//取消
void CEditMember::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessage(WM_CLOSE);
}

//输选中
void CEditMember::OnBnClickedRadio1_lose()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Select == LOSE)
		return;
	else
		m_Select = LOSE;

	//设置下拉框
	m_ComboBox.EnableWindow(TRUE);
	m_ComboBox.ResetContent();
	m_Money_Edit.SetWindowText(m_TempMoney);
	CString moeny;
	for (int i = 0; i < 30; ++i)
	{
		moeny.Format(_T("%d"), (i + 1) * m_Sum);
		m_ComboBox.InsertString(i, "-" + moeny);
	}
}

//赢选中
void CEditMember::OnBnClickedRadio2()
{
	if (m_Select == WIN)
		return;
	else
		m_Select = WIN;

	//设置下拉框
	m_ComboBox.EnableWindow(TRUE);
	m_ComboBox.ResetContent();
	m_Money_Edit.SetWindowText(m_TempMoney);
	CString moeny;
	for (int i = 0; i < 30; ++i)
	{
		moeny.Format(_T("%d"), (i + 1) * m_Sum);
		m_ComboBox.InsertString(i, "+" + moeny);
	}
}

//不输不赢选中
void CEditMember::OnBnClickedRadio3()
{
	if (m_Select == NO_WIN_LOSE)
		return;
	else
		m_Select = NO_WIN_LOSE;

	m_ComboBox.ResetContent();
	m_ComboBox.EnableWindow(FALSE);
	m_Money_Edit.SetWindowText(_T("0"));
}

//选中金额
void CEditMember::OnCbnCloseupCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString money;
	m_ComboBox.GetWindowText(money);
	m_Money_Edit.SetWindowText(money);
}
