#include"FJSP.h"
#include<algorithm>
#include<numeric>



/// <summary>
/// 用于求出工序在机器上的位置
/// </summary>
/// <param name="i">工件序号</param>
/// <param name="j">该工件的操作出现次数</param>
/// <param name="job_op_num">工件操作数列表</param>
/// <returns>index(序号)</returns>
int PSO::op_in_m(int i, int j, vector<int>job_op_num)
{
    int op_index;
    if (i == 1)
    {
        op_index = j - 1;
    }
    else
    {
        op_index = accumulate(job_op_num.begin(), job_op_num.begin()+i-1, 0) + j - 1;
    }
    return op_index;
}
void PSO::Start(Data data)
{
	data.Init();//初始化数据集

	//行数为粒子规模，打乱的工序工件序号列表
	vector<vector<int>> os;
	//行数为粒子规模，总工序中各工序的一个可用机器列表（未打乱）
	vector<vector<int>> ms;

	os.resize(pSize);
	ms.resize(pSize);
	for (int i = 0; i < pSize; i++)
	{
		os[i] = data.operation_job_list;
        srand(i);
		random_shuffle(os[i].begin(), os[i].end());
		
		ms[i].resize(data.time_table.size());
		for (int j = 0; j < ms[i].size(); j++)
		{
            cout << ms[j].size();
			vector<int>machine;
			for (int k = 0; k < data._machineNum; k++)
			{
				int temp = data.time_table[j][k];
				if (temp!=-1)
				{
					machine.push_back(k+1);
				}
			}
            vector<int>arange_m(machine.size());
            for (int k = 0; k < machine.size(); k++)
            {
                arange_m[k] = k + 1;
            }
            int seed = j;
			srand(seed);
			ms[i][j] = arange_m[rand() % machine.size()];
		}
	}
    vector<int>fitnessList;
    for (int i = 0; i < pSize; i++)
    {
        fitnessList.push_back(Decode(data._jobNum, data._machineNum, os[i], ms[i], data.time_table, data.job_op_num_list));
        cout << fitnessList[i] << ",";
    }






	for (int i = 0; i < iter; i++)
	{
		double pf = pf_max - (pf_max - pf_min) / i * iter;//适应性
	}
}

float PSO::Decode(int n, int m, vector<int>os, vector<int>ms, vector<vector<int>>p_table, vector<int>job_op_num)
{
    int max_op = Max(job_op_num);//单个工件的最大工序数
    int total0pNum = p_table.size();
        // 大致步骤：1.根据染色体的MS部分得到机器顺序矩阵和时间顺序矩阵
        //         2.根据染色体的OS部分，插空得到最终的调度安排

        // 1.根据染色体的MS部分得到机器顺序矩阵和时间顺序矩阵

        //构建两个列为工件数，行为工件最大工序数的矩阵

        //机器顺序矩阵，存储的是每个工序对应加工的机器的序号，值为0表示不存在此工序，
        //如Jm[0, 0] = 1表示 工件1的工序1在机器1上进行加工
    vector<vector<int>> Jm(n, vector<int>(max_op));
        // 时间顺序矩阵，存储的是每个工序在Jm的机器上的加工时间，值为0表示不存在此工序
        // 如T[0, 0] = 5 表示工序1在机器1上进行加工的时间为5
    vector<vector<int>> T(n,vector<int>(max_op));
        //染色体长度的一半 len(p_table)，得到MS和OS
        // 得到Jm和T, 目前的时间复杂度约为n * max_0p * M

        // 用于帮助p_table找到相应位置
    int p_index = 0;
        //i，j是为了得到Jm和T的下标
    for (int i = 0; i < job_op_num.size();i++) 
    {
        for (int j = 0; j < job_op_num[i]; j++)
        {

            //用于计数，记住当前行有多少个可加工的机器
            int count = 0;

            //找到符合MS的第几个机器序号
            for (int index = 0; index < p_table[p_index].size(); index++)
            {
                if (p_table[p_index][index]!=-1)
                {
                    count++;
                }
                if (count == ms[p_index])
                {
                    Jm[i][j] = index + 1;
                    T[i][j] = p_table[p_index][index];
                    break;
                }
            }
            if (count < ms[p_index])
            {
                cout << "false" << endl;
            }
            p_index ++;

        }
       
    }
    //列为总工序序号，行为机器序号
       //每个机器上的工序的开始加工时间，如第一行: [0, 3, 0, 0, 5, 0, 0] 表示机器1上，工序O1, 2开始加工时间为3，工序O3, 1开始加工时间为5

    vector<vector<int>> start_time(m,vector<int>(total0pNum));//某机器上的开始加工时间，行数为机器数，列数为总工序数的矩阵
    vector<vector<int>> end_time(m, vector<int>(total0pNum));//某机器上的结束加工时间，行数为机器数，列数为总工序数的矩阵

    //2.根据染色体的OS部分，插空得到最终的调度安排
    //用于得出工件的工序，如工件1出现了两次，那么就知道第二次出现的是工序O1, 2
    map<int, int>op_count_dict;
    // 用于存储所有机器上已分配的工序个数, 长度为m
    vector<int>m_op(m);
    for (int i = 0; i < os.size(); i++)
    {
        if (op_count_dict.count(os[i]))
        {
            op_count_dict[os[i]]++;
        }
        else
        {
            op_count_dict.insert({ os[i],1 });
        }

        // 得到os对应的加工机器的序号和相应的加工时间, op_count_dict[os]代表该工件出现了几次
        int m_num = Jm[os[i] - 1][op_count_dict[os[i]] - 1];

        int pro_time = T[os[i] - 1][op_count_dict[os[i]] - 1];
        
        // 求出这个工序在相应机器上的位置
        int op_index = op_in_m(os[i], op_count_dict[os[i]], job_op_num);

        // 求出上一道工序在那个机器上的位置，用job_op_num来求, 因此op_count_dict[os] - 1这里要减去1
        int prev_op_index = op_in_m(os[i], op_count_dict[os[i]] - 1, job_op_num);
        int prev_end_time;

        // 如果是工件的第一个工序也是机器的第一个工序，直接放上去
        // m_op[m_num - 1]代表该机器上已加工的工序个数，op_count_dict[os]代表是这个工件的第几道工序


        if (m_op[m_num - 1] == 0 && op_count_dict[os[i]] == 1)
        {
            start_time[m_num - 1][op_index] = 0;
            end_time[m_num - 1][op_index] = pro_time;
        }
        // 如果是机器的第一道工序，不是工件的第一道工序，直接从这个工件的上一个工序结束时间开始即可
        else if (m_op[m_num - 1] == 0 && op_count_dict[os[i]] > 1)
        {

            // 先找到上一道工序在哪个机器上加工
            int prev_m_num = Jm[os[i] - 1][op_count_dict[os[i]] - 2];
            // 上一道的结束时间
            prev_end_time = end_time[prev_m_num - 1][prev_op_index];
            start_time[m_num - 1][op_index] = prev_end_time;
            end_time[m_num - 1][op_index] = prev_end_time + pro_time;
        }

        
        else if (m_op[m_num - 1] > 0)
        {
            // 用来标记插到空位置了没
            int flag = 0;
            // 这里设置prev_end_time是为了最终的统一 free_start = max(max(end_time[m_num - 1]), prev_end_time)
            int pre_end_time = 0;
            int free_start;
            // 如果是该工件的第一道工序
            if (op_count_dict[os[i]] == 1)
            {
                free_start = 0;
            }
            // 如果既不是机器的第一道工序，也不是是工件的第一道工序，要插空, 用从上一个工序的结束时间开始找
            else
            {
                // 先找到上一道工序在哪个机器上加工
                int prev_m_num = Jm[os[i] - 1][op_count_dict[os[i] - 2]];
                // 上一道的结束时间
                int  prev_end_time = end_time[prev_m_num - 1][prev_op_index];
                // 这里的free_start为上一个工序结束的时间
                free_start = prev_end_time;
            }

            vector<int> order_start_time;
            for (int j = 0; j < end_time[m_num - 1].size();j++)
            {
                if (end_time[m_num - 1][j] > 0)
                {
                    order_start_time.push_back(start_time[m_num - 1][j]);
                }
            }
            sort(order_start_time.begin(), order_start_time.end());
            vector<int> order_end_time;
            for (int j = 0; j < end_time[m_num - 1].size(); j++)
            {
                if (end_time[m_num - 1][j] > 0)
                {
                    order_end_time.push_back(end_time[m_num - 1][j]);
                }
            }
            sort(order_end_time.begin(), order_end_time.end());
            for (int index = 0; index < order_start_time.size(); index++)
            {
                if (order_start_time[index] - free_start >= pro_time)
                {
                    start_time[m_num - 1][op_index] = free_start;
                    end_time[m_num - 1][op_index] = free_start + pro_time;
                    flag = 1;
                    break;
                    // else:
                    // 这里写if是因为要确保free_start的起始点是要大于或者等于prev_end_time
                }
                if (order_end_time[index] - free_start >= 0)
                {
                    free_start = order_end_time[index];
                }
            }
            if (flag == 0)
            {
                free_start = max(Max(end_time[m_num - 1]), prev_end_time);
                start_time[m_num - 1][op_index] = free_start;
                end_time[m_num - 1][op_index] = free_start + pro_time;
            }
        }
        m_op[m_num - 1]++;
    }
    vector<int>max_end_time;
    for (int i = 0; i < end_time.size(); i++)
    {
        max_end_time.push_back(Max(end_time[i]));
    }


    int c_max = Max(max_end_time);

    return c_max;
}
PSO::PSO()
{
	iter = 100;
	pSize = 100;
	fitness = 0.5;
	d = 2;
	speed = 1;
	times = 2;
}
PSO::PSO(int _iter, int _pSize, double _fitness, int _d, int _speed, int _times)
{
	iter = _iter;
	pSize = _pSize;
	fitness = _fitness;
	d = _d;
	speed =_speed;
	times =_times;
}
