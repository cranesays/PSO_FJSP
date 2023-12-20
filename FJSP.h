#pragma once
#include<iostream>
#include<map>
#include<vector>
#include<fstream>
#include "json/json-forwards.h"
#include "json/json.h"
using namespace std;

/// <summary>��������jobNum����������machineNum��ÿ�������Ĺ�������operationNum���ӹ�operation</summary>

struct Job
{
	int operationNum;
	/// <summary>
	/// vector(map(machine_id,time),......��
	/// (((int,int),(int,int)...),(...)...��
	/// </summary>
	vector<map<int, int>> Operation;
	Job();
};

class Data {
	
public:
	Json::Value root;
	int _jobNum, _machineNum, operation_list_size, max_jobOpNum;//���������������������ܹ�������,�����������
	vector<Job> job;
	vector<int> operation_job_list;//����Ϊ�ܹ���Ĺ�������б�(1��ʼ)
	vector<int> job_op_num_list;//�����������б�
	/// <summary>
	/// ����Ϊ�ܹ�������Ϊ������������Ϊʱ��ľ���
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
	int iter;//��������
	int pSize;//���Ӹ���
	double fitness;//��Ӧ��
	int d;//����ά�ȣ��Ա���������
	int speed;//�����ٶ�
	int times;//��������




	//������
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
	/// ����Ϊ������ţ����Ӵ洢������
	/// </summary>
	/// <returns>���ʱ��</returns>
	float Decode(int n, int m, vector<int>os, vector<int>ms, vector<vector<int>>p_table, vector<int>job_op_num);
	int op_in_m(int i, int j, vector<int>job_op_num);
};


//Tools
int Max(vector<int>temp);
int Min(vector<int>temp);