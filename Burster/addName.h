#pragma once
#include <vector>

using namespace std;

// addName 对话框

class addName : public CDialogEx
{
	DECLARE_DYNAMIC(addName)

public:
	addName(vector<CString>& names, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~addName();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CEdit* m_NameEdit;
	vector<CString>& m_Name;
	afx_msg void OnEnUpdateEdit1();
};
