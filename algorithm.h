#pragma once
#include "Headerfun.h"
#include <unordered_map>
#define MAXVALUE 4

typedef unordered_map<wstring, vector<wstring>> UNORDERWSMAP;

unordered_map<wstring, wstring> studentMatching;

unordered_map<wstring, wstring> schoolMatching;

void stableMatching(map<wstring, student> studentList, map<wstring, school> schoolList, UNORDERWSMAP& output)
{
	vector<wstring> freeStudent;

	map<wstring, int> num;	//school's name, matching number

	map<wstring, int> admissionNum;

	for (auto& it : schoolList)  //初始化
	{
		schoolMatching[it.first] = {};

		num[it.first] = it.second.Number;

		if (num[it.first] > it.second.stdList.size())		//如果招收人數小於可錄取人數
		{
			num[it.first] = it.second.stdList.size();
		}
	}

	map<wstring, bool> noOneWantHim, exceedSize;

	for (auto& it : studentList)
	{
		admissionNum[it.first] = 0;

		exceedSize[it.first] = false;
	}

	bool noFreeStudent = false, noSize = false;

	int dieLoop = 0;

	while (!noSize)
	{
		for (auto& it : studentList)
		{
			studentMatching[it.first] = {};

			freeStudent.push_back(it.first);

			noOneWantHim[it.first] = false;
		}

		map<wstring, wstring > current;

		while (!freeStudent.empty())		//每個申請者都在名單上 or 他申請的學校皆拒絕他
		{
			if (noSize)
				break;

			for (auto& it : studentList)
			{
				if (freeStudent.empty())		//沒有自由人
				{
					break;
				}

				wstring student = it.first;

				int favorite = 0, rejected = 0;

				if (admissionNum[student] != MAXVALUE) {//
					if (studentMatching[student].empty())
					{
						while (it.second.Rank.size())		//學生志願序中的學校
						{
							wstring school = it.second.Rank[favorite];

							if (num[school] != 0)		//如果學校人數還沒招完
							{
								if (find(schoolList[school].stdList.begin(), schoolList[school].stdList.end(), student) != schoolList[school].stdList.end())  ///學校清單找的到學生
								{
									if (schoolMatching[school].empty())
									{
										//學校還沒配對
										schoolMatching[school] = student;

										studentMatching[student] = school;

										current[school] = student;

										favorite = 0;

										freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

										noOneWantHim[student] = false;

										admissionNum[student]++;

										break;
									}
									else
									{	//學校已經配對
										auto currentIndex = current.find(school);

										wstring currentStudent = currentIndex->second;

										int currentStundetIndex = distance(schoolList[school].stdList.begin(),

											find(schoolList[school].stdList.begin(), schoolList[school].stdList.end(), currentStudent));

										int newStudentIndex = distance(schoolList[school].stdList.begin(),

											find(schoolList[school].stdList.begin(), schoolList[school].stdList.end(), student));

										if (currentStundetIndex > newStudentIndex)
										{		//如果比原先的學生順位更高
											schoolMatching[school] = student;

											studentMatching[student] = school;

											studentMatching[currentStudent] = L"";

											freeStudent.push_back(currentStudent);

											favorite = 0;

											current[school] = student;

											freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

											noOneWantHim[student] = false;

											admissionNum[currentStudent]--;

											admissionNum[student]++;

											break;
										}

										else
										{		//比原先學生順位低
											favorite++;			//find next school

											rejected++;

											if (rejected == studentList[student].Rank.size())		//被拒絕數等於他所申請的數量(全被拒絕)
											{
												if (noOneWantHim[student])		//之前就有人不要了
												{
													break;
												}
												noOneWantHim[student] = true; /**/

												freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

												break;
											}
										}
									}
								}
								else
								{	///學校清單找不到學生(成績不夠格)
									favorite++;

									rejected++;

									if (rejected == studentList[student].Rank.size())		//被拒絕數等於他所申請的數量
									{
										if (noOneWantHim[student])		//之前就有人不要了
										{
											break;
										}

										noOneWantHim[student] = true; /**/

										freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

										/*加某變數 判斷學校在出現無限迴圈時有無被分配到*/

										break;
									}
								}
							}
							else
							{	//學校已招滿
								favorite++;

								rejected++;

								if (noOneWantHim[student])		//之前就有人不要了
								{
									break;
								}

								if (rejected == studentList[student].Rank.size())		//被拒絕數等於他所申請的數量
								{
									noOneWantHim[student] = true; /**/

									freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

									break;
								}
							}
						}
					}
				}//
				else
				{
					if (exceedSize[student] == true)
					{
						noSize = true;
						if (find(freeStudent.begin(), freeStudent.end(), student) != freeStudent.end())
						{
							freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));
						}
						break;
					}
					if (find(freeStudent.begin(), freeStudent.end(), student) != freeStudent.end())
					{
						freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));
					}
					exceedSize[student] = true;
				}
			}
		}

		for (auto& entry : studentMatching)
		{
			output[entry.first].push_back(entry.second);   //輸出

			schoolMatching[entry.second] = {};

			if (!entry.second.empty())
			{
				schoolList[entry.second].stdList.erase(find(schoolList[entry.second].stdList.begin(), 

					schoolList[entry.second].stdList.end(), entry.first));	///把已經配對到的學生從學校列表中刪除

				if (num[entry.second] != 0)		///如果有排到人，剩餘人數減一
				{
					num[entry.second]--;
				}

				studentMatching[entry.first] = {}; //初始化
			}
			else
			{
				
			}
		}

		int count = 0;

		for (auto& entry : num)
		{
			if (entry.second == 0)
			{
				count++;
			}

			if (count == num.size())
			{
				noSize = true;
			}
		}
	}
}
