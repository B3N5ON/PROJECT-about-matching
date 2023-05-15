#pragma once
#include "Headerfun.h"
#include <unordered_map>

typedef unordered_map<wstring, vector<wstring>> UNORDERWSMAP;

unordered_map<wstring, wstring> studentMatching;

unordered_map<wstring, wstring> schoolMatching;

void stableMatching(UNORDERWSMAP studentList, UNORDERWSMAP schoolList, UNORDERWSMAP& output)
{
	vector<wstring> freeStudent;

	map<wstring, int> num;	//school's name, matching number

	num[L"A"] = 2;
	num[L"B"] = 3;

	for (auto& it : schoolList)  //��l��
	{
		schoolMatching[it.first] = {};
	}

	bool noFreeStudent = false, noOneWantHim = false, noSize = false;

	while (!noSize)
	{
		for (auto& it : studentList)
		{
			studentMatching[it.first] = {};

			freeStudent.push_back(it.first);
		}

		map<wstring, wstring > current;

		while (!freeStudent.empty())		//�C�ӥӽЪ̳��b�W��W or �L�ӽЪ��Ǯլҩڵ��L
		{
			for (auto& it : studentList)
			{
				if (freeStudent.empty())
					break;

				wstring student = it.first;

				int favorite = 0, rejected = 0;

				if (studentMatching[student].empty())
				{
					while (it.second.size())		//�ǥͧ��@�Ǥ����Ǯ�
					{
						wstring school = it.second[favorite];

						if (num[school] != 0)		//�p�G�ǮդH���٨S�ۧ�
						{
							if (find(schoolList[school].begin(), schoolList[school].end(), student) != schoolList[school].end())  ///�ǮղM��䪺��ǥ�
							{
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
											noOneWantHim = true; /**/

											freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

											break;
										}
									}
								}
							}
							else
							{	///�ǮղM��䤣��ǥ�(���Z������)
								favorite++;

								rejected++;

								if (rejected == studentList[student].size())		//�Q�ڵ��Ƶ���L�ҥӽЪ��ƶq
								{
									noOneWantHim = true; /**/

									freeStudent.erase(find(freeStudent.begin(), freeStudent.end(), student));

									break;
								}
							}
						}
						else
						{	//�Ǯդw�ۺ�
							favorite++;

							rejected++;

							if (rejected == studentList[student].size())		//�Q�ڵ��Ƶ���L�ҥӽЪ��ƶq
							{
								noOneWantHim = true; /**/

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
			output[entry.first].push_back(entry.second);

			schoolMatching[entry.second] = {};

			if (!entry.second.empty())
			{
				schoolList[entry.second].erase(find(schoolList[entry.second].begin(), schoolList[entry.second].end(), entry.first));	///��w�g�t��쪺�ǥͱq�ǮզC���R��

				if(num[entry.second] != 0)
					num[entry.second]--;

				studentMatching[entry.first] = {}; //��l��

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
