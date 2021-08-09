#pragma once
#include "afxwin.h"


// CGroupingResult 对话框

class CGroupingResult : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupingResult)

public:
	// 获取剪切板上的字符串
	CString fnGetStrFromClipboard(void);

	// 设置剪切板内容
	void fnCopyStrToClipboard(CString szStr);

	CGroupingResult(CWnd* pParent = NULL,CString str = "");   // 标准构造函数

	virtual ~CGroupingResult();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
														//计时器消息
	DECLARE_MESSAGE_MAP()
public:
	CString m_Data;
	CEdit m_Edit;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};
