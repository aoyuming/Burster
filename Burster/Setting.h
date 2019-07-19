#pragma once
#include "BursterDlg.h"
#include "agreement.h"

// CSetting �Ի���

class CSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CSetting)

public:
	CSetting(UpdateMode& mode, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetting();

	UpdateMode& m_Mode;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
