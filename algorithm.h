#pragma once
#include "Headerfun.h"
#include <unordered_map>

typedef unordered_map<wstring, vector<wstring>> UNORDERWSMAP;

unordered_map<wstring, wstring> studentMatching;

unordered_map<wstring, wstring> schoolMatching;


void stableMatching(UNORDERWSMAP studentList, UNORDERWSMAP schoolList, unordered_map<wstring, wstring>& output)
{
	vector<wstring> freeStudent;

	for (auto& it : schoolList)  //初始化
	{
		schoolMatching[it.first] = {};
	}
	for (auto& it : studentList)
	{
		studentMatching[it.first] = {};

		freeStudent.push_back(it.first);
	}

	bool noFreeStudent = false, noOneWantHim = false;

	map<wstring, wstring > current;

	while (!freeStudent.empty())		//每個申請者都在名單上 or 他申請的學校皆拒絕他
	{
		for (auto& it : studentList)
		{
			wstring student = it.first;

			int favorite = 0, rejected = 0;

			if (studentMatching[student].empty())
			{
				while (it.second.size())		//學生志願序中的學校
				{
					wstring school = it.second[favorite];

					if (find(schoolList[school].begin(), schoolList[school].end(), student) == schoolList[school].end()) //如果學校清單沒有這位學生
					{
						break;
					}

					if (schoolMatching[school].empty())
					{
						schoolMatching[school] = student;

						studentMatching[student] = school;

						current[school] = student;

						favorite = 0;

						freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

						break;
					}
					else
					{
						auto currentIndex = current.find(school);

						wstring currentStudent = currentIndex->second;

						int currentStundetIndex = distance(schoolList[school].begin(), find(schoolList[school].begin(), schoolList[school].end(), currentStudent));

						int newStudentIndex = distance(schoolList[school].begin(), find(schoolList[school].begin(), schoolList[school].end(), student));

						if (currentStundetIndex > newStudentIndex)
						{
							schoolMatching[school] = student;

							studentMatching[student] = school;

							studentMatching[currentStudent] = L"";

							freeStudent.push_back(currentStudent);

							favorite = 0;

							current[school] = student;

							freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

							break;
						}
						else
						{
							favorite++;			//find next school

							rejected++;

							if (rejected == studentList[student].size())		//被拒絕數等於他所申請的數量
							{
								noOneWantHim = true;
							}

							if (noOneWantHim)
							{
								freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

								break;
							}
						}
					}
				}
			}
		}
	}
	for (auto& entry : studentMatching)
	{
		output[entry.first] = entry.second;
	}
}
