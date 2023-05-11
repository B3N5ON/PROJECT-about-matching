#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <iomanip>
#include <limits>
#include <string >
#include <windows.h>
#include <algorithm>
#define STUDENT "newstudata.csv"
#define RANK "newrank.csv"
#define SCHOOL "newschooldata.csv"

using namespace std;

typedef vector<wstring> WS;

class student {
public:
	wstring id, name;

	int Chinese{};

	int English{};

	int MathA{};

	int MathB{};

	int Social{};

	int Nature{};

	int totalGrade{};

	WS finalRank;
};

class school {
public:
	wstring SchoolName;

	int Number{};

	int ChineseG{};			

	int EnglishG{};

	int MathAG{};

	int MathBG{};

	int SocialG{};

	int NatureG{};

	double chRatio{};		//國文倍率

	double EnRatio{};	//英文倍率

	double MARatio{};	//數A倍率

	double MBRatio{};	//數B倍率

	double SoRatio{};	//社會倍率

	double NaRatio{};	//自然倍率

	vector<wstring> stdList;	
};

void commaSeparate(vector<wstring> &v, wstring temp, wstring symbol) {		//字串分割(存入vector, 暫存string, 符號)
	while (true)
	{
		v.push_back(temp.substr(0, temp.find(symbol)));

		temp = temp.substr(temp.find(symbol) + 1, temp.length());

		if (temp.find(symbol) == -1) {
			v.push_back(temp);

			break;
		}
	}
}

void fill0To6(wstring &p) {			//學校代號為6碼，補0並存入string
		wstringstream ss(p);

		ss << setw(6) << setfill(L'0') << p;

		ss >> p;
}

void FiveStanderMap(map<wstring, vector<int>>&FiveStandards)
{
	ifstream grade("differtinating grade.txt");

	string str;

	wchar_t buf[255];

	int count = 0;

	while (getline(grade, str)) {
		int sz = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf, sz);

		wstring wstr = buf;

		vector<wstring> s;

		commaSeparate(s, wstr, L"\t");

		if (count == 0) {
			count++;

			continue;
		}
		wstring subject = s[0];

		s.erase(s.begin());

		vector<int> stander;

		for (int i = 0; i < s.size(); i++)
			stander.push_back(stoi(s[i]));

		FiveStandards.insert(pair<wstring, vector<int>>(subject, stander));
	}

	grade.close();
}

void buiild_student_rank(vector<WS> &rankList)
{
	ifstream in(RANK);

	string str;

	wchar_t buf[255];

	while (getline(in, str))
	{
		int sz = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf, sz);

		wstring wstr = buf;

		WS s, t, p;

		commaSeparate(s, wstr, L",");

		p.push_back(s[0]);

		for (int i = 1; i < s.size(); i++) {
			fill0To6(s[i]);

			if (s[i] == L"000000")
				t.push_back(s[i]);
			else
				p.push_back(s[i]);
		}
		rankList.push_back(p);
	}
	rankList.erase(rankList.begin());

	in.close();
}

void build_student_list(map<wstring, student> &StudentMap)
{
	ifstream in(STUDENT);

	string str;

	wchar_t buf[255];

	int count = 0;

	while (getline(in, str))
	{
		int sz = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf, sz);

		wstring wstr = buf;

		WS s;

		student temp;

		commaSeparate(s, wstr, L",");

		if (count == 0) {
			count++;
			continue;
		}
		temp.id = s[0];

		temp.name = s[1];

		temp.Chinese = stoi(s[2]);

		temp.English = stoi(s[3]);		

		if (s[4] != L"null") 
			temp.MathA = stoi(s[4]);
		else
			temp.MathA = -1;

		if (s[5] != L"null") 
			temp.MathB = stoi(s[5]);			
		else
			temp.MathB = -1;

		if (s[6] != L"null") 
			temp.Social = stoi(s[6]);
		else
			temp.Social = -1;

		if (s[7] != L"null") 
			temp.Nature = stoi(s[7]);
		else
			temp.Nature = -1;

		//studentList.push_back(temp);
		StudentMap[temp.id] = temp;
	}

	in.close();
}

void build_school_rank(map<wstring, school> &SchoolDetail, map<wstring, vector<int>> FiveStandards)
{
	ifstream in(SCHOOL);

	string str;

	wchar_t buf[255];

	int count = 0;

	vector<wstring> SubJ;	//get the subject to determine which column is 

	while (getline(in, str))
	{
		int sz = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf, sz);

		wstring wstr = buf;

		WS s;

		commaSeparate(s, wstr, L",");

		if (count == 0) {	//get rid of first row
			count++;

			SubJ = s;

			s.erase(s.begin(), s.end());

			continue;
		}
		fill0To6(s[0]);	

		vector<int> grade;

		for (int i = 3; i < 9; i++) {
			if (s[i] == L"頂標") {
				auto it = FiveStandards.find(SubJ[i]);

				grade.push_back(it->second[0]);
			}
			else if (s[i] == L"前標") {
				auto it = FiveStandards.find(SubJ[i]);

				grade.push_back(it->second[1]);
			}
			else if (s[i] == L"均標") {
				auto it = FiveStandards.find(SubJ[i]);

				grade.push_back(it->second[2]);
			}
			else if (s[i] == L"後標") {
				auto it = FiveStandards.find(SubJ[i]);

				grade.push_back(it->second[3]);
			}
			else if (s[i] == L"底標") {
				auto it = FiveStandards.find(SubJ[i]);

				grade.push_back(it->second[4]);
			}
			else if (s[i] == L"")
				grade.push_back(-1);
		}
		school sc;

		sc.SchoolName = s[1];

		sc.Number = stoi(s[2]);

		for (int i = 0; i < 6; i++) {
			if (grade[i] != -1) {
				switch (i) {
				case 0:
					sc.ChineseG = grade[i];
					continue;
				case 1:
					sc.EnglishG = grade[i];
					continue;
				case 2:
					sc.MathAG = grade[i];
					continue;
				case 3:
					sc.MathBG = grade[i];
					continue;
				case 4:
					sc.SocialG = grade[i];
					continue;
				case 5:
					sc.NatureG = grade[i];
					continue;
				}
			}
		}
		for (int i = 9; i < 15; i++) {
			if (s[i] != L"") {
				switch (i) {
				case 9:
					sc.chRatio = stoi(s[i]);
					continue;
				case 10:
					sc.EnRatio = stoi(s[i]);
					continue;
				case 11:
					sc.MARatio = stoi(s[i]);
					continue;
				case 12:
					sc.MBRatio = stoi(s[i]);
					continue;
				case 13:
					sc.SoRatio = stoi(s[i]);
					continue;
				case 14:
					sc.NaRatio = stoi(s[i]);
					continue;
				}
			}
		}		//ratio
		SchoolDetail.insert(pair<wstring, school>(s[0], sc));
	}
	in.close();
}
