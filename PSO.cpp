#include"FJSP.h"
#include<algorithm>
#include<numeric>



/// <summary>
/// ������������ڻ����ϵ�λ��
/// </summary>
/// <param name="i">�������</param>
/// <param name="j">�ù����Ĳ������ִ���</param>
/// <param name="job_op_num">�����������б�</param>
/// <returns>index(���)</returns>
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
	data.Init();//��ʼ�����ݼ�

	//����Ϊ���ӹ�ģ�����ҵĹ��򹤼�����б�
	vector<vector<int>> os;
	//����Ϊ���ӹ�ģ���ܹ����и������һ�����û����б�δ���ң�
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
		double pf = pf_max - (pf_max - pf_min) / i * iter;//��Ӧ��
	}
}

float PSO::Decode(int n, int m, vector<int>os, vector<int>ms, vector<vector<int>>p_table, vector<int>job_op_num)
{
    int max_op = Max(job_op_num);//�����������������
    int total0pNum = p_table.size();
        // ���²��裺1.����Ⱦɫ���MS���ֵõ�����˳������ʱ��˳�����
        //         2.����Ⱦɫ���OS���֣���յõ����յĵ��Ȱ���

        // 1.����Ⱦɫ���MS���ֵõ�����˳������ʱ��˳�����

        //����������Ϊ����������Ϊ������������ľ���

        //����˳����󣬴洢����ÿ�������Ӧ�ӹ��Ļ�������ţ�ֵΪ0��ʾ�����ڴ˹���
        //��Jm[0, 0] = 1��ʾ ����1�Ĺ���1�ڻ���1�Ͻ��мӹ�
    vector<vector<int>> Jm(n, vector<int>(max_op));
        // ʱ��˳����󣬴洢����ÿ��������Jm�Ļ����ϵļӹ�ʱ�䣬ֵΪ0��ʾ�����ڴ˹���
        // ��T[0, 0] = 5 ��ʾ����1�ڻ���1�Ͻ��мӹ���ʱ��Ϊ5
    vector<vector<int>> T(n,vector<int>(max_op));
        //Ⱦɫ�峤�ȵ�һ�� len(p_table)���õ�MS��OS
        // �õ�Jm��T, Ŀǰ��ʱ�临�Ӷ�ԼΪn * max_0p * M

        // ���ڰ���p_table�ҵ���Ӧλ��
    int p_index = 0;
        //i��j��Ϊ�˵õ�Jm��T���±�
    for (int i = 0; i < job_op_num.size();i++) 
    {
        for (int j = 0; j < job_op_num[i]; j++)
        {

            //���ڼ�������ס��ǰ���ж��ٸ��ɼӹ��Ļ���
            int count = 0;

            //�ҵ�����MS�ĵڼ����������
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
    //��Ϊ�ܹ�����ţ���Ϊ�������
       //ÿ�������ϵĹ���Ŀ�ʼ�ӹ�ʱ�䣬���һ��: [0, 3, 0, 0, 5, 0, 0] ��ʾ����1�ϣ�����O1, 2��ʼ�ӹ�ʱ��Ϊ3������O3, 1��ʼ�ӹ�ʱ��Ϊ5

    vector<vector<int>> start_time(m,vector<int>(total0pNum));//ĳ�����ϵĿ�ʼ�ӹ�ʱ�䣬����Ϊ������������Ϊ�ܹ������ľ���
    vector<vector<int>> end_time(m, vector<int>(total0pNum));//ĳ�����ϵĽ����ӹ�ʱ�䣬����Ϊ������������Ϊ�ܹ������ľ���

    //2.����Ⱦɫ���OS���֣���յõ����յĵ��Ȱ���
    //���ڵó������Ĺ����繤��1���������Σ���ô��֪���ڶ��γ��ֵ��ǹ���O1, 2
    map<int, int>op_count_dict;
    // ���ڴ洢���л������ѷ���Ĺ������, ����Ϊm
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

        // �õ�os��Ӧ�ļӹ���������ź���Ӧ�ļӹ�ʱ��, op_count_dict[os]����ù��������˼���
        int m_num = Jm[os[i] - 1][op_count_dict[os[i]] - 1];

        int pro_time = T[os[i] - 1][op_count_dict[os[i]] - 1];
        
        // ��������������Ӧ�����ϵ�λ��
        int op_index = op_in_m(os[i], op_count_dict[os[i]], job_op_num);

        // �����һ���������Ǹ������ϵ�λ�ã���job_op_num����, ���op_count_dict[os] - 1����Ҫ��ȥ1
        int prev_op_index = op_in_m(os[i], op_count_dict[os[i]] - 1, job_op_num);
        int prev_end_time;

        // ����ǹ����ĵ�һ������Ҳ�ǻ����ĵ�һ������ֱ�ӷ���ȥ
        // m_op[m_num - 1]����û������Ѽӹ��Ĺ��������op_count_dict[os]��������������ĵڼ�������


        if (m_op[m_num - 1] == 0 && op_count_dict[os[i]] == 1)
        {
            start_time[m_num - 1][op_index] = 0;
            end_time[m_num - 1][op_index] = pro_time;
        }
        // ����ǻ����ĵ�һ�����򣬲��ǹ����ĵ�һ������ֱ�Ӵ������������һ���������ʱ�俪ʼ����
        else if (m_op[m_num - 1] == 0 && op_count_dict[os[i]] > 1)
        {

            // ���ҵ���һ���������ĸ������ϼӹ�
            int prev_m_num = Jm[os[i] - 1][op_count_dict[os[i]] - 2];
            // ��һ���Ľ���ʱ��
            prev_end_time = end_time[prev_m_num - 1][prev_op_index];
            start_time[m_num - 1][op_index] = prev_end_time;
            end_time[m_num - 1][op_index] = prev_end_time + pro_time;
        }

        
        else if (m_op[m_num - 1] > 0)
        {
            // ������ǲ嵽��λ����û
            int flag = 0;
            // ��������prev_end_time��Ϊ�����յ�ͳһ free_start = max(max(end_time[m_num - 1]), prev_end_time)
            int pre_end_time = 0;
            int free_start;
            // ����Ǹù����ĵ�һ������
            if (op_count_dict[os[i]] == 1)
            {
                free_start = 0;
            }
            // ����Ȳ��ǻ����ĵ�һ������Ҳ�����ǹ����ĵ�һ������Ҫ���, �ô���һ������Ľ���ʱ�俪ʼ��
            else
            {
                // ���ҵ���һ���������ĸ������ϼӹ�
                int prev_m_num = Jm[os[i] - 1][op_count_dict[os[i] - 2]];
                // ��һ���Ľ���ʱ��
                int  prev_end_time = end_time[prev_m_num - 1][prev_op_index];
                // �����free_startΪ��һ�����������ʱ��
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
                    // ����дif����ΪҪȷ��free_start����ʼ����Ҫ���ڻ��ߵ���prev_end_time
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
