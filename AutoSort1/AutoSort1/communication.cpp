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
		Mes[0][1] �� 32 λ ��
		ǰ��λ��ʾ��Ϣ���ͣ�
		�м� 15 λ��ʾ�ڵ�λ�ã�
		����� 15 λ��ʾ 0 �ĸ�����
	*/
	Meg[0][1] = dis;
	dis = COMMU_DIS / EQUAL_DIS;
	int id = node->Id;
	++node->Send_Mes_Count;
	//���Ҵ�����Ϣ
	while (dis)
	{
		if (id == NodeCountNum) { break; }
		else
		{
			--dis;
			++id;
			++node;
			//cout << temp->Id << "���ҷ��� == 25" << " ��λ��Ϊ��" << temp->Num << " ����Ϊ��" << temp->Node_List_Length << endl;
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
		Mes[0][1] �� 32 λ ��
		ǰ��λ��ʾ��Ϣ���ͣ�
		�м� 15 λ��ʾ�ڵ�λ�ã�
		����� 15 λ��ʾ 0 �ĸ�����
	*/
	Meg[0][1] = 0 + node->Node_List_Length;
	int dis = COMMU_DIS / EQUAL_DIS;
	int id = node->Id;
	++node->Send_Mes_Count;
	//���󴫵���Ϣ
	while (dis)
	{
		if (id == 2) {	break;	}//���߳��и�Ϊ id == 1 
		else
		{
			--dis;
			--id;
			--node;
			//cout << temp->Id << "���󷢰� == 51" << " ��λ��Ϊ��" << temp->Num << " ����Ϊ��" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
	//���Ҵ�����Ϣ
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
			//cout << temp->Id << "���ҷ��� == 65" << " ��λ��Ϊ��" << temp->Num << " ����Ϊ��" << temp->Node_List_Length << endl;
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
		Mes[0][1] �� 32 λ ��
		ǰ��λ��ʾ��Ϣ���ͣ�
		�м� 15 λ��ʾ�ڵ�λ�ã�
		����� 15 λ��ʾ 0 �ĸ�����
	*/
	int id = 1;
	id <<= 30; //����30λ
	Meg[0][1] = id + node->Node_List_Length - node->Remain_Zero_Count;
	int dis = COMMU_DIS / EQUAL_DIS ;
	id = node->Id;
	++node->Send_Mes_Count;
	//���󴫵���Ϣ
	while (dis)
	{
		if (id == 1) {	break;	}
		else
		{
			--dis;
			--id;
			--node;
			//cout << temp->Id << "����ͬ��1�İ� == 99" << " ��λ��Ϊ��" << temp->Num << " ����Ϊ��" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
	//���Ҵ�����Ϣ
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
			//cout << temp->Id << "���ҷ�ͬ��1�İ� == 113" << " ��λ��Ϊ��" << temp->Num << " ����Ϊ��" << temp->Node_List_Length << endl;
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
		Mes[0][1] �� 32 λ ��
		ǰ��λ��ʾ��Ϣ���ͣ�
		�м� 15 λ��ʾ�ڵ�λ�ã�
		����� 15 λ��ʾ 0 �ĸ�����
	*/
	
	int dis = COMMU_DIS / EQUAL_DIS;
	int id = node->Num;
	id <<= 15;
	Meg[0][1] = id + node->Node_List_Length;
	//���󴫵���Ϣ
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
			//cout << temp->Id << "���󷢸��°� == 145" <<" ��λ��Ϊ��" << temp->Num << " ����Ϊ��" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
	//���Ҵ�����Ϣ
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
			//cout << temp->Id << "���ҷ����°�== 159" << " ��λ��Ϊ��" << temp->Num << " ����Ϊ��" << temp->Node_List_Length << endl;
			received_Meg(node, Meg);
		}
	}
}

void received_Meg(Node * node, int meg[1][2])
{
	if (node->Num == 0 || node->Num == 1) {
		int i, j, k;
		i = meg[0][1] & 0xC0000000; //ȡǰ 2 λ
		i >>= 30;
		j = meg[0][1] & 0x3FFF8000;	//ȡ�м� 15 λ
		j >>= 15;
		k = meg[0][1] & 0x00007FFF; //ȡ�� 15 λ
		int _flag = 0, _tag = 0;
		//cout << node->Id << "�յ�" << meg[0][0] << "�İ� == 196" << " i=" << i << " j= " << j << " k= " << k << endl;
		//�յ����°�
		if (j != 0) {
			if (node->Num != 0) {//�Լ�λ���Ѿ�ȷ����ֻ��Ҫ���¼���
				++node->Rece_Mes_Count;
			}
			else if(node->Tag != 3)//�Լ�λ��δȷ��
			{
				for (int m = 0; m < node->Node_List_Length; ++m) {
					if (meg[0][0] == node->Rece_Node_Info[m][0]) { //�ҵ���id
						_tag = 1;
						if (j == 1) {//�Ƿ�Ϊͷ��㷢�����ݰ�
							//cout << node->Id << "�յ�ͷ�����°�,���ظ���,���� == 208" << endl;
							++node->Rece_Mes_Count;
							node->Tag = 1;//���ظ���
							break;
						}
						else if (node->Rece_Node_Info[m][1] != meg[0][1])//����ͷ����Ҳ����ظ���
						{
							for (int m = node->Node_List_Length-1; m >= 0; --m) {
								if (((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15) == 1) {//�� 1 �ڵ�
									//cout << node->Id << "�յ���ͷ�����°������Լ��ڿ�ʼ�ķ�Χ�У��ݲ�����ð� == 216" << endl;
									++node->Rece_Mes_Count;
									_flag = 1;//��ʱ������
									break;
								}
							}
							if (_flag != 1) {//Ϊ��ͨ�ڵ���°�
								if (node->Rece_Node_Info[m][1] != meg[0][1])//�����ظ���
								{
									//cout << node->Id << "�յ����°�,�����ظ��� == 225" << endl;
									node->Rece_Node_Info[m][1] = meg[0][1];
									--node->Remain_Zero_Count;
									++node->Rece_Mes_Count;
									if (node->Tag != 3)
										node->Tag = 2;
									break;
								}
								else {
									//cout << node->Id << "�յ����°�,���ظ��� == 231" << endl;
									if (node->Tag != 3)
										node->Tag = 2;//���ظ���
									break;
								}
							}
							else {//�յ���ͷ�����°������Լ��ڿ�ʼ�ķ�Χ�У��ݲ�����ð�
								node->Tag = 1;//��ʱ������
								_flag = 0;
								break;
							}
						}
					}
				}
				if (_tag == 0) {//��һ���յ�ͷ��� 1 �İ�
					if (j == 1) {
						//cout << node->Id << "��һ���յ�ͷ��� 1 �İ� == 226" << endl;
						node->Rece_Node_Info[node->Node_List_Length][0] = meg[0][0];
						node->Rece_Node_Info[node->Node_List_Length][1] = meg[0][1];
						++node->Node_List_Length;//���� +1
						++node->Rece_Mes_Count;
						node->Tag = 1;
					}
				}
				else {
					_tag = 0;
				}
			}
		}
		//�յ���ͨͨ�Ű�
		else if (i == 0)
		{
			for (int m = 0; m < node->Node_List_Length; ++m) {
				if (meg[0][0] == node->Rece_Node_Info[m][0]) { //�ҵ���id
					_flag = 1;
					if ((0x00007FFF & node->Rece_Node_Info[m][1]) == k && k != 0) { //�ظ��յ�����������
						//cout << node->Id << "�յ���ͨͨ�Ű�,�ظ��� == 249" << endl;
						if (node->Num == 1)//�Լ�����ͷ��㣩���ȵ÷����������յ��ظ���
						{
							node->Node_Stable[meg[0][0]] = 1;
						}
						else if (node->Send_Mes_Count != 0) {
							node->Node_Stable[meg[0][0]] = 1;
						}
						++node->Rece_Mes_Count;
						node->Tag = 0;
					}
					else  //�����ظ���������
					{
						//��ԭ�������ݲ��� 1 �ĸ���
						if ((node->Rece_Node_Info[m][1] >> 30) != 1) {
							//cout << node->Id << "�յ���ͨͨ�Ű�,�����ظ�������ԭ�������ݲ��� 1 �ĸ��� == 253" << endl;
							node->Rece_Node_Info[m][1] = meg[0][1];
							++node->Rece_Mes_Count;
							++node->Rece_Mes_Count;
							node->Tag = 0;
						}
					}
				}
			}
			if (_flag != 1) {//δ����ѭ������ʾ�ð�Ϊ��һ���յ���ֱ��д��
				//cout << node->Id << "��һ���յ���ͨͨ�Ű�,�����ظ��� == 261" << endl;
				node->Rece_Node_Info[node->Node_List_Length][0] = meg[0][0];
				node->Rece_Node_Info[node->Node_List_Length][1] = meg[0][1];
				++node->Node_List_Length;//���� +1
				++node->Remain_Zero_Count;
				++node->Rece_Mes_Count;
				//node->Val = 1;//��ʾ��һ���յ���ͨͨ�Ű������б仯
				node->Flag = true;
				node->Tag = 0;
			}
			else
			{
				_flag = 0;
			}
		}

		//�յ�ͳ�� 1 �İ�
		else if (i == 1)
		{
			node->Tag = 3;
			for (int m = 0; m < node->Node_List_Length; ++m) {
				if (meg[0][0] == node->Rece_Node_Info[m][0]) { //�ҵ���id
					if ((0xC0000000 & node->Rece_Node_Info[m][1]) >> 30 == 1) { //�ظ��յ�����������
						//cout << node->Id << "�ظ��յ�ͳ�� 1 �İ�,���� == 282" << endl;
						++node->Rece_Mes_Count;
						break;
					}
					else  //��һ���յ�ͳ�� 1 �İ�������
					{
						//cout << node->Id << "��һ���յ�ͳ�� 1 �İ�,�����ظ��� == 279" << endl;
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
			//����û��ȷ��λ�õ���Ϣ��δͬ�� 1 �İ�����ͬ�����
			if (((node->Rece_Node_Info[m][1] & 0X3FFF8000) >> 15) == 0) {
				if (((0xC0000000 & node->Rece_Node_Info[m][1]) >> 30) != 1) {
					flag = 1; //��ʾ����δȷ��λ�õĻ�δ�յ�ͬ�� 1 �İ�
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








