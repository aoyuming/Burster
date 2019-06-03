#include "stdafx.h"
#include "member.h"

void MemberCopy(vector<stMember>& vect1, const vector<stMember*>& vect2)
{
	vect1.clear();
	for (int i = 0; i < (int)vect2.size(); ++i)
		vect1.push_back(*(vect2)[i]);
}

void MemberCopy(vector<stMember*>& vect1, const vector<stMember>& vect2)
{
	for (int i = 0; i < (int)vect1.size(); ++i)
		*(vect1)[i] = vect2[i];
}

//判断stMember是否在vect里面
stMember* IsMemberInVect(stMember m, vector<stMember*>& ls)
{
	for (vector<stMember*>::iterator it = ls.begin(); it != ls.end(); ++it)
	{
		if (m.name == (*it)->name)
			return *it;
	}

	return NULL;
}

//判断stMember是否在vect里面
int IsMemberInVect_index(stMember m, vector<stMember*>& ls)
{
	for (int i = 0; i < ls.size(); ++i)
	{
		if (m.name == ls[i]->name)
			return i;
	}

	return -1;
}

//最佳支付方案
void PayScheme_g(vector<CString>* paySchemeString, 
				vector<stMember*>* allMemberVect, 
				int sum,
				CListBox* historyListBox)
{
	paySchemeString->clear();
	char buf[32];

	//把赢钱的和输钱的分开
	vector<stMember> lose, vectory;
	for (int i = 0; i < allMemberVect->size(); ++i)
	{
		if ((*allMemberVect)[i]->money < 0)
			lose.push_back(*(*allMemberVect)[i]);
		else if ((*allMemberVect)[i]->money > 0)
			vectory.push_back(*(*allMemberVect)[i]);
	}

	vector<stMember>::iterator loseIt = lose.begin();
	while (loseIt != lose.end())
	{
		bool flag = true;
		for (vector<stMember>::iterator it = vectory.begin(); it != vectory.end();)
		{
			if (-((*loseIt).money) == (*it).money)
			{
				_itoa_s((*it).money * sum, buf, 10);
				CString s = (*loseIt).name + _TEXT("  给  ") + (*it).name + _TEXT("  ") + buf;
				paySchemeString->push_back(s);

				loseIt = lose.erase(loseIt);
				it = vectory.erase(it);
				flag = false;
				break;
			}
			else
				++it;

		}

		if (flag)
			loseIt++;
	}

	loseIt = lose.begin();
	while (loseIt != lose.end())
	{
		bool flag = true;
		for (vector<stMember>::iterator it = vectory.begin(); it != vectory.end();)
		{
			if (-((*loseIt).money) == (*it).money)
			{
				_itoa_s((*it).money * sum, buf, 10);
				CString s = (*loseIt).name + _TEXT("  给  ") + (*it).name + _TEXT("  ") + buf;
				paySchemeString->push_back(s);

				loseIt = lose.erase(loseIt);
				it = vectory.erase(it);
				flag = false;
				break;
			}
			else if (-((*loseIt).money) < (*it).money)
			{
				_itoa_s(-(*loseIt).money * sum, buf, 10);
				CString s = (*loseIt).name + _TEXT("  给  ") + (*it).name + _TEXT("  ") + buf;
				paySchemeString->push_back(s);

				(*it).money += (*loseIt).money;
				loseIt = lose.erase(loseIt);
				flag = false;

				if ((*it).money <= 0)
					it = vectory.erase(it);
				break;
			}
			else
			{
				_itoa_s((*it).money * sum, buf, 10);
				CString s = (*loseIt).name + _TEXT("  给  ") + (*it).name + _TEXT("  ") + buf;
				paySchemeString->push_back(s);

				((*loseIt).money) += (*it).money;
				it = vectory.erase(it);

				if (((*loseIt).money) >= 0)
				{
					loseIt = lose.erase(loseIt);
					flag = false;
					break;
				}
			}
		}

		if (flag)
			loseIt++;
	}

	for (auto it = lose.begin(); it != lose.end(); ++it)
	{
		_itoa_s(abs((*it).money * sum), buf, 10);
		CString s = (*it).name + _TEXT("  给  ") + _T("[???]") + _TEXT("  ") + buf;
		paySchemeString->push_back(s);
	}

	for (auto it = vectory.begin(); it != vectory.end(); ++it)
	{
		_itoa_s(abs((*it).money * sum), buf, 10);
		CString s = _T("[???]  给  ") + (*it).name + +_TEXT("  ") + buf;
		paySchemeString->push_back(s);
	}

	historyListBox->ResetContent();

	for (int i = 0; i < paySchemeString->size(); ++i)
		historyListBox->InsertString(0, (*paySchemeString)[i]);
}