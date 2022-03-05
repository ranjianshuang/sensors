#include "communication.h"
extern int NodeCountNum;
extern int COMMU_DIS;
void head_Send_Commun_Zero(Node * node)
{
	Node * temp;
	temp = node;
	int dis = 1;
	dis <<= 15;
	dis += node->Node_List_Length;
	int Meg[1][2];
	Meg[0][0] = node->Id;
	/*
		Mes[0][1] 共 32 位 ，
		前两位表示消息类型；
		中间 15 位表示节点位置；
		最后面 15 位表示 0 的个数；
	*/
	Meg[0][1] = dis;
	dis = COMMU_DIS / EQUAL_DIS;
	int id = node->Id;
	++node->Send_Mes_Count;
	//向右传递消息
	while (dis)
	{
		if (id == NodeCountNum) { break; }
		else
		{
			--dis;
			++id;
			++node;
			//cout << temp->Id << "向右发包 == 25" << " 其位置为：" << temp->Num << " 表长度为：" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
}

void send_Commun_Zero(Node * node)
{
	Node * temp;
	temp = node;
	int Meg[1][2];
	Meg[0][0] = node->Id;
	/*
		Mes[0][1] 共 32 位 ，
		前两位表示消息类型；
		中间 15 位表示节点位置；
		最后面 15 位表示 0 的个数；
	*/
	Meg[0][1] = 0 + node->Node_List_Length;
	int dis = COMMU_DIS / EQUAL_DIS;
	int id = node->Id;
	++node->Send_Mes_Count;
	//向左传递消息
	while (dis)
	{
		if (id == 2) {	break;	}//多线程中改为 id == 1 
		else
		{
			--dis;
			--id;
			--node;
			//cout << temp->Id << "向左发包 == 51" << " 其位置为：" << temp->Num << " 表长度为：" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
	//向右传递消息
	node = temp;
	id = node->Id;
	dis = COMMU_DIS / EQUAL_DIS;
	while (dis)
	{
		if (id == NodeCountNum) {	break;	}
		else
		{
			--dis;
			++id;
			++node;
			//cout << temp->Id << "向右发包 == 65" << " 其位置为：" << temp->Num << " 表长度为：" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
}

void send_Commun_One(Node * node)
{
	Node * temp;
	temp = node;
	int Meg[1][2];
	Meg[0][0] = node->Id;
	/*
		Mes[0][1] 共 32 位 ，
		前两位表示消息类型；
		中间 15 位表示节点位置；
		最后面 15 位表示 0 的个数；
	*/
	int id = 1;
	id <<= 30; //左移30位
	Meg[0][1] = id + node->Node_List_Length - node->Remain_Zero_Count;
	int dis = COMMU_DIS / EQUAL_DIS ;
	id = node->Id;
	++node->Send_Mes_Count;
	//向左传递消息
	while (dis)
	{
		if (id == 1) {	break;	}
		else
		{
			--dis;
			--id;
			--node;
			//cout << temp->Id << "向左发同步1的包 == 99" << " 其位置为：" << temp->Num << " 表长度为：" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
	//向右传递消息
	node = temp;
	id = node->Id;
	dis = COMMU_DIS / EQUAL_DIS;
	while (dis)
	{
		if (id == NodeCountNum) {	break;	}
		else
		{
			--dis = COMMU_DIS / EQUAL_DIS;
			++id;
			++node;
			//cout << temp->Id << "向右发同步1的包 == 113" << " 其位置为：" << temp->Num << " 表长度为：" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
}

void send_Update(Node * node)
{
	Node * temp;
	temp = node;
	int Meg[1][2];
	Meg[0][0] = node->Id;
	/*
		Mes[0][1] 共 32 位 ，
		前两位表示消息类型；
		中间 15 位表示节点位置；
		最后面 15 位表示 0 的个数；
	*/
	
	int dis = COMMU_DIS / EQUAL_DIS;
	int id = node->Num;
	id <<= 15;
	Meg[0][1] = id + node->Node_List_Length;
	//向左传递消息
	id = node->Id;
	while (dis)
	{
		if (id == 1) {	break;	}
		else
		{
			--dis;
			--id;
			--node;
			++node->Send_Mes_Count;
			//cout << temp->Id << "向左发更新包 == 145" <<" 其位置为：" << temp->Num << " 表长度为：" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
	//向右传递消息
	node = temp;
	id = node->Id;
	dis = COMMU_DIS / EQUAL_DIS;
	while (dis)
	{
		if (id == NodeCountNum) {	break;	}
		else
		{
			--dis;
			++id;
			++node;
			++node->Send_Mes_Count;
			//cout << temp->Id << "向右发更新包== 159" << " 其位置为：" << temp->Num << " 表长度为：" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
}

void received_Meg(Node * node, int meg[1][2])
{
	if (node->Num == 0 || node->Num == 1) {
		int i, j, k;
		i = meg[0][1] & 0xC0000000; //取前 2 位
		i >>= 30;
		j = meg[0][1] & 0x3FFF8000;	//取中间 15 位
		j >>= 15;
		k = meg[0][1] & 0x00007FFF; //取低 15 位
		int _flag = 0, _tag = 0;
		//cout << node->Id << "收到" << meg[0][0] << "的包 == 196" << " i=" << i << " j= " << j << " k= " << k << endl;
		//收到更新包
		if (j != 0) {
			if (node->Num != 0) {//自己位置已经确定，只需要更新即可
				++node->Rece_Mes_Count;
			}
			else if(node->Tag != 3)//自己位置未确定
			{
				for (int m = 0; m < node->Node_List_Length; ++m) {
					if (meg[0][0] == node->Rece_Node_Info[m][0]) { //找到该id
						_tag = 1;
						if (j == 1) {//是否为头结点发的数据包
							//cout << node->Id << "收到头结点更新包,是重复包,丢掉 == 208" << endl;
							++node->Rece_Mes_Count;
							node->Tag = 1;//是重复包
							break;
						}
						else if (node->Rece_Node_Info[m][1] != meg[0][1])//不是头结点且不是重复包
						{
							for (int m = node->Node_List_Length-1; m >= 0; --m) {
								if (((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15) == 1) {//有 1 节点
									//cout << node->Id << "收到非头结点更新包，但自己在开始的范围中，暂不处理该包 == 216" << endl;
									++node->Rece_Mes_Count;
									_flag = 1;//暂时不处理
									break;
								}
							}
							if (_flag != 1) {//为普通节点更新包
								if (node->Rece_Node_Info[m][1] != meg[0][1])//不是重复包
								{
									//cout << node->Id << "收到更新包,不是重复包 == 225" << endl;
									node->Rece_Node_Info[m][1] = meg[0][1];
									--node->Remain_Zero_Count;
									++node->Rece_Mes_Count;
									if (node->Tag != 3)
										node->Tag = 2;
									break;
								}
								else {
									//cout << node->Id << "收到更新包,是重复包 == 231" << endl;
									if (node->Tag != 3)
										node->Tag = 2;//是重复包
									break;
								}
							}
							else {//收到非头结点更新包，但自己在开始的范围中，暂不处理该包
								node->Tag = 1;//暂时不处理
								_flag = 0;
								break;
							}
						}
					}
				}
				if (_tag == 0) {//第一次收到头结点 1 的包
					if (j == 1) {
						//cout << node->Id << "第一次收到头结点 1 的包 == 226" << endl;
						node->Rece_Node_Info[node->Node_List_Length][0] = meg[0][0];
						node->Rece_Node_Info[node->Node_List_Length][1] = meg[0][1];
						++node->Node_List_Length;//长度 +1
						++node->Rece_Mes_Count;
						node->Tag = 1;
					}
				}
				else {
					_tag = 0;
				}
			}
		}
		//收到普通通信包
		else if (i == 0)
		{
			for (int m = 0; m < node->Node_List_Length; ++m) {
				if (meg[0][0] == node->Rece_Node_Info[m][0]) { //找到该id
					_flag = 1;
					if ((0x00007FFF & node->Rece_Node_Info[m][1]) == k && k != 0) { //重复收到包，不处理
						//cout << node->Id << "收到普通通信包,重复包 == 249" << endl;
						if (node->Num == 1)//自己（非头结点）首先得发过包，再收到重复包
						{
							node->Node_Stable[meg[0][0]] = 1;
						}
						else if (node->Send_Mes_Count != 0) {
							node->Node_Stable[meg[0][0]] = 1;
						}
						++node->Rece_Mes_Count;
						node->Tag = 0;
					}
					else  //不是重复包，更新
					{
						//且原来的数据不是 1 的个数
						if ((node->Rece_Node_Info[m][1] >> 30) != 1) {
							//cout << node->Id << "收到普通通信包,不是重复包，且原来的数据不是 1 的个数 == 253" << endl;
							node->Rece_Node_Info[m][1] = meg[0][1];
							++node->Rece_Mes_Count;
							++node->Rece_Mes_Count;
							node->Tag = 0;
						}
					}
				}
			}
			if (_flag != 1) {//未进入循环，表示该包为第一次收到，直接写入
				//cout << node->Id << "第一次收到普通通信包,不是重复包 == 261" << endl;
				node->Rece_Node_Info[node->Node_List_Length][0] = meg[0][0];
				node->Rece_Node_Info[node->Node_List_Length][1] = meg[0][1];
				++node->Node_List_Length;//长度 +1
				++node->Remain_Zero_Count;
				++node->Rece_Mes_Count;
				//node->Val = 1;//表示第一次收到普通通信包，表长有变化
				node->Flag = true;
				node->Tag = 0;
			}
			else
			{
				_flag = 0;
			}
		}

		//收到统计 1 的包
		else if (i == 1)
		{
			node->Tag = 3;
			for (int m = 0; m < node->Node_List_Length; ++m) {
				if (meg[0][0] == node->Rece_Node_Info[m][0]) { //找到该id
					if ((0xC0000000 & node->Rece_Node_Info[m][1]) >> 30 == 1) { //重复收到包，不处理
						//cout << node->Id << "重复收到统计 1 的包,丢掉 == 282" << endl;
						++node->Rece_Mes_Count;
						break;
					}
					else  //第一次收到统计 1 的包，更新
					{
						//cout << node->Id << "第一次收到统计 1 的包,不是重复包 == 279" << endl;
						node->Rece_Node_Info[m][1] = meg[0][1];
						++node->Rece_Mes_Count;
						break;
					}
				}
			}
		}
	}
}

int one_Stable(Node * node)
{
	int flag = 0;
	if (node->Tag == 3) {
		for (int m = 0; m < node->Node_List_Length; ++m) {
			//所有没有确定位置的信息是未同步 1 的包，则同步完成
			if (((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15) == 0) {
				if (((0xC0000000 & node->Rece_Node_Info[m][1]) >> 30) != 1) {
					flag = 1; //表示存在未确定位置的还未收到同步 1 的包
					break;
				}
			}
		}
		if (flag == 1) {
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
		return 0;
}

void testFunc()
{
	cout << NodeCountNum<<endl;
}








