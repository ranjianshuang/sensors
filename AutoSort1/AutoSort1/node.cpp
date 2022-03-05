#include "node.h"
#include "windows.h"
int NodeCountNum;
int COMMU_DIS = 5;
Node * CreatNode(int i)
{
	Node * node;
	node = (Node *)malloc(sizeof(Node)*i);

	//第一个节点初始化
	node[0].Id = 1;
	node[0].Distance = 0;
	node[0].Num = 1;
	node[0].Tag = 0;
	node[0].Clock = 3;//表示3个0.2s
	node[0].Flag = false;
	node[0].Val = 0;
	
	//节点信息表初始化
	for (int temp = 0; temp < MAX_MES_COUNT; temp++)
	{
		node[0].Rece_Node_Info[temp][0] = 0;
		node[0].Rece_Node_Info[temp][1] = 0;
		node[0].Node_Stable[temp] = 0;
	}
	node[0].Node_List_Length = 0;
	node[0].Remain_Zero_Count = 0;
	node[0].Rece_Mes_Count = 0;
	node[0].Send_Mes_Count = 0;
	//后续节点初始化
	bool flag = true;
	for (int j = 1; j < i; ++j)
	{
		int ran = MAX_RAND;
		while (flag) {
			if (ran < MAX_RAND && ran > 2 )
			{
				node[j].Clock = ran;
				break;
			}
			ran = rand();
		}
		
		node[j].Id = j+1;
		node[j].Distance = j * EQUAL_DIS;//均匀生成间距为 EQUAL_DIS
		node[j].Num = 0;
		node[j].Tag = 0;
		node[j].Flag = false;
		node[j].Val = 0;
		
		//节点信息表初始化
		for (int temp = 0; temp < MAX_MES_COUNT; temp++)
		{
			node[j].Rece_Node_Info[temp][0] = 0;
			node[j].Rece_Node_Info[temp][1] = 0;
			node[j].Node_Stable[temp] = 0;
		}
		node[j].Node_List_Length = 0;
		node[j].Remain_Zero_Count = 0;
		node[j].Rece_Mes_Count = 0;
		node[j].Send_Mes_Count = 0;
	}
	return node;
}

void Wait()
{
	//Sleep(2000);//休眠 2s
}

int Calc_Position(Node * node)
{
	int temp = 0, zero_count = 0, _bar = 0;
	//有值=无值
	if ((node->Node_List_Length - node->Remain_Zero_Count) == node->Remain_Zero_Count) {
		if (node->Tag != 1) {//不是开始的情况，传播一个节点时的情况会出现 有值=无值
			if (node->Tag == 3) {//表示是尾部的有值=无值，且已经同步 1 的个数，直接计算尾部的情况
				temp = 0;
				int one_count = 0, j = 1;
				one_count = node->Node_List_Length - node->Remain_Zero_Count;//自己 1 的个数
				for (int m = 0; m < node->Node_List_Length; ++m)
				{
					if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0) {//找到位置为 0 的
						if ((node->Rece_Node_Info[m][1] & 0XC0000000) == 0) {//如果还有未同步 1 的
							_bar = 1;
							break;
						}
						else if (one_count < (node->Rece_Node_Info[m][1] & 0X7FFF))
						{
							++j;
						}
					}
					//保存当前最大位置
					else if (temp < ((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15))
					{
						temp = (node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15;
					}
				}
				//判断并生成自己的位置
				if (_bar == 0)
				{
					//cout << node->Id << "有值=无值，尾部节点，已确定位置==122\n";
					node->Num = temp + j;
					return 4;
				}
				else {
					//cout << node->Id << "有值=无值，等待，未确定位置==127 Tag=" << node->Tag << endl;
					//Wait();
					return 0;
				}
			}
			else {//中间节点
				for (int m = 0; m < node->Node_List_Length; ++m)
				{
					if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0)
						continue;
					else if (temp < ((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15))
					{
						//保存当前最大位置以及它的0的个数
						temp = (node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15;
						zero_count = node->Rece_Node_Info[m][1] & 0X7FFF;
					}
				}
				//比较 0 个数，判断并生成自己的位置
				if (zero_count <= node->Node_List_Length)
				{
					//cout << node->Id << "有值=无值，已确定位置==147\n";
					node->Num = temp + 1;
					return 4;
				}
				else {
					//cout << node->Id << "有值=无值，等待，未确定位置==152  Tag=" << node->Tag << endl;
					//Wait();
					return 0;
				}
			}
		}
		else {//是开始的情况,传播一个节点时的情况会出现 有值=无值
			temp = 1;
			//比较 0 个数的大小，并生成自己的位置
			for (int m = 0; m < node->Node_List_Length; ++m)
			{
				if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0)
				{
					if ((node->Rece_Node_Info[m][1] & 0X7FFF) < (node->Node_List_Length - 1)) {
						++temp;
						for (int k = m + 1; k < node->Node_List_Length; ++k) {//重复的第一个跳过
							if ((node->Rece_Node_Info[k][1] & 0X3FFF8000) == 0)
								if ((node->Rece_Node_Info[m][1] & 0X7FFF) == (node->Rece_Node_Info[k][1] & 0X7FFF)) {
									--temp;
									break;
								}
						}
					}
				}
			}
			cout << node->Id << "有值>无值,开始节点，已确定位置\n";
			node->Num = temp + 1;
			return 4;
		}
	}
	//有值<无值
	else if ((node->Node_List_Length - node->Remain_Zero_Count) < node->Remain_Zero_Count)
	{
		if (node->Tag != 3) {//不是尾部的计算
			if (node->Tag == 1) {
				temp = 1;
				//比较 0 个数的大小，并生成自己的位置
				for (int m = 0; m < node->Node_List_Length; ++m)
				{
					if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0)
					{
						if ((node->Rece_Node_Info[m][1] & 0X7FFF) < (node->Node_List_Length - 1)) {
							++temp;
							for (int k = m + 1; k < node->Node_List_Length; ++k) {//重复的第一个跳过
								if ((node->Rece_Node_Info[k][1] & 0X3FFF8000) == 0)
									if ((node->Rece_Node_Info[m][1] & 0X7FFF) == (node->Rece_Node_Info[k][1] & 0X7FFF)) {
										--temp;
										break;
									}
							}
						}
					}
				}
				//cout << node->Id << "有值<无值,开始节点，已确定位置\n";
				node->Num = temp + 1;
				return 4;
			}
			else {
				//cout << node->Id << "有值<无值,等待节点，未确定位置 ==135 \n";
				//Wait();
				return 0;
			}
		}
		else//是尾部的计算，这种情况也会出现 有值<无值
		{
			temp = 0;
			int one_count = 0, j = 1;
			one_count = node->Node_List_Length - node->Remain_Zero_Count;//自己 1 的个数
			for (int m = 0; m < node->Node_List_Length; ++m)
			{
				if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0) {//找到位置为 0 的
					if ((node->Rece_Node_Info[m][1] & 0XC0000000) == 0) {//如果还有未同步 1 的
						_bar = 1;
						break;
					}
					else if (one_count < (node->Rece_Node_Info[m][1] & 0X7FFF))
					{
						++j;
					}
				}
				//保存当前最大位置
				else if (temp < ((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15))
				{
					temp = (node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15;
				}
			}
			//判断并生成自己的位置
			if (_bar == 0)
			{
				//cout << node->Id << "有值<无值,尾部节点，已确定位置==122\n";
				node->Num = temp + j;
				return 4;
			}
			else {
				//cout << node->Id << "有值=无值，等待，未确定位置==127 Tag=" << node->Tag << endl;
				//Wait();
				return 0;
			}
		}
	}
	//有值>无值 一定是尾部的情况
	else {
		for (int m = 0; m < node->Node_List_Length; ++m)
		{
			//判断位置为 0 的收到的包是否全部是统计 1 的个数
			if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0) {
				if ((node->Rece_Node_Info[m][1] & 0XC0000000) >> 30 != 1)
				{
					//cout << node->Id << "有值>无值,1未同步完毕，未确定位置 ==199  Tag=" << node->Tag << endl;
					node->Tag = 3;
					//Wait();
					return 0;
				}
			}
		}
		//比较 1 个数的大小，并生成自己的位置
		temp = 1;
		zero_count = 0;
		for (int m = 0; m < node->Node_List_Length; ++m)
		{
			if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0) {
				if ((node->Rece_Node_Info[m][1] & 0X7FFF) > (node->Node_List_Length - node->Remain_Zero_Count))
					++temp;
			}
			else {//保存当前最大位置
				if (((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15) > zero_count)
					zero_count = (node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15;
			}
		}
		//cout << node->Id << "有值>无值,已经确定位置\n";
		node->Num = temp + zero_count;
		zero_count = 0;
		return 4;
	}
}

void Print_Info(Node * node)
{
	cout << "Id："<< node->Id << " \t" ;
	cout << "Location："<< node->Num << " \t" ;
	//cout << "clock："<< node->Clock * 0.2 << " s\t" ;
	//cout << "Adjacency table length："<< node->Node_List_Length << " \t" ;
	cout << "Total number of packets sent："<< node->Send_Mes_Count << " \t" ;
	cout << "Total number of packets received："<< node->Rece_Mes_Count << " \t" << endl;
}

void Print_Rece_Node_Info(Node * node)
{
	cout << "Id：" << node->Id << " \t" << endl;
	int j = 0,k=0;
	for (int i = 0; i < node->Node_List_Length; ++i) {
		j = (0x00007FFF & node->Rece_Node_Info[i][1]);
		k = ((node->Rece_Node_Info[i][1] & 0X3FFF8000) >> 15);
		cout << "接收的id：" << node->Rece_Node_Info[i][0] << "\t" ;
		cout << "其位置为：" << k << " \t" ;
		cout << "0的个数：" << j << " \t" << endl;
	}
}

int Stop_Send(Node * node)
{
	int length = 0, stop_0 = 0, stop_1 = 0;
	length = node->Node_List_Length;
	if (length == 0) {
		cout << node->Id << " Stop_Send =0 == 176\n";
		return 0;
	}
	//判断表中数据是否全部都确定了位置
	for (int m = 0; m < node->Node_List_Length; ++m)
	{
		if (((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15) == 0)//有未确定位置的
			stop_0 = 1;
		else//有确定位置的
			stop_1 = 1;
	}
	if ((stop_0 == 0) && node->Num != 0) 	//自己位置确定，表中所有的位置都确定，程序终止条件
	{
		cout << node->Id << " Stop_Send = 6 == 189\n";
		return 6;
	}
	if ((stop_0 == 1) && (stop_1 == 1))
	{
		if (node->Num == 0) {//自己位置未确定，停止发，接收
			if (node->Tag == 3) {
				for (int m = 0; m < node->Node_List_Length; ++m) {
					if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0)
						if ((node->Rece_Node_Info[m][1] >> 30) != 1) {
							cout << node->Id << " 需要同步 1 个数 Stop_Send = 7 == 199\n";
							return 7;
						}
				}
				cout << node->Id << "通过 1 个数计算位置 Stop_Send = 8 == 203\n";
				return 8;
			}
			else {
				cout << node->Id << " Stop_Send = 5 == 206 Tag= "<<node->Tag<<"\n";
				//Wait();  //多线程使用
				return 5;
			}
		}
		else  //自己位置确定，但是表中还有未确定的，则继续发更新包
		{
			cout << node->Id << " 自己位置确定，但是表中还有未确定的，则继续发更新包 Stop_Send = 0 == 211\n";
			return 0;
		}
	}
	else
	{		//表变
		if (Stable(node) == 0)
		{
			cout << node->Id << " 表变化 Stop_Send = 0 \n";
			return 0;
		}
		else {
			cout << node->Id << " Stop_Send = 5 ==307  Tag= " << node->Tag << "\n";
			//Wait();//多线程使用
			return 5;
		}
	}
}

int Stable(Node * node)
{
	if (node->Node_List_Length == 0)
	{
		return 0;
	}
	int temp = 0;
	for (int i = 0; i < node->Node_List_Length; ++i) {
		if ((node->Node_Stable[node->Rece_Node_Info[i][0]] == 0))
			temp = 1;
	}
	if(temp == 0) return 1;
	return 0;
}

void testNode(Node * node)
{
	cout << "当前节点id：" << node->Id << endl;
	for (int i = 0; i < node->Node_List_Length; ++i)
	{
		cout << "收到包的id："<<node->Rece_Node_Info[i][0] << endl;
		cout << "收到包的位置： " << (node->Rece_Node_Info[i][1]>>15) << endl;
	}
}
