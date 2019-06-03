#pragma once
#include "afxwin.h"


// CEditMenber �Ի���

class CEditMember : public CDialogEx
{
	DECLARE_DYNAMIC(CEditMember)

	enum EWin {LOSE, WIN, NO_WIN_LOSE};

	EWin m_Select;
	int m_Sum;//��ע
	CString& m_sName;
	CString& m_sMoney;

	// ���ֱ༭��
	CEdit m_Name_Edit;

	// ��Ǯ�༭��
	CEdit m_Money_Edit;

public:
	CEditMember(CString& name, CString& money, int nSum, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditMember();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_MENBER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1_lose();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	// ѡ����
	CComboBox m_ComboBox;
	afx_msg void OnCbnCloseupCombo1();
};
