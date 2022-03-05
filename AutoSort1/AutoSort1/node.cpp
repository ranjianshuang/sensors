#include "node.h"
#include "windows.h"
int NodeCountNum;
int COMMU_DIS = 5;
Node * CreatNode(int i)
{
	Node * node;
	node = (Node *)malloc(sizeof(Node)*i);

	//��һ���ڵ��ʼ��
	node[0].Id = 1;
	node[0].Distance = 0;
	node[0].Num = 1;
	node[0].Tag = 0;
	node[0].Clock = 3;//��ʾ3��0.2s
	node[0].Flag = false;
	node[0].Val = 0;
	
	//�ڵ���Ϣ���ʼ��
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
	//�����ڵ��ʼ��
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
		node[j].Distance = j * EQUAL_DIS;//�������ɼ��Ϊ EQUAL_DIS
		node[j].Num = 0;
		node[j].Tag = 0;
		node[j].Flag = false;
		node[j].Val = 0;
		
		//�ڵ���Ϣ���ʼ��
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
	//Sleep(2000);//���� 2s
}

int Calc_Position(Node * node)
{
	int temp = 0, zero_count = 0, _bar = 0;
	//��ֵ=��ֵ
	if ((node->Node_List_Length - node->Remain_Zero_Count) == node->Remain_Zero_Count) {
		if (node->Tag != 1) {//���ǿ�ʼ�����������һ���ڵ�ʱ���������� ��ֵ=��ֵ
			if (node->Tag == 3) {//��ʾ��β������ֵ=��ֵ�����Ѿ�ͬ�� 1 �ĸ�����ֱ�Ӽ���β�������
				temp = 0;
				int one_count = 0, j = 1;
				one_count = node->Node_List_Length - node->Remain_Zero_Count;//�Լ� 1 �ĸ���
				for (int m = 0; m < node->Node_List_Length; ++m)
				{
					if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0) {//�ҵ�λ��Ϊ 0 ��
						if ((node->Rece_Node_Info[m][1] & 0XC0000000) == 0) {//�������δͬ�� 1 ��
							_bar = 1;
							break;
						}
						else if (one_count < (node->Rece_Node_Info[m][1] & 0X7FFF))
						{
							++j;
						}
					}
					//���浱ǰ���λ��
					else if (temp < ((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15))
					{
						temp = (node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15;
					}
				}
				//�жϲ������Լ���λ��
				if (_bar == 0)
				{
					//cout << node->Id << "��ֵ=��ֵ��β���ڵ㣬��ȷ��λ��==122\n";
					node->Num = temp + j;
					return 4;
				}
				else {
					//cout << node->Id << "��ֵ=��ֵ���ȴ���δȷ��λ��==127 Tag=" << node->Tag << endl;
					//Wait();
					return 0;
				}
			}
			else {//�м�ڵ�
				for (int m = 0; m < node->Node_List_Length; ++m)
				{
					if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0)
						continue;
					else if (temp < ((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15))
					{
						//���浱ǰ���λ���Լ�����0�ĸ���
						temp = (node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15;
						zero_count = node->Rece_Node_Info[m][1] & 0X7FFF;
					}
				}
				//�Ƚ� 0 �������жϲ������Լ���λ��
				if (zero_count <= node->Node_List_Length)
				{
					//cout << node->Id << "��ֵ=��ֵ����ȷ��λ��==147\n";
					node->Num = temp + 1;
					return 4;
				}
				else {
					//cout << node->Id << "��ֵ=��ֵ���ȴ���δȷ��λ��==152  Tag=" << node->Tag << endl;
					//Wait();
					return 0;
				}
			}
		}
		else {//�ǿ�ʼ�����,����һ���ڵ�ʱ���������� ��ֵ=��ֵ
			temp = 1;
			//�Ƚ� 0 �����Ĵ�С���������Լ���λ��
			for (int m = 0; m < node->Node_List_Length; ++m)
			{
				if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0)
				{
					if ((node->Rece_Node_Info[m][1] & 0X7FFF) < (node->Node_List_Length - 1)) {
						++temp;
						for (int k = m + 1; k < node->Node_List_Length; ++k) {//�ظ��ĵ�һ������
							if ((node->Rece_Node_Info[k][1] & 0X3FFF8000) == 0)
								if ((node->Rece_Node_Info[m][1] & 0X7FFF) == (node->Rece_Node_Info[k][1] & 0X7FFF)) {
									--temp;
									break;
								}
						}
					}
				}
			}
			cout << node->Id << "��ֵ>��ֵ,��ʼ�ڵ㣬��ȷ��λ��\n";
			node->Num = temp + 1;
			return 4;
		}
	}
	//��ֵ<��ֵ
	else if ((node->Node_List_Length - node->Remain_Zero_Count) < node->Remain_Zero_Count)
	{
		if (node->Tag != 3) {//����β���ļ���
			if (node->Tag == 1) {
				temp = 1;
				//�Ƚ� 0 �����Ĵ�С���������Լ���λ��
				for (int m = 0; m < node->Node_List_Length; ++m)
				{
					if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0)
					{
						if ((node->Rece_Node_Info[m][1] & 0X7FFF) < (node->Node_List_Length - 1)) {
							++temp;
							for (int k = m + 1; k < node->Node_List_Length; ++k) {//�ظ��ĵ�һ������
								if ((node->Rece_Node_Info[k][1] & 0X3FFF8000) == 0)
									if ((node->Rece_Node_Info[m][1] & 0X7FFF) == (node->Rece_Node_Info[k][1] & 0X7FFF)) {
										--temp;
										break;
									}
							}
						}
					}
				}
				//cout << node->Id << "��ֵ<��ֵ,��ʼ�ڵ㣬��ȷ��λ��\n";
				node->Num = temp + 1;
				return 4;
			}
			else {
				//cout << node->Id << "��ֵ<��ֵ,�ȴ��ڵ㣬δȷ��λ�� ==135 \n";
				//Wait();
				return 0;
			}
		}
		else//��β���ļ��㣬�������Ҳ����� ��ֵ<��ֵ
		{
			temp = 0;
			int one_count = 0, j = 1;
			one_count = node->Node_List_Length - node->Remain_Zero_Count;//�Լ� 1 �ĸ���
			for (int m = 0; m < node->Node_List_Length; ++m)
			{
				if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0) {//�ҵ�λ��Ϊ 0 ��
					if ((node->Rece_Node_Info[m][1] & 0XC0000000) == 0) {//�������δͬ�� 1 ��
						_bar = 1;
						break;
					}
					else if (one_count < (node->Rece_Node_Info[m][1] & 0X7FFF))
					{
						++j;
					}
				}
				//���浱ǰ���λ��
				else if (temp < ((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15))
				{
					temp = (node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15;
				}
			}
			//�жϲ������Լ���λ��
			if (_bar == 0)
			{
				//cout << node->Id << "��ֵ<��ֵ,β���ڵ㣬��ȷ��λ��==122\n";
				node->Num = temp + j;
				return 4;
			}
			else {
				//cout << node->Id << "��ֵ=��ֵ���ȴ���δȷ��λ��==127 Tag=" << node->Tag << endl;
				//Wait();
				return 0;
			}
		}
	}
	//��ֵ>��ֵ һ����β�������
	else {
		for (int m = 0; m < node->Node_List_Length; ++m)
		{
			//�ж�λ��Ϊ 0 ���յ��İ��Ƿ�ȫ����ͳ�� 1 �ĸ���
			if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0) {
				if ((node->Rece_Node_Info[m][1] & 0XC0000000) >> 30 != 1)
				{
					//cout << node->Id << "��ֵ>��ֵ,1δͬ����ϣ�δȷ��λ�� ==199  Tag=" << node->Tag << endl;
					node->Tag = 3;
					//Wait();
					return 0;
				}
			}
		}
		//�Ƚ� 1 �����Ĵ�С���������Լ���λ��
		temp = 1;
		zero_count = 0;
		for (int m = 0; m < node->Node_List_Length; ++m)
		{
			if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0) {
				if ((node->Rece_Node_Info[m][1] & 0X7FFF) > (node->Node_List_Length - node->Remain_Zero_Count))
					++temp;
			}
			else {//���浱ǰ���λ��
				if (((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15) > zero_count)
					zero_count = (node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15;
			}
		}
		//cout << node->Id << "��ֵ>��ֵ,�Ѿ�ȷ��λ��\n";
		node->Num = temp + zero_count;
		zero_count = 0;
		return 4;
	}
}

void Print_Info(Node * node)
{
	cout << "Id��"<< node->Id << " \t" ;
	cout << "Location��"<< node->Num << " \t" ;
	//cout << "clock��"<< node->Clock * 0.2 << " s\t" ;
	//cout << "Adjacency table length��"<< node->Node_List_Length << " \t" ;
	cout << "Total number of packets sent��"<< node->Send_Mes_Count << " \t" ;
	cout << "Total number of packets received��"<< node->Rece_Mes_Count << " \t" << endl;
}

void Print_Rece_Node_Info(Node * node)
{
	cout << "Id��" << node->Id << " \t" << endl;
	int j = 0,k=0;
	for (int i = 0; i < node->Node_List_Length; ++i) {
		j = (0x00007FFF & node->Rece_Node_Info[i][1]);
		k = ((node->Rece_Node_Info[i][1] & 0X3FFF8000) >> 15);
		cout << "���յ�id��" << node->Rece_Node_Info[i][0] << "\t" ;
		cout << "��λ��Ϊ��" << k << " \t" ;
		cout << "0�ĸ�����" << j << " \t" << endl;
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
	//�жϱ��������Ƿ�ȫ����ȷ����λ��
	for (int m = 0; m < node->Node_List_Length; ++m)
	{
		if (((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15) == 0)//��δȷ��λ�õ�
			stop_0 = 1;
		else//��ȷ��λ�õ�
			stop_1 = 1;
	}
	if ((stop_0 == 0) && node->Num != 0) 	//�Լ�λ��ȷ�����������е�λ�ö�ȷ����������ֹ����
	{
		cout << node->Id << " Stop_Send = 6 == 189\n";
		return 6;
	}
	if ((stop_0 == 1) && (stop_1 == 1))
	{
		if (node->Num == 0) {//�Լ�λ��δȷ����ֹͣ��������
			if (node->Tag == 3) {
				for (int m = 0; m < node->Node_List_Length; ++m) {
					if ((node->Rece_Node_Info[m][1] & 0X3FFF8000) == 0)
						if ((node->Rece_Node_Info[m][1] >> 30) != 1) {
							cout << node->Id << " ��Ҫͬ�� 1 ���� Stop_Send = 7 == 199\n";
							return 7;
						}
				}
				cout << node->Id << "ͨ�� 1 ��������λ�� Stop_Send = 8 == 203\n";
				return 8;
			}
			else {
				cout << node->Id << " Stop_Send = 5 == 206 Tag= "<<node->Tag<<"\n";
				//Wait();  //���߳�ʹ��
				return 5;
			}
		}
		else  //�Լ�λ��ȷ�������Ǳ��л���δȷ���ģ�����������°�
		{
			cout << node->Id << " �Լ�λ��ȷ�������Ǳ��л���δȷ���ģ�����������°� Stop_Send = 0 == 211\n";
			return 0;
		}
	}
	else
	{		//���
		if (Stable(node) == 0)
		{
			cout << node->Id << " ��仯 Stop_Send = 0 \n";
			return 0;
		}
		else {
			cout << node->Id << " Stop_Send = 5 ==307  Tag= " << node->Tag << "\n";
			//Wait();//���߳�ʹ��
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
	cout << "��ǰ�ڵ�id��" << node->Id << endl;
	for (int i = 0; i < node->Node_List_Length; ++i)
	{
		cout << "�յ�����id��"<<node->Rece_Node_Info[i][0] << endl;
		cout << "�յ�����λ�ã� " << (node->Rece_Node_Info[i][1]>>15) << endl;
	}
}
