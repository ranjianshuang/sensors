// AutoSort1.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//
//#include "strCut.h"
#include "node.h"
#include "communication.h"
#include "windows.h"
#include "randArr.h"


void Node_Operation(Node * node);


int main()
{
	extern int NodeCountNum;
	extern int COMMU_DIS;
	int a[MAX_MES_COUNT];
	double sum_send = 0, sum_rece = 0;
	while (1) {
		while (1) {
			cout << "Please enter the number of nodes, which must be an integer and less than 1024 :" << endl;
			cout << "N = " ;
			cin >> NodeCountNum;
			cout << "Please enter the transmission distance: " << endl; 
			cout << "�� = " ;
			cin >> COMMU_DIS;
			if (NodeCountNum > (2 * COMMU_DIS + 1)) {
				COMMU_DIS = COMMU_DIS * 5;
				break;
			}
			else {
				cout << "N < 2�� + 1,Please re-enter and ensure N �� 2 ��+ 1 .\n" << endl;
			}
		}
		Node * node = CreatNode(NodeCountNum);
		int stable[MAX_MES_COUNT];//ͨ��ͬ���ȶ�����
		for (int i = 0; i < MAX_MES_COUNT; i++) {
			stable[i] = 0;
		}
		//��ʼ���������
		//�±�0 ~ NodeCountNum-1
		//ֵ  1 ~ NodeCountNum
		for (int i = 0; i < NodeCountNum; i++) {
			a[i] = i + 1;
		}
		cout << "\nInitialize information for all nodes\n" << endl;
		
		cout << "===================================================================================" << endl;
		for (int i = 0; i < NodeCountNum; ++i) {
			Print_Info(&node[i]);
		}

		bool flag = true;
		int tag = 0, tag2 = 0;
		//��ͨ�Ű�
		while (flag) {
			tag = 0;
			randArray(&a[0]);
			for (int j = 0; j < NodeCountNum; ++j) {
				tag2 = 0;
				if (a[j] == 1) continue;
				send_Commun_Zero(&node[a[j] - 1]);//�±��ֵС 1
				if (node[a[j] - 1].Node_List_Length != 0) {
					for (int i = 0; i < node[a[j] - 1].Node_List_Length; ++i) {
						if (node[a[j] - 1].Node_Stable[node[a[j] - 1].Rece_Node_Info[0][0]] == 0) {
							tag2 = 1;
							break;
						}
					}
					if (tag2 == 0) {
						stable[a[j] - 1] = 1;
					}
				}
			}
			//ͨ��ѭ����ֹ����
			for (int i = 2; i < NodeCountNum; i++) {
				if (stable[i] == 0) {
					tag = 1;
					break;
				}
			}
			if (tag == 0) {
				flag = false;
			}
		}
		cout << "\nThe relative locations of all nodes are calculated by the algorithm\n" << endl;
		cout << "===================================================================================" << endl;

		/*for (int i = 1; i < NodeCountNum; ++i) {
			Print_Rece_Node_Info(&node[i]);
		}*/

		head_Send_Commun_Zero(&node[0]);

		flag = true;
		tag = 0;
		while (flag) {
			randArray(&a[0]);
			for (int m = 0; m < NodeCountNum; ++m) {
				if (node[a[m] - 1].Num != 0) continue;
				//�ж��Ƿ���Ҫ�����Լ���λ��
				if (node[a[m] - 1].Tag == 1 || node[a[m] - 1].Tag == 2) {
					//cout << node->Id << "������� Tag= " << node->Tag << endl;
					if (Calc_Position(&node[a[m] - 1]) == 4)
					{
						node[a[m] - 1].Tag = 0;
						//cout << "============================================ " << node[a[m] - 1].Id << " ȷ���Լ���λ�ã����˳�\n";
						//Print_Info(&node[a[m] - 1]);
						//���� 1 ��
						send_Update(&node[a[m] - 1]);
					}
				}
				//��ͳ��1�����İ�
				else if (node[a[m] - 1].Tag == 3) {
					//cout << node[a[m] - 1].Id << "ͬ�� 1 ����������" << endl;
					send_Commun_One(&node[a[m] - 1]);
					if (one_Stable(&node[a[m] - 1]) == 0) continue;

					if (Calc_Position(&node[a[m] - 1]) == 4)
					{
						node[a[m] - 1].Tag = 0;
						//cout << "============================================ " << node->Id << " ȷ���Լ���λ�ã����˳�\n";
						//Print_Info(node);
						//���� 1 ��
						send_Update(&node[a[m] - 1]);
					}
				}
			}
			//ѭ����ֹ����,���нڵ��ĺ�λ��
			for (; tag < NodeCountNum; tag++) {
				if (node[tag].Num == 0) {
					break;
				}
			}
			if (tag == NodeCountNum) {
				flag = false;
			}
		}
		
		for (int i = 0; i < NodeCountNum; ++i) {
			sum_rece += node[i].Rece_Mes_Count;
			sum_send += node[i].Send_Mes_Count;
			Print_Info(&node[i]);
		}

		cout << "===================================================================================" << endl;
		cout << "Average number of packets sent by nodes = " << (double)(sum_send / NodeCountNum) << endl;
		cout << "Average number of packets received by nodes = " << (double)(sum_rece / NodeCountNum) <<"\n\n"<< endl;
	}
}


void Node_Operation(Node * node)
{
	extern int NodeCountNum;
	//ͷ����ӳٷ���
	if (node->Num == 1) {

		//�ȴ������ڵ��ȶ�
		Node *temp = node;
		++temp;
		int _i = 2;
		while (_i <= NodeCountNum) {
			if (temp->Val != 100) {
				cout << "1 ����Ϊ 0 ����ȴ� ==75 " << endl;
				Sleep(3000);
				continue;
			}
			else {
				++temp;
				++_i;
				cout << "_i=" << _i << endl;
			}
		}

		cout << "==============id��" << node->Id << "�� 0���ȶ�����ʼ����==========" << endl;
		//Print_Info(node);

		for (int i = 0; i < NodeCountNum; ++i) {
			Print_Rece_Node_Info(&node[i]);
		}
		//getchar();
		//���� 3 ��
		for (int j = 0; j < 3; ++j) {
			for (int i = 0; i < node->Clock; ++i)
				Sleep(200);
			head_Send_Commun_Zero(node);
		}
		/*int j = 1;
		while (true) {
			_sleep(1000 * j);
			++j;
			head_Send_Commun_Zero(node);
			if (Stop_Send(node) == 6) break;
		}*/
		cout << "========================id��" << node->Id << " �ڵ�ֹͣ���Ͱ�================" << endl;
		Print_Info(node);
	}

	//��ͨ�ڵ�
	else {
		
		
	}
}