
// BursterDlg.h : ͷ�ļ�

#pragma once
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include "addName.h"
#include "SelectDlg.h"
#include "member.h"
#include "Config.h"

// CBursterDlg �Ի���
class CBursterDlg : public CDialogEx
{
// ����
public:
	CBursterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	addName* m_addNameDlg;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	//�Ľ�
	int m_Sum;

	//��ǰ��Ա�б�
	vector<stMember*> m_CurMemberVect;

	//���г�Ա��
	vector<stMember*> m_AllMemberVect;

	//���ӱ�
	vector<stMember*> m_BlueMemberVect;

	//��ӱ�
	vector<stMember*> m_RedMemberVect;

	//���ӱ�
	vector<stMember*> m_DoveMemberVect;

	//֧������
	vector<CString> m_PaySchemeString;

	//��ս��Ϣ
	vector<stData> m_Data;

	//����
	void Save(const char* fn,const char* rb);

	//���������ļ�
	void SaveConfiguration();

	//���������ļ�
	bool LoadConfiguration();

	//�ж�stMember�Ƿ���vect����
	stMember* IsMemberInVect(stMember m,vector<stMember*>& ls);
	int IsMemberInVect_index(stMember m, vector<stMember*>& ls);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton8_Add();
	afx_msg void OnBnClickedButton2_Delete();
	afx_msg void OnBnClickedButton3_Clear();
	afx_msg void OnBnClickedButton1_Save();
	afx_msg void OnBnClickedButton4_Separate();
	afx_msg void OnBnClickedButton6_Change();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton5_Blue();
	afx_msg void OnBnClickedButton7_Red();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton11();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};
