// Setting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Burster.h"
#include "Setting.h"
#include "afxdialogex.h"


// CSetting �Ի���

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


// CSetting ��Ϣ�������


void CSetting::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Mode = (UpdateMode)((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	//((CBursterDlg*)(AfxGetApp()->GetMainWnd()))->m_pThread = AfxBeginThread(downRemoteFile, this);
	CDialogEx::OnOK();
}


void CSetting::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


BOOL CSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(0, _T("���Ǽ�����"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(1, _T("ÿ�ܼ�����"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(2, _T("�Ӳ�������"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(m_Mode);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CSetting::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_Mode = (UpdateMode)((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	CDialogEx::OnClose();
}
