// GroupingResult.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Burster.h"
#include "GroupingResult.h"
#include "afxdialogex.h"
#include <iostream>

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
	ON_WM_TIMER()
	ON_WM_CLOSE()
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

		if (pMsg->wParam == 'C')
			SetTimer(1, 30, NULL);

	} while (FALSE);

	return CDialog::PreTranslateMessage(pMsg);
}

// ��ȡ���а��ϵ��ַ���
CString CGroupingResult::fnGetStrFromClipboard(void)
{
	CString strTmp;
	strTmp.Empty();
	if (::OpenClipboard(NULL))
	{
		UINT nFormat = (sizeof(TCHAR) == sizeof(WCHAR) ? CF_UNICODETEXT : CF_TEXT);
		HGLOBAL hClip = GetClipboardData(nFormat);        // typedef HANDLE HGLOBAL
		if (hClip)
		{
			char *pBuff = (char*)GlobalLock(hClip);
			GlobalUnlock(hClip);
			strTmp.Format(_T("%s"), pBuff);
		}
		CloseClipboard();
	}
	return strTmp;
}

// ���ü��а�����
void CGroupingResult::fnCopyStrToClipboard(CString szStr)
{
	if (::OpenClipboard(NULL))
	{
		if (EmptyClipboard())
		{
			size_t cbStr = (szStr.GetLength() + 1)*sizeof(TCHAR);
			HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);
			memcpy_s(GlobalLock(hData), cbStr, szStr.LockBuffer(), cbStr);
			szStr.UnlockBuffer();
			GlobalUnlock(hData);
			UINT nFormat = (sizeof(TCHAR) == sizeof(WCHAR) ? CF_UNICODETEXT : CF_TEXT);
			if (NULL == ::SetClipboardData(nFormat, hData))
			{
				CloseClipboard();
				return;
			}
		}
		CloseClipboard();        //�رռ��а�
	}
}

BOOL CGroupingResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Edit.SetWindowText(m_Data);
	m_Edit.PostMessage(EM_SETSEL, 0, 0);
	//m_Edit.SetSel(-1, -1);
	//m_Edit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CGroupingResult::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		KillTimer(1);
		CString str = fnGetStrFromClipboard();
		str.Replace("\r\n��", "aoyuming1996��");
		str.Replace("\r\n", " ");
		str.Replace("aoyuming1996��", "\r\n��");
		fnCopyStrToClipboard(str);
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void CGroupingResult::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(_TEXT("ȷ���ر��𣿹ر��Ժ������Ϣ����գ�"), _TEXT("��ʾ"), MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		return;

	CDialogEx::OnClose();
}
