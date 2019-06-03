#pragma once
#include "member.h"
#include "afxcmn.h"
#include "BursterDlg.h"

// CConfig 对话框


class CConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CConfig)

	CBursterDlg* m_MainDlg;
	#define _M m_MainDlg

	bool& m_SaveFlag;//是否保存

	vector<stMember> m_CurMember_Temp;
	vector<stMember> m_AllMember_Temp;
	vector<stMember> m_RedMember_Temp;
	vector<stMember> m_BlueMember_Temp;
	vector<stMember> m_RemainMember_Temp;//剩下可以添加的玩家列表
	vector<stMember> m_EraseMember;

	CListBox* m_AllListBoxExternal;//外部的所有成员listbox
	CListBox* m_RedListBox;
	CListBox* m_BlueListBox;

public:
	CConfig(bool& saveFlag, CBursterDlg* pParent = NULL);   // 标准构造函数
	virtual ~CConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton_RedAdd();
	afx_msg void OnBnClickedButtonBlueAdd();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton12();

	int find(CString name, const vector<stMember>& vect);
	int find(CString name, const vector<stMember*>* vect);

	afx_msg BOOL OnInitDialog();
	// 历史记录
	CListCtrl m_History_LC;
	afx_msg void OnNMClickList5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton1();
};
