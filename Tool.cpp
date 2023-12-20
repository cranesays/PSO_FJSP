#include"FJSP.h"

int Max(vector<int>temp)
{
	if (!temp.empty())
	{
		int max = temp[0];
		for (int i = 0; i < temp.size(); i++)
		{
			if (max < temp[i])
			{
				max = temp[i];
			}
		}
		return max;
	}
}
int Min(vector<int>temp)
{
	if (!temp.empty())
	{
		int min = temp[0];
		for (int i = 0; i < temp.size(); i++)
		{
			if (min > temp[i])
			{
				min = temp[i];
			}
		}
		return min;
	}
}