#pragma once
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

//信息
struct stData
{
	CString fenZutime, vectoryTmie;
	vector<stMember> bule;
	vector<stMember> red;
	int redLose;
};
