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

	for (auto& it : schoolList)  //��l��
	{
		schoolMatching[it.first] = {};

		num[it.first] = it.second.Number;

		if (num[it.first] > it.second.stdList.size())		//�p�G�ۦ��H�Ƥp��i�����H��
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
					while (it.second.Rank.size())		//�ǥͧ��@�Ǥ����Ǯ�
					{
						wstring school = it.second.Rank[favorite];

						if (num[school] != 0)		//�p�G�ǮդH���٨S�ۧ�
						{
							if (find(schoolList[school].stdList.begin(), schoolList[school].stdList.end(), student) != schoolList[school].stdList.end())  ///�ǮղM��䪺��ǥ�
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

										if (rejected == studentList[student].Rank.size())		//�Q�ڵ��Ƶ���L�ҥӽЪ��ƶq
										{
											if (noOneWantHim[student])		//���e�N���H���n�F
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
							{	///�ǮղM��䤣��ǥ�(���Z������)
								favorite++;

								rejected++;

								if (rejected == studentList[student].Rank.size())		//�Q�ڵ��Ƶ���L�ҥӽЪ��ƶq
								{
									if (noOneWantHim[student])		//���e�N���H���n�F
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
						{	//�Ǯդw�ۺ�
							favorite++;

							rejected++;

							if (noOneWantHim[student])		//���e�N���H���n�F
							{
								break;
							}

							if (rejected == studentList[student].Rank.size())		//�Q�ڵ��Ƶ���L�ҥӽЪ��ƶq
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
			output[entry.first].push_back(entry.second);   //��X

			schoolMatching[entry.second] = {};

			if (!entry.second.empty())
			{
				schoolList[entry.second].stdList.erase(find(schoolList[entry.second].stdList.begin(), 

					schoolList[entry.second].stdList.end(), entry.first));	///��w�g�t��쪺�ǥͱq�ǮզC���R��

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
