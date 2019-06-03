#pragma once
#include "member.h"


// CEraseDlg �Ի���

class CSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectDlg)

	int& m_Index;
	CString m_Hint;
	CString m_Verify;
	CString* m_Select;
	vector<stMember> m_CurMember;

public:
	CSelectDlg(const vector<stMember*>& cur, int& index, CString* select,  CString hint, 
			CString verify, CWnd* pParent = NULL);   // ��׼���캯��

	CSelectDlg(const vector<stMember>& cur, int& index, CString* select, CString hint,
			CString verify, CWnd* pParent = NULL);
	virtual ~CSelectDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ERASE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLbnSelchangeList1();
};
