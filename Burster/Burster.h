
// Burster.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBursterApp:
// �йش����ʵ�֣������ Burster.cpp
//

class CBursterApp : public CWinApp
{
public:
	CBursterApp();

	int ExitInstance();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBursterApp theApp;