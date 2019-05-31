// addName.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Burster.h"
#include "addName.h"
#include "afxdialogex.h"


// addName �Ի���

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


// addName ��Ϣ�������


//ȷ��
void addName::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(str);
	if (str != _T(""))
	{
		m_Name = str;
		SendMessage(WM_CLOSE);
	}
}

//ȡ��
void addName::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessage(WM_CLOSE);
}
