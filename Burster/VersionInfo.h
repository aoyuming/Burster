#pragma once
#include "afxwin.h"


// CVersionInfo 对话框
extern const CString LOCAL_LIVE_UPDATE;


class CVersionInfo : public CDialog
{
	DECLARE_DYNAMIC(CVersionInfo)

public:
	CVersionInfo(CString verInfo,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVersionInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VERSION_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_VerInfo;
	CEdit m_VerInfoEdit;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
