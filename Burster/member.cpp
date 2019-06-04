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

//�ж�stMember�Ƿ���vect����
stMember* IsMemberInVect(stMember m, vector<stMember*>& ls)
{
	for (vector<stMember*>::iterator it = ls.begin(); it != ls.end(); ++it)
	{
		if (m.name == (*it)->name)
			return *it;
	}

	return NULL;
}

//�ж�stMember�Ƿ���vect����
int IsMemberInVect_index(stMember m, vector<stMember*>& ls)
{
	for (int i = 0; i < ls.size(); ++i)
	{
		if (m.name == ls[i]->name)
			return i;
	}

	return -1;
}

//�����Ƿ����˸���һ�ξ��ܽ�����ĳ�Ա�±�
static int pairing(const vector<stMember>& vect, int loseMoeny)
{
	for (int i = 0; i < (int)vect.size(); ++i)
	{
		if (vect[i].money + loseMoeny == 0)
			return i;
	}
	return -1;
}

//���֧������
static void _PayScheme(vector<CString>* paySchemeString,//����֧������
						stMember& lose,//��ǰ������
						vector<stMember>& win,//����б�
						int sum)//��ע
{
	if (lose.money >= 0)
		return;

	for (int i = 0; i < (int)win.size(); ++i)
	{
		//��ǰ����ѽ���
		if (win[i].money <= 0)
			continue;

		//������Ӯ��ҵĽ�Ǯ ѡ������٣� �п��ܻ����һ�������� �����Ӯ�����
		int m = abs(lose.money) > abs(win[i].money) ? win[i].money : lose.money;
		m = abs(m);

		lose.money += m;
		win[i].money -= m;

		//����֧����Ϣ�ַ���
		CString moeny = "";
		moeny.Format("%d", m * sum);
		CString pay = lose.name + _T("  ��  ") + win[i].name + _T("  ") + moeny;
		paySchemeString->push_back(pay);

		//��ǰ���������ѻ���
		if (lose.money >= 0)
			return;
	}
}

//���֧������
void PayScheme(vector<CString>* paySchemeString, //֧����ϢҪ��ŵ�����
				const vector<stMember*>& allMemberVect,//��ǰ���г�Ա
				int sum,//��ע
				CListBox* payListBox)
{
	//���1��a - 6, b + 6 ...
	//	֧������ a=>b 6

	//���2��a - 6, b + 6�� c + 7 ...
	//	֧������ a=>b 6, ???? =>c 7

	//���3��b - 10�� c + 7 ...
	//	֧������ b=>c 7, b => ???? 3

	//���4��a - 6, b + 2 , c + 2, d + 1, e + 1 ......
	//	֧������ a=>b 2, a=>c 2, a=>d 1, a=>e 1

	//���5��a - 6, b + 10, c - 2, d - 1, e - 1....
	//	֧������ a=>b 6, c=>b 2, d=>b 1, e=>b 1

	paySchemeString->clear();

	//��ӮǮ�ĺ���Ǯ�ķֿ� ���䲻Ӯ�Ĳ���
	vector<stMember> lose, win;
	for (int i = 0; i < allMemberVect.size(); ++i)
	{
		if (allMemberVect[i]->money < 0)
			lose.push_back(*(allMemberVect)[i]);
		else if (allMemberVect[i]->money > 0)
			win.push_back(*(allMemberVect)[i]);
	}

	//���ܹ�ֱ��һ�ξͽ������2����ѡ����
	for (int i = 0; i < (int)lose.size();)
	{
		int idx = pairing(win, lose[i].money);
		if (idx != -1)
		{
			CString moeny = "";
			moeny.Format("%d", abs(lose[i].money) * sum);
			CString pay = lose[i].name + _T("  ��  ") + win[idx].name + _T("  ")+ moeny;
			paySchemeString->push_back(pay);
			lose.erase(lose.begin() + i);//ɾ����ǰ������
			win.erase(win.begin() + idx);//ɾ����ǰӮ�����
		}
		else
		{
			++i;
		}
	}

	//����ʣ�µģ� ��Щ�˿϶�������ӮǮ�ĺ���Ǯ�������Ϊ0�����
	for (int i = 0; i < (int)lose.size(); ++i)
		_PayScheme(paySchemeString, lose[i], win, sum);

	//�鿴��������û��û����ȥ��
	for (int i = 0; i < (int)lose.size(); ++i)
	{
		if (lose[i].money < 0)
		{
			CString moeny = "";
			moeny.Format("%d", abs(lose[i].money) * sum);
			CString pay = lose[i].name + _T("  ��  ") + _T("????  ") + moeny;
			paySchemeString->push_back(pay);
		}
	}

	//�鿴Ӯ�������û��û�յ���
	for (int i = 0; i < (int)win.size(); ++i)
	{
		if (win[i].money > 0)
		{
			CString moeny = "";
			moeny.Format("%d", abs(win[i].money) * sum);
			CString pay = _T("????");
			pay += _T("  ��  ") + win[i].name + _T("  ") + moeny;
			paySchemeString->push_back(pay);
		}
	}
	
	//�Ѽ���õ�֧����������listbox��ʾ
	payListBox->ResetContent();
	for (int i = (int)paySchemeString->size() - 1; i >= 0; --i)
		payListBox->InsertString(0, (*paySchemeString)[i]);
}
