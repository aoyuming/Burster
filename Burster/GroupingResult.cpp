// GroupingResult.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Burster.h"
#include "GroupingResult.h"
#include "afxdialogex.h"


// CGroupingResult �Ի���

IMPLEMENT_DYNAMIC(CGroupingResult, CDialogEx)

CGroupingResult::CGroupingResult(CWnd* pParent ,CString str/*=NULL*/)
	: CDialogEx(IDD_DATA, pParent),
	m_Data(str)
{

}

CGroupingResult::~CGroupingResult()
{
}

void CGroupingResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
}


BEGIN_MESSAGE_MAP(CGroupingResult, CDialogEx)
END_MESSAGE_MAP()


// CGroupingResult ��Ϣ�������


//������Ϣ
BOOL CGroupingResult::PreTranslateMessage(MSG* pMsg)
{
	do
	{
		if (pMsg->message != WM_KEYDOWN)
			break;

		const SHORT l_cnKeyState = 0x8000;
		if (l_cnKeyState != (GetKeyState(VK_CONTROL) & l_cnKeyState))
			break;

		// do ctrl+A
		if (pMsg->wParam == 'A')
			m_Edit.SetSel(0, -1);

	} while (FALSE);

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CGroupingResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Edit.SetWindowText(m_Data);
	m_Edit.PostMessage(EM_SETSEL, 0, 0);
	//m_Edit.SetSel(-1, -1);
	//m_Edit.SetFocus();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
