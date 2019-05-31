// EraseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Burster.h"
#include "SelectDlg.h"
#include "afxdialogex.h"


// CSelectDlg �Ի���

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

//��ʼ��
BOOL CSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CStatic* hint = (CStatic*)GetDlgItem(IDC_STATIC);
	hint->SetWindowText(m_Hint);

	CButton* btn = (CButton*)GetDlgItem(IDC_BUTTON1);
	btn->SetWindowText(m_Verify);

	//����ɾ����ť����
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);

	//����һ�ݴ������Ա
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


// CSelectDlg ��Ϣ�������


//ȷ��
void CSelectDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListBox* list = (CListBox*)GetDlgItem(IDC_LIST1);
	m_Index = list->GetCurSel();
	if (m_Select)
	{
		list->GetText(m_Index, *m_Select);
	}
	SendMessage(WM_CLOSE);
}

//ȡ��
void CSelectDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessage(WM_CLOSE);
}

//listboxѡ���¼�
void CSelectDlg::OnLbnSelchangeList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
}
