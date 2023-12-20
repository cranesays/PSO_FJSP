#include"FJSP.h"


Job::Job()
{
	operationNum = 0;
}
void Data::Init()
{
	root = readJsonFile();

	_machineNum = 6;

	if (root["job_list"].isArray())
	{
		operation_list_size = root["job_list"].size();
		operation_job_list.resize(operation_list_size);
		time_table = vector<vector<int>>(operation_list_size, vector<int>(_machineNum, -1));

		for (int i = 0; i < operation_list_size; i++)
		{
			int job_id = root["job_list"][i]["item_id"].asInt();
			operation_job_list[i] = job_id;//1,1,1,1,2,2,3,3,3,3
			int operation_id = root["job_list"][i]["job_id"].asInt();


			if (job_id > job.size())
			{
				job.resize(job_id);
			}
			if (operation_id > job[job_id - 1].Operation.size())
			{
				job[job_id - 1].Operation.resize(operation_id);
				job[job_id - 1].operationNum++;
			}
			int machine_map_size = root["job_list"][i]["machine_map"].size();
			map<int, int>machine_map;
			for (int j = 0; j < machine_map_size; j++)
			{
				machine_map.insert(pair<int, int>
					(root["job_list"][i]["machine_map"][j]["machine_id"].asInt(), root["job_list"][i]["machine_map"][j]["work_time"].asInt())
				);
				time_table[i][root["job_list"][i]["machine_map"][j]["machine_id"].asInt()-1] = root["job_list"][i]["machine_map"][j]["work_time"].asInt();
			}
			job[job_id - 1].Operation[operation_id - 1] = machine_map;
		}
		_jobNum = job.size();

		vector<int>temp;

		for (int i = 0; i < _jobNum; i++)
		{
			temp.push_back(job[i].operationNum);
		}
		max_jobOpNum = Max(temp);

		//p_table
		job_op_num_list.resize(_jobNum);
		for (int i = 0; i < _jobNum; i++)
		{
			job_op_num_list[i] = job[i].operationNum;
		}
	}




	//check
	/*
	cout <<"jobNum:" << _jobNum << endl;
	cout << "operationNum:" << operation_list_size << endl;
	cout << "operation_job_list:";
	for (int j = 0; j < operation_job_list.size(); j++)
	{
		cout << operation_job_list[j];
	}
	cout << endl;
	for (int i = 0; i < job.size(); i++)
	{
		cout << "job:" << i + 1 << endl;
		for (int j = 0; j < job[i].Operation.size();j++)
		{
			cout << "opertion:" << j + 1 << endl;
			for (int k = 0; k < job[i].Operation[j].size(); k++)
			{
				cout << "machine_id:" << k + 1 << endl;
				cout << "time:" << job[i].Operation[j][k] << endl;
			}
		}
	}
	*/
}

void Data::Input(int jobNum,int machineNum)
{
	_jobNum = jobNum;
	_machineNum = machineNum;
}
void Data::Input()
{
	cout << "���빤��������";
	cin >> _jobNum;
	cout << "�������������";
	cin >> _machineNum;
}
string Data::Read(string fileName)
{
	fstream file;
	file.open(fileName, ios::in); //��ֻдģʽ���ļ�
	

	char *buff = new char[1024];
	file.read(buff, 0);
	file.close(); //�ر��ļ�
	string json(buff);
	return json;
}
Json::Value Data::readJsonFile(const string& filename)
{
	//1.���ļ�
	ifstream ifile;
	ifile.open(filename);

	//2.����json����������
	Json::CharReaderBuilder ReaderBuilder;
	ReaderBuilder["emitUTF8"] = true;//utf8֧�֣�������䣬utf8�������ַ�����\uxxx

	//3.����json���󣬵Ȼ�Ҫ�����������
	Json::Value root;

	//4.���ļ�ת��Ϊjson����Ҫ�õ��������������,����д��root
	string strerr;
	bool ok = Json::parseFromStream(ReaderBuilder, ifile, &root, &strerr);
	if (!ok) {
		cerr << "json��������";
	}
	//5.���ش������ݵ�json�����json�����Ѿ�������
	return root;
}