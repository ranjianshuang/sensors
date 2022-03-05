// AutoSort1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
			cout << "Δ = " ;
			cin >> COMMU_DIS;
			if (NodeCountNum > (2 * COMMU_DIS + 1)) {
				COMMU_DIS = COMMU_DIS * 5;
				break;
			}
			else {
				cout << "N < 2Δ + 1,Please re-enter and ensure N ≥ 2 Δ+ 1 .\n" << endl;
			}
		}
		Node * node = CreatNode(NodeCountNum);
		int stable[MAX_MES_COUNT];//通信同步稳定数组
		for (int i = 0; i < MAX_MES_COUNT; i++) {
			stable[i] = 0;
		}
		//初始化随机数组
		//下标0 ~ NodeCountNum-1
		//值  1 ~ NodeCountNum
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
		//发通信包
		while (flag) {
			tag = 0;
			randArray(&a[0]);
			for (int j = 0; j < NodeCountNum; ++j) {
				tag2 = 0;
				if (a[j] == 1) continue;
				send_Commun_Zero(&node[a[j] - 1]);//下标比值小 1
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
			//通信循环终止条件
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
				//判断是否需要计算自己的位置
				if (node[a[m] - 1].Tag == 1 || node[a[m] - 1].Tag == 2) {
					//cout << node->Id << "进入计算 Tag= " << node->Tag << endl;
					if (Calc_Position(&node[a[m] - 1]) == 4)
					{
						node[a[m] - 1].Tag = 0;
						//cout << "============================================ " << node[a[m] - 1].Id << " 确定自己的位置，并退出\n";
						//Print_Info(&node[a[m] - 1]);
						//发包 1 次
						send_Update(&node[a[m] - 1]);
					}
				}
				//发统计1个数的包
				else if (node[a[m] - 1].Tag == 3) {
					//cout << node[a[m] - 1].Id << "同步 1 个数，发包" << endl;
					send_Commun_One(&node[a[m] - 1]);
					if (one_Stable(&node[a[m] - 1]) == 0) continue;

					if (Calc_Position(&node[a[m] - 1]) == 4)
					{
						node[a[m] - 1].Tag = 0;
						//cout << "============================================ " << node->Id << " 确定自己的位置，并退出\n";
						//Print_Info(node);
						//发包 1 次
						send_Update(&node[a[m] - 1]);
					}
				}
			}
			//循环终止条件,所有节点拍好位置
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
	//头结点延迟发送
	if (node->Num == 1) {

		//等待其他节点稳定
		Node *temp = node;
		++temp;
		int _i = 2;
		while (_i <= NodeCountNum) {
			if (temp->Val != 100) {
				cout << "1 长度为 0 进入等待 ==75 " << endl;
				Sleep(3000);
				continue;
			}
			else {
				++temp;
				++_i;
				cout << "_i=" << _i << endl;
			}
		}

		cout << "==============id：" << node->Id << "的 0包稳定，开始发包==========" << endl;
		//Print_Info(node);

		for (int i = 0; i < NodeCountNum; ++i) {
			Print_Rece_Node_Info(&node[i]);
		}
		//getchar();
		//发包 3 次
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
		cout << "========================id：" << node->Id << " 节点停止发送包================" << endl;
		Print_Info(node);
	}

	//普通节点
	else {
		
		
	}
}