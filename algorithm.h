#pragma once
#include "Headerfun.h"
#include <unordered_map>

typedef unordered_map<wstring, vector<wstring>> UNORDERWSMAP;

unordered_map<wstring, wstring> studentMatching;

unordered_map<wstring, wstring> schoolMatching;

void stableMatching(map<wstring, student> studentList, map<wstring, school> schoolList, UNORDERWSMAP& output)
{
	vector<wstring> freeStudent;

	map<wstring, int> num;	//school's name, matching number

	for (auto& it : schoolList)  //初始化
	{
		schoolMatching[it.first] = {};

		num[it.first] = it.second.Number;

		if (num[it.first] > it.second.stdList.size())		//如果招收人數小於可錄取人數
		{
			num[it.first] = it.second.stdList.size();
		}
	}

	bool noFreeStudent = false, noSize = false;

	map<wstring, bool> noOneWantHim;

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
			for (auto& it : studentList)
			{
				if (freeStudent.empty())
					break;

				wstring student = it.first;

				int favorite = 0, rejected = 0;

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
									schoolMatching[school] = student;

									studentMatching[student] = school;

									current[school] = student;

									favorite = 0;

									freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

									noOneWantHim[student] = false;

									break;
								}
								else
								{
									auto currentIndex = current.find(school);

									wstring currentStudent = currentIndex->second;

									int currentStundetIndex = distance(schoolList[school].stdList.begin(), find(schoolList[school].stdList.begin(), schoolList[school].stdList.end(), currentStudent));

									int newStudentIndex = distance(schoolList[school].stdList.begin(), find(schoolList[school].stdList.begin(), schoolList[school].stdList.end(), student));

									if (currentStundetIndex > newStudentIndex)
									{
										schoolMatching[school] = student;

										studentMatching[student] = school;

										studentMatching[currentStudent] = L"";

										freeStudent.push_back(currentStudent);

										favorite = 0;

										current[school] = student;

										freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

										noOneWantHim[student] = false;

										break;
									}

									else
									{
										favorite++;			//find next school

										rejected++;

										if (rejected == studentList[student].Rank.size())		//被拒絕數等於他所申請的數量
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

				if(num[entry.second] != 0)
					num[entry.second]--;

				studentMatching[entry.first] = {}; //初始化

				int text = 0;
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
				noSize = true;
		}
	}
}
