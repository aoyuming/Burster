#pragma once
#include "afxwin.h"


// CGroupingResult 对话框

class CGroupingResult : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupingResult)

public:
	CGroupingResult(CWnd* pParent = NULL,CString str = "");   // 标准构造函数
	virtual ~CGroupingResult();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Data;
	CEdit m_Edit;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
