#pragma once
#include "stdafx.h"
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

//������Ա��Ϣ
void MemberCopy(vector<stMember>& vect1, const vector<stMember*>& vect2);
void MemberCopy(vector<stMember*>& vect1, const vector<stMember>& vect2);

//�ж�stMember�Ƿ���vect����
stMember* IsMemberInVect(stMember m, vector<stMember*>& ls);

//�ж�stMember�Ƿ���vect����
int IsMemberInVect_index(stMember m, vector<stMember*>& ls);

//���֧������
void PayScheme_g(vector<CString>* paySchemeString,
				vector<stMember*>* allMemberVect,
				int sum,
				CListBox* historyListBox);


//��Ϣ
struct stData
{
	CString fenZutime, vectoryTmie;
	vector<stMember> blue;
	vector<stMember> red;
	int redLose;
};
