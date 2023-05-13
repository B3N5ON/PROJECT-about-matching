#pragma once
#include "Headerfun.h"
#include <unordered_map>

typedef unordered_map<wstring, vector<wstring>> UNORDERWSMAP;

unordered_map<wstring, wstring> studentMatching;

unordered_map<wstring, wstring> schoolMatching;


void stableMatching(UNORDERWSMAP studentList, UNORDERWSMAP schoolList, unordered_map<wstring, wstring>& output)
{
	vector<wstring> freeStudent;

	for (auto& it : schoolList)  //��l��
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

	while (!freeStudent.empty())		//�C�ӥӽЪ̳��b�W��W or �L�ӽЪ��Ǯլҩڵ��L
	{
		for (auto& it : studentList)
		{
			wstring student = it.first;

			int favorite = 0, rejected = 0;

			if (studentMatching[student].empty())
			{
				while (it.second.size())		//�ǥͧ��@�Ǥ����Ǯ�
				{
					wstring school = it.second[favorite];

					if (find(schoolList[school].begin(), schoolList[school].end(), student) == schoolList[school].end()) //�p�G�ǮղM��S���o��ǥ�
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

							if (rejected == studentList[student].size())		//�Q�ڵ��Ƶ���L�ҥӽЪ��ƶq
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
