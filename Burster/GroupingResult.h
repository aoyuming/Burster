#pragma once
#include "afxwin.h"


// CGroupingResult �Ի���

class CGroupingResult : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupingResult)

public:
	// ��ȡ���а��ϵ��ַ���
	CString fnGetStrFromClipboard(void);

	// ���ü��а�����
	void fnCopyStrToClipboard(CString szStr);

	CGroupingResult(CWnd* pParent = NULL,CString str = "");   // ��׼���캯��

	virtual ~CGroupingResult();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
														//��ʱ����Ϣ
	DECLARE_MESSAGE_MAP()
public:
	CString m_Data;
	CEdit m_Edit;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};
