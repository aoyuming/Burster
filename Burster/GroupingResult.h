#pragma once
#include "afxwin.h"


// CGroupingResult �Ի���

class CGroupingResult : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupingResult)

public:
	CGroupingResult(CWnd* pParent = NULL,CString str = "");   // ��׼���캯��
	virtual ~CGroupingResult();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Data;
	CEdit m_Edit;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
