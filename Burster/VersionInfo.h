#pragma once
#include "afxwin.h"


// CVersionInfo �Ի���
extern const CString LOCAL_LIVE_UPDATE;


class CVersionInfo : public CDialog
{
	DECLARE_DYNAMIC(CVersionInfo)

public:
	CVersionInfo(CString verInfo,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVersionInfo();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VERSION_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_VerInfo;
	CEdit m_VerInfoEdit;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
