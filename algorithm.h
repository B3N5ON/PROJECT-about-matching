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

	for (auto& it : schoolList)  //��l��
	{
		schoolMatching[it.first] = {};

		num[it.first] = it.second.Number;

		if (num[it.first] > it.second.stdList.size())		//�p�G�ۦ��H�Ƥp��i�����H��
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

		while (!freeStudent.empty())		//�C�ӥӽЪ̳��b�W��W or �L�ӽЪ��Ǯլҩڵ��L
		{
			if (noSize)
				break;

			for (auto& it : studentList)
			{
				if (freeStudent.empty())		//�S���ۥѤH
				{
					break;
				}

				wstring student = it.first;

				int favorite = 0, rejected = 0;

				if (admissionNum[student] != MAXVALUE) {//
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
										//�Ǯ��٨S�t��
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
									{	//�Ǯդw�g�t��
										auto currentIndex = current.find(school);

										wstring currentStudent = currentIndex->second;

										int currentStundetIndex = distance(schoolList[school].stdList.begin(),

											find(schoolList[school].stdList.begin(), schoolList[school].stdList.end(), currentStudent));

										int newStudentIndex = distance(schoolList[school].stdList.begin(),

											find(schoolList[school].stdList.begin(), schoolList[school].stdList.end(), student));

										if (currentStundetIndex > newStudentIndex)
										{		//�p�G�������ǥͶ����
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
										{		//�����ǥͶ���C
											favorite++;			//find next school

											rejected++;

											if (rejected == studentList[student].Rank.size())		//�Q�ڵ��Ƶ���L�ҥӽЪ��ƶq(���Q�ڵ�)
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

										/*�[�Y�ܼ� �P�_�Ǯզb�X�{�L���j��ɦ��L�Q���t��*/

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
			output[entry.first].push_back(entry.second);   //��X

			schoolMatching[entry.second] = {};

			if (!entry.second.empty())
			{
				schoolList[entry.second].stdList.erase(find(schoolList[entry.second].stdList.begin(), 

					schoolList[entry.second].stdList.end(), entry.first));	///��w�g�t��쪺�ǥͱq�ǮզC���R��

				if (num[entry.second] != 0)		///�p�G���ƨ�H�A�Ѿl�H�ƴ�@
				{
					num[entry.second]--;
				}

				studentMatching[entry.first] = {}; //��l��
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
