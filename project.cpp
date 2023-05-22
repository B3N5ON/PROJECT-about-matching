#include "Headerfun.h"
#include "algorithm.h"
#include <locale>	
#include <codecvt>

// �����٭n�إ߾ǮեN�X��W�ӮզW�١B��t���ɮ�
vector<WS> rankList;			///���@�ǱƦC

map<wstring, student> StudentMap;			//Map<�ҥͥN�X�A���즨�Z���>

map<wstring, vector<int>> FiveStandards;		//map<��ئW�١A����>

map<wstring, school> SchoolDetail;			//map<�Ǯ�ID, �ղӤ��h�ԲӸ��>

int main()
{
	ios_base::sync_with_stdio(false);

	locale loc(locale(), new codecvt_utf8<wchar_t>);

	wcout.imbue(loc);

	SetConsoleCP(65001);

	SetConsoleOutputCP(65001);

	buiild_student_rank(rankList);		//Get the student's rank type is vector<vector<string>>

	build_student_list(StudentMap);			//Get the student's list type is map<wstring,  class student>

	FiveStanderMap(FiveStandards);				//Get the Stander of grade and convert it to the integer

	build_school_rank(SchoolDetail,FiveStandards);			//Put in the map<wstring, school> if the 5 stander convert chinese to the interger 

	//build school's preferance of the students
	for (int i = 0; i < rankList.size(); i++)
	{
		auto st = StudentMap.find(rankList[i][0]);		
		//get student detail
		for (int j = 1; j < rankList[i].size(); j++)  //first is ID, so loop start on the 1
		{
			auto sch = SchoolDetail.find(rankList[i][j]);		//get school detail

			if (sch == SchoolDetail.end())			//can't find the school
			{
				break;
			}
			//Determine whether student's grade are confrom with school's grade
			//���пz��
			//�p�G��妨�Z�j�󵹪����зǩΪ̾Ǯդ������(��媺���v��0)
			if (st->second.Chinese >= sch->second.ChineseG || sch->second.ChineseG == 0)
			{
				if (st->second.English >= sch->second.EnglishG || sch->second.EnglishG == 0) 
				{
					if (st->second.MathA >= sch->second.MathAG || sch->second.MathAG == 0) 
					{
						if (st->second.MathB >= sch->second.MathBG || sch->second.MathBG == 0)
						{
							if (st->second.Social >= sch->second.SocialG || sch->second.SocialG == 0)
							{
								if (st->second.Nature >= sch->second.NatureG || sch->second.NatureG == 0)
								{
									sch->second.stdList.push_back(st->first);			//the sort at here haven't sortted regularly
								}
							}
						}
					}
				}								
			}									
		}
	}
	//sorting stdList let the sort inside is descending(high -> low score) to complete school's preferance
	for (auto& t : SchoolDetail) {
		int wantNum = static_cast<int>(t.second.stdList.size());		///�o��L���ƼзǪ��H��

		int auditionNum = 0;

		vector<double> ratioArray = { t.second.chRatio, t.second.EnRatio, t.second.MARatio, t.second.MBRatio, t.second.SoRatio, t.second.NaRatio };//might add more

		vector<double> ratio5Array = { t.second.chRatio, t.second.EnRatio, t.second.MARatio, t.second.MBRatio, t.second.SoRatio, t.second.NaRatio };
		
		multimap<double, size_t> m;		//�o�줭��ۦP��v����m�A�Y�S���ۦPsize��1

		map<double, vector<int>> sameRat;

		for (size_t i = 0; i < ratio5Array.size(); i++) {
			if(ratio5Array[i] != 0)
				m.insert({ ratio5Array[i], i });
		}
		for (auto it = m.begin(); it != m.end();)
		{
			auto p = m.equal_range(it->first);

			vector<int> temp;

			while (p.first != p.second)
			{
				temp.push_back(static_cast<int>(p.first++->second));
			}
			sameRat.insert(pair<double, vector<int>>(it->first, temp));//(��v�A�ۦP��v����m)

			it = p.second;
		}
		//���D���X�ؤ��P���Ҭ�A��v�ۦP��b�@�_
		for (auto it = sameRat.rbegin(); it != sameRat.rend(); ++it)			//�]��map�w�g�ƦC��key�Ѥp��j�A�G��rbegin�q����(�̤j��)�}�l��
		{		
			auditionNum = int(ceil(double(t.second.Number) * it->first)); //�Y��v���p���I�A�h������L����i��

			vector<double> SubjectGrade;			

			if (it->second.size() == 1) //�Y�S���ۦP�����v
			{
				int tempG = it->second[0];

				for (int i = 0; i < t.second.stdList.size(); i++)
				{
					double temp;

					auto k = StudentMap.find(t.second.stdList[i]);

					double s_sub = it->first;		//����Ӭ쪺���v

					switch (tempG)
					{
					case 0:
						temp = k->second.Chinese * s_sub;

						SubjectGrade.push_back(temp);

						continue;
					case 1:
						temp = k->second.English * s_sub;

						SubjectGrade.push_back(temp);

						continue;
					case 2:
						temp = k->second.MathA * s_sub;

						SubjectGrade.push_back(temp);

						continue;
					case 3:
						temp = k->second.MathB * s_sub;

						SubjectGrade.push_back(temp);

						continue;
					case 4:
						temp = k->second.Social * s_sub;

						SubjectGrade.push_back(temp);

						continue;
					case 5:
						temp = k->second.Nature * s_sub;

						SubjectGrade.push_back(temp);

						continue;
					}
				}
			}
			//�p�G���ۦP��v�A�h�ۦP��v�����n�ۥ[
			else if(it->second.size() > 1)
			{
				vector<int> mutiSub = it->second;

				for (int i = 0; i < t.second.stdList.size(); i++)
				{
					auto k = StudentMap.find(t.second.stdList[i]);		

					double SumSubs = 0;

					for (int p : mutiSub)  //get which subs
					{
						if (p == 0) 
						{
							SumSubs += k->second.Chinese;
						}
						else if (p == 1) 
						{
							SumSubs += k->second.English;
						}						
						else if (p == 2) 
						{
							SumSubs += k->second.MathA;
						}							
						else if (p == 3)
						{
							SumSubs += k->second.MathB;
						}							
						else if (p == 4)
						{
							SumSubs += k->second.Social;
						}							
						else if (p == 5)
						{
							SumSubs += k->second.Nature;
						}
					}
					double ratio = it->first;

					SumSubs *= ratio;	// ratio * ( same ratio's subjects sum)

					SubjectGrade.push_back(SumSubs);
				}
			}
			///Use Bubble Sort to sort student's ID, sum of score.
			for (int j = SubjectGrade.size()-1; j>0; j--)
			{
				for (int k = 0; k <= j - 1; k++)
				{
					if (SubjectGrade[k] < SubjectGrade[k + 1])
					{
						swap(SubjectGrade[k], SubjectGrade[k + 1]);

						swap(t.second.stdList[k], t.second.stdList[k + 1]);
					}
				}
			}

			if (t.second.stdList.size() > auditionNum)
			{
				double LastStudentGrade = SubjectGrade[auditionNum - 1];

				vector<int> AddresReg;		//�����ۦP���ƪ���m

				for (int k = 0; k < SubjectGrade.size(); k++)
				{
					if (SubjectGrade[k] == LastStudentGrade)
					{
						AddresReg.push_back(k);		//if same grade then put the address into vector
					}
				}
				int a = AddresReg.back();			//get the last element's value, to know how many people have same grade

				t.second.stdList.erase(t.second.stdList.begin() + a, t.second.stdList.end());		//then erase the people whose score is less than the last person's score
			}
		}
	}

	wcout << L"\n" << L"�Ǯչ�ǥͪ�����" << endl;

	for (auto& it : SchoolDetail) {

		wcout << it.second.SchoolName << L":";

		for (int i = 0; i < it.second.stdList.size(); i++) 
		{
			auto it2 = StudentMap.find(it.second.stdList[i]);

			wcout << it2->second.name << L" ";
		}
		wcout << L"\n";
	}

//Do algorithm
//�w�g����school��ǥͪ�preference�M����ǥ͹�school��preference,�{�b�}�l���t��k���Ƨ�
//school's preference has completed its priority of student, and those student has admissed  minimum requirement

	unordered_map<wstring, vector<wstring>> s_rank;

	for (int i = 0; i < rankList.size(); i++)
	{
		vector<wstring> temp_rank = rankList[i];

		temp_rank.erase(temp_rank.begin());

		wstring sID = rankList[i][0];

		auto it = StudentMap.find(sID);

		it->second.Rank = temp_rank;
	}

	UNORDERWSMAP match;

	///*This is testing data */
	//unordered_map<wstring, vector<wstring>> Test_data_sc = {
	//{L"A", {L"a", L"b", L"c", L"d",L"e",L"f"}},
	//{L"B", {L"f", L"e", L"d", L"c",L"b",L"a"}}
	//};
	//unordered_map<wstring, vector<wstring>> Test_data_sd = {
	//{L"a", {L"A", L"B"}},
	//{L"b", {L"A", L"B"}},
	//{L"c", {L"B", L"A"}},
	//{L"d", {L"B", L"A"}},
	//	{L"e",{L"A",L"B"}},
	//	{L"f", {L"B", L"A"}},
	//	};

stableMatching(StudentMap, SchoolDetail, match);

wcout << L"\n" << L"�ξǮչ�ǥͪ�����ƦW�M�ǥͪ����@���t��k�Ƨ�" << endl;

for (auto& it2 : match)
{
	auto sTN = StudentMap.find(it2.first);

	wstring studentname = sTN->second.name;

	wstring schoolName = L"NUN";

	wcout << sTN->second.id << L"  " << studentname <<L":" << L"\n";

	for (auto& it3 : it2.second)
	{
		if (it3 != L"")
		{
			auto sCN = SchoolDetail.find(it3);

			schoolName = sCN->second.SchoolName;

			wcout << schoolName << L"	";
		}
	}
	wcout << endl;
}

	SetConsoleCP(950);

	SetConsoleOutputCP(950);
}

