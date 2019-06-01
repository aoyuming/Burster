
// BursterDlg.h : 头文件
//

#pragma once
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include "addName.h"
#include "SelectDlg.h"
#include "menber.h"
#include "projectCommand.h"
#include "Config.h"

// CBursterDlg 对话框
class CBursterDlg : public CDialogEx
{
// 构造
public:
	CBursterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	addName* m_addNameDlg;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	//赌金
	int m_Sum;

	//当前成员列表
	vector<stMember*> m_CurMenberVect;

	//所有成员表
	vector<stMember*> m_AllMenberVect;

	//蓝队表
	vector<stMember*> m_BuleMenberVect;

	//红队表
	vector<stMember*> m_RedMenberVect;

	//鸽子表
	vector<stMember*> m_DoveMenberVect;

	//支付方案
	vector<CString> m_PaySchemeString;

	//对战信息
	vector<stData> m_Data;

	//保存
	void Save(const char* fn,const char* rb);

	//保存配置文件
	void SaveConfiguration();

	//加载配置文件
	bool LoadConfiguration();

	//判断stMember是否在vect里面
	stMember* IsMemberInVect(stMember m,vector<stMember*>& ls);
	int IsMemberInVect_index(stMember m, vector<stMember*>& ls);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	afx_msg void OnBnClickedButton5_Bule();
	afx_msg void OnBnClickedButton7_Red();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton11();
};
