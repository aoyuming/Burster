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

//查找是否有人跟他一次就能结算完的成员下标
static int pairing(const vector<stMember>& vect, int loseMoeny)
{
	for (int i = 0; i < (int)vect.size(); ++i)
	{
		if (vect[i].money + loseMoeny == 0)
			return i;
	}
	return -1;
}

//最佳支付方案
static void _PayScheme(vector<CString>* paySchemeString,//最终支付方案
						stMember& lose,//当前输的玩家
						vector<stMember>& win,//玩家列表
						int sum)//底注
{
	if (lose.money >= 0)
		return;

	for (int i = 0; i < (int)win.size(); ++i)
	{
		//当前玩家已结清
		if (win[i].money <= 0)
			continue;

		//根据输赢玩家的金钱 选择给多少， 有可能会出现一个输的玩家 给多个赢得玩家
		int m = abs(lose.money) > abs(win[i].money) ? win[i].money : lose.money;
		m = abs(m);

		lose.money += m;
		win[i].money -= m;

		//创建支付信息字符串
		CString moeny = "";
		moeny.Format("%d", m * sum);
		CString pay = lose.name + _T("  给  ") + win[i].name + _T("  ") + moeny;
		paySchemeString->push_back(pay);

		//当前输的玩家帐已还完
		if (lose.money >= 0)
			return;
	}
}

//最佳支付方案
void PayScheme(vector<CString>* paySchemeString, //支付信息要存放的数组
				const vector<stMember*>& allMemberVect,//当前所有成员
				int sum,//底注
				CListBox* payListBox)
{
	//情况1：a - 6, b + 6 ...
	//	支付方案 a=>b 6

	//情况2：a - 6, b + 6， c + 7 ...
	//	支付方案 a=>b 6, ???? =>c 7

	//情况3：b - 10， c + 7 ...
	//	支付方案 b=>c 7, b => ???? 3

	//情况4：a - 6, b + 2 , c + 2, d + 1, e + 1 ......
	//	支付方案 a=>b 2, a=>c 2, a=>d 1, a=>e 1

	//情况5：a - 6, b + 10, c - 2, d - 1, e - 1....
	//	支付方案 a=>b 6, c=>b 2, d=>b 1, e=>b 1

	paySchemeString->clear();

	//把赢钱的和输钱的分开 不输不赢的不管
	vector<stMember> lose, win;
	for (int i = 0; i < allMemberVect.size(); ++i)
	{
		if (allMemberVect[i]->money < 0)
			lose.push_back(*(allMemberVect)[i]);
		else if (allMemberVect[i]->money > 0)
			win.push_back(*(allMemberVect)[i]);
	}

	//把能够直接一次就结算完的2个人选出来
	for (int i = 0; i < (int)lose.size();)
	{
		int idx = pairing(win, lose[i].money);
		if (idx != -1)
		{
			CString moeny = "";
			moeny.Format("%d", abs(lose[i].money) * sum);
			CString pay = lose[i].name + _T("  给  ") + win[idx].name + _T("  ")+ moeny;
			paySchemeString->push_back(pay);
			lose.erase(lose.begin() + i);//删除当前输的玩家
			win.erase(win.begin() + idx);//删除当前赢得玩家
		}
		else
		{
			++i;
		}
	}

	//计算剩下的， 这些人肯定不会有赢钱的和输钱的人相加为0的情况
	for (int i = 0; i < (int)lose.size(); ++i)
		_PayScheme(paySchemeString, lose[i], win, sum);

	//查看输的玩家有没有没给出去的
	for (int i = 0; i < (int)lose.size(); ++i)
	{
		if (lose[i].money < 0)
		{
			CString moeny = "";
			moeny.Format("%d", abs(lose[i].money) * sum);
			CString pay = lose[i].name + _T("  给  ") + _T("????  ") + moeny;
			paySchemeString->push_back(pay);
		}
	}

	//查看赢的玩家有没有没收到的
	for (int i = 0; i < (int)win.size(); ++i)
	{
		if (win[i].money > 0)
		{
			CString moeny = "";
			moeny.Format("%d", abs(win[i].money) * sum);
			CString pay = _T("????");
			pay += _T("  给  ") + win[i].name + _T("  ") + moeny;
			paySchemeString->push_back(pay);
		}
	}
	
	//把计算好的支付方案放入listbox显示
	payListBox->ResetContent();
	for (int i = (int)paySchemeString->size() - 1; i >= 0; --i)
		payListBox->InsertString(0, (*paySchemeString)[i]);
}
