#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;


//成员
struct stMember
{
	CString name;//名字
	int money;//金钱

	bool operator < (const stMember& that)
	{
		return this->money < that.money;
	}
};

//拷贝成员信息
void MemberCopy(vector<stMember>& vect1, const vector<stMember*>& vect2);
void MemberCopy(vector<stMember*>& vect1, const vector<stMember>& vect2);

//判断stMember是否在vect里面
stMember* IsMemberInVect(stMember m, vector<stMember*>& ls);

//判断stMember是否在vect里面
int IsMemberInVect_index(stMember m, vector<stMember*>& ls);

//最佳支付方案
void PayScheme_g(vector<CString>* paySchemeString,
				vector<stMember*>* allMemberVect,
				int sum,
				CListBox* historyListBox);


//信息
struct stData
{
	CString fenZutime, vectoryTmie;
	vector<stMember> blue;
	vector<stMember> red;
	int redLose;
};
