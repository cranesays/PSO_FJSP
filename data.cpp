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
	cout << "输入工件数量：";
	cin >> _jobNum;
	cout << "输入机器数量：";
	cin >> _machineNum;
}
string Data::Read(string fileName)
{
	fstream file;
	file.open(fileName, ios::in); //以只写模式打开文件
	

	char *buff = new char[1024];
	file.read(buff, 0);
	file.close(); //关闭文件
	string json(buff);
	return json;
}
Json::Value Data::readJsonFile(const string& filename)
{
	//1.打开文件
	ifstream ifile;
	ifile.open(filename);

	//2.创建json读工厂对象
	Json::CharReaderBuilder ReaderBuilder;
	ReaderBuilder["emitUTF8"] = true;//utf8支持，不加这句，utf8的中文字符会编程\uxxx

	//3.创建json对象，等会要返回这个对象
	Json::Value root;

	//4.把文件转变为json对象，要用到上面的三个变量,数据写入root
	string strerr;
	bool ok = Json::parseFromStream(ReaderBuilder, ifile, &root, &strerr);
	if (!ok) {
		cerr << "json解析错误";
	}
	//5.返回存有数据的json，这个json对象已经能用了
	return root;
}