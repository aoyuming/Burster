#pragma once
#include <vector>

using namespace std;

// addName �Ի���

class addName : public CDialogEx
{
	DECLARE_DYNAMIC(addName)

public:
	addName(vector<CString>& names, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~addName();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CEdit* m_NameEdit;
	vector<CString>& m_Name;
	afx_msg void OnEnUpdateEdit1();
};
