#include<algorithm>
#include<iostream>
#include<string.h>
#include<stdio.h>
#include"Function.h"
#include"Departments.h"
#include"Students.h"
#include <fstream>
#pragma warning(disable:4996)
using namespace std;
#define INF 0x3f3f3f3f
#define maxn 3010
string t1, t2, t3, t4;
int n, m; //部门，学生数量
Student st[maxn];
department dep[maxn];
int StuRank[maxn][maxn] = { 0 };//每个学生每个志愿的权值
int score[maxn][maxn] = { 0 };
int s[maxn] = { 0 };
int k[maxn] = { 0 };
int temp[300][300] = { 0 };
int counts[maxn] = { 0 }, StuCount = 0, DepCount = 0;
void Function::input()
{
	freopen("import.txt", "r", stdin);
	cin >> n >> m;
	for (int i = 0; i < n; i++)//读入部门的数据
	{
		cin >> dep[i].DepartmentNo >> dep[i].MemberLimit;
		for (int j = 0; j < 4; j++)
		{
			cin >> dep[i].DepartmentTags[j];
		}
		cin >> dep[i].we >> dep[i].hour1 >> t1 >> dep[i].hour2 >> t2;
	}
	for (int i = 0; i < m; i++)//读入学生的数据
	{
		cin >> st[i].StudentNo >> st[i].Point;
		for (int j = 0; j < 5; j++)
		{
			cin >> st[i].aspiration[j];
		}
		for (int j = 0; j < 3; j++)
		{
			cin >> st[i].StudentTags[j];
		}
		for (int j = 0; j < 5; j++)
		{
			cin >> st[i].we[j] >> st[i].front[j] >> t3 >> st[i].last[j] >> t4;
		}
	}
}


/*void Function::PrintData()
{
freopen("output.txt", "w", stdout);
cout << "部门数量:" << n << endl;
cout << "学生数量:" << m << endl;
for (int i = 0; i < n; i++)
{
cout << "部门编号:" << dep[i].DepartmentNo << "  部门限定人数：" << dep[i].MemberLimit << endl;
cout << "部门标签:" << endl;
for (int j = 0; j < 4; j++)
{
cout << dep[i].DepartmentTags[j] << "  ";
}
cout << endl;
cout << "部门常规时间: " << dep[i].we << " " << dep[i].hour1 << t1 << dep[i].hour2 << t2 << endl;
cout << endl;
}
for (int i = 0; i < m; i++)
{
cout << "学生编号: " << st[i].StudentNo << "  绩点; " << st[i].Point << endl;
cout << "学生志愿:  " ;
for (int j =0; j < 5; j++)
{
cout << st[i].aspiration[j] << " ";
}
cout << endl;
cout << "学生标签:" << endl;
for (int j = 0; j < 3; j++)
{
cout << st[i].StudentTags[j] << "  ";
}
cout << endl;
cout << "空闲时间: " << endl;
for (int j = 0; j < 5; j++)
{
cout << st[i].we[j] << " " << st[i].front[j] << t3 << st[i].last[j] << t4 << endl;
}
cout << endl;
}
}*/
//计算权重
void Function::GetRank()
{
	//freopen("rank.txt", "w", stdout);
	StuRank[maxn][n] = { 0 };
	double num = 0;//志愿的匹配数
	int q[maxn] = { 0 };
	double tidu = 5;
	for (int i = 0; i < m; i++)
	{
		
		for (int k = 0; k < 5; k++)
		{
			for (int p = 0; p < 5; p++)
			{
				if (st[i].we[p] == dep[st[i].aspiration[k] - 1].we)//当学生空余时间与部门常规时间是同一天的时候
				{
					//学生时间与常规时间不冲突
					if ((st[i].front[p] <= dep[st[i].aspiration[k] - 1].hour1) 
						&& (st[i].last[p] >= dep[st[i].aspiration[k] - 1].hour2))
					{
						StuRank[st[i].StudentNo][st[i].aspiration[k]] = 1;//先将时间符合的人标签置为1
						break;
					}
				}

			}
		}
	}

	for (int j = 0; j < m; j++)
	{
		
		for (int k = 0; k < 5; k++)
		{
			//只有当学生时间与常规时间不冲突的时候才计算该志愿的权重
			if (StuRank[st[j].StudentNo][st[j].aspiration[k]] == 1)
			{
				for (int f = 0; f < 3; f++)
				{
					for (int o = 0; o < 4; o++)
					{
						if ((q[st[j].aspiration[k]] != 1) && (st[j].StudentTags[f]
							== dep[st[j].aspiration[k] - 1].DepartmentTags[o]))
						{
							num+=1;
						}
					}
				}

				if ((q[st[j].aspiration[k]] != 1))//第一次出现这个志愿
				{
					//志愿权重，梯度用来表示这个志愿是该学生的第几志愿，5K是第一志愿，4K是第二志愿，依次类推
					StuRank[st[j].StudentNo][st[j].aspiration[k]] = (int)((st[j].Point*0.14 + num * 0.1 + tidu) * 1000);
					
					q[st[j].aspiration[k]] = 1;//将已经选择的部门编号置为1
				}
				num = 0;
			}
			tidu -= 1;//志愿梯度，第一志愿的梯度最大，之后逐级递减
		}
		for (int i = 1; i <= n; i++)
		{
			q[i] = 0;
		}
		memset(q, 0, sizeof(q));
		tidu = 5;
	}
}


//对选择同一志愿的学生根据权重从大到小排序
//部门选择时将根据权重选择
void Function::sort()
{	
	/*for (int i = 0; i < m; i++)
	{
		cout << "编号：" << i + 1 << endl;
		for (int j = 0; j < 5; j++)
		{
			cout << "志愿 " << st[i].aspiration[j] << " 的分数为：" << StuRank[st[i].StudentNo][st[i].aspiration[j]] << endl;
		}
	}*/
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (StuRank[st[j].StudentNo][i + 1] != 0)
			{
				temp[i + 1][k[i + 1]] = st[j].StudentNo;//k[i+1]表示选了部门i+1的人一共有多少
				k[i + 1]++;
			}
		}
	}
	for (int i = 0; i < n; i++)//i+1是部门编号
	{
		memset(s, 0, sizeof(s));

		//将所有同学的该志愿权重存入一维数组
		for (int j = 0; j < k[i + 1]; j++)
		{
			s[j] = StuRank[temp[i + 1][j]][i + 1];
		}
		for (int j = 0; j < k[i + 1] - 1; j++)//冒泡排序，按照权重从大到小排序
		{
			for (int p = 0; p < k[i + 1] - j - 1; p++)
			{
				if (s[p] <s[p + 1])
				{
					int t;
					int t1;

					//权重和学生编号同时替换
					t1 = s[p];
					s[p] = s[p + 1];
					s[p + 1] = t1;

					t = temp[i + 1][p]; //学生编号
					temp[i + 1][p] = temp[i + 1][p + 1];
					temp[i + 1][p + 1] = t;

				}
			}
		}
	}
}

//输出
void Function::print()
{
	freopen("output.txt", "w", stdout);
	for (int i = 0; i < n; i++)
	{
		int Max = 0;

		//部门需要的人数的上限大于报名该志愿的人数
		if (dep[i].MemberLimit > k[i + 1])
		{
			Max = k[i + 1];
		}
		else
		{
			Max = dep[i].MemberLimit;
		}

		//没有选中学生的部门
		if (Max == 0)
		{
			DepCount++;
		}
		cout << "第" << i + 1 << "部门" << "一共选中" << Max << "位学生，学生编号为：" << endl;

		for (int j = 0; j < Max; j++)
		{
			
			cout << temp[i + 1][j] << "  ";
			if (counts[temp[i + 1][j]] == 0)
			{
				StuCount++; //已经被部门选中的学生
			}
			counts[temp[i + 1][j]] = 1;
		}
		cout << endl;
		cout << endl;
	}
	int sum = 0;
	//n个部门所需人数上限的总和
	for (int i = 0; i < n; i++)
	{
		sum += dep[i].MemberLimit;
	}

	cout << n << "个部门一共招" << sum << "个学生！" << endl;
	cout << "一共有" << n << "个部门，其中有" << DepCount << "个部门没有招到学生！" << endl;
	cout << "一共有" << m << "位学生，中选的一共有" << StuCount << "位学生！" << endl;

}