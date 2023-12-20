#pragma once
#include<iostream>
#include<map>
#include<vector>
#include<fstream>
#include "json/json-forwards.h"
#include "json/json.h"
using namespace std;

/// <summary>工件数量jobNum，机器数量machineNum，每个工件的工序数量operationNum，加工operation</summary>

struct Job
{
	int operationNum;
	/// <summary>
	/// vector(map(machine_id,time),......）
	/// (((int,int),(int,int)...),(...)...）
	/// </summary>
	vector<map<int, int>> Operation;
	Job();
};

class Data {
	
public:
	Json::Value root;
	int _jobNum, _machineNum, operation_list_size, max_jobOpNum;//工件数量，机器数量，总工序数量,工件最大工序数
	vector<Job> job;
	vector<int> operation_job_list;//长度为总工序的工件序号列表(1开始)
	vector<int> job_op_num_list;//工件工序数列表
	/// <summary>
	/// 行数为总工序，列数为机器数，内容为时间的矩阵
	/// </summary>
	vector<vector<int>>time_table;

	void Init();
	void Input(int jobNum, int machineNum);
	void Input();
	string Read(string fileName = "object.json");
	Json::Value readJsonFile(const string& filename = "object.json");
};


class PSO
{
private:
	int iter;//迭代次数
	int pSize;//粒子个数
	double fitness;//适应度
	int d;//粒子维度（自变量个数）
	int speed;//粒子速度
	int times;//迭代次数




	//超参数
	float o_mega = 0.15;
	float c1 = 0.5;
	float c2 = 0.7;
	float pf_max = 0.8;
	float pf_min = 0.2;

public:
	PSO();
	PSO(int _iter, int _pSize, double _fitness, int d, int speed, int times);
	void Start(Data data);
	/// <summary>
	/// 输入为粒子序号，粒子存储在类内
	/// </summary>
	/// <returns>最短时间</returns>
	float Decode(int n, int m, vector<int>os, vector<int>ms, vector<vector<int>>p_table, vector<int>job_op_num);
	int op_in_m(int i, int j, vector<int>job_op_num);
};


//Tools
int Max(vector<int>temp);
int Min(vector<int>temp);