#pragma once
#include <vector>

using namespace std;


//��Ա
struct stMember
{
	CString name;//����
	int money;//��Ǯ

	bool operator < (const stMember& that)
	{
		return this->money < that.money;
	}
};

//��Ϣ
struct stData
{
	CString fenZutime, vectoryTmie;
	vector<stMember> bule;
	vector<stMember> red;
	int redLose;
};
