#pragma once
#include "afxwin.h"


// CEditMenber 对话框

class CEditMember : public CDialogEx
{
	DECLARE_DYNAMIC(CEditMember)

	enum EWin {LOSE, WIN, NO_WIN_LOSE};

	EWin m_Select;
	int m_Sum;//底注
	CString& m_sName;
	CString& m_sMoney;

	// 名字编辑框
	CEdit m_Name_Edit;

	// 金钱编辑框
	CEdit m_Money_Edit;

public:
	CEditMember(CString& name, CString& money, int nSum, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditMember();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_MENBER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1_lose();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	// 选择金额
	CComboBox m_ComboBox;
	afx_msg void OnCbnCloseupCombo1();
};
