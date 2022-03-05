#pragma once
#include "node.h"

/*
	函数功能：发送 0 通信包
	参数：Node * node 发送方节点指针
*/
void head_Send_Commun_Zero(Node * node);

/*
	函数功能：发送 0 通信包
	参数：Node * node 发送方节点指针
*/
void send_Commun_Zero(Node * node);

/*
	函数功能：发送 1 通信包
	参数：Node * node  发送方节点指针
*/
void send_Commun_One(Node * node);

/*
	函数功能：发送更新包
	参数：Node * node  发送方节点指针
*/
void send_Update(Node * node);

/*
	函数功能：接收包
	参数：Node * node  接收方节点指针
		 int meg[1][2] 消息数据包
*/
void received_Meg(Node * node,int meg[1][2]);

/*
	函数功能：测试未确定位置的自己的 1 包是否同步完成
	参数：Node * node  自己节点指针
	返回 1 同步完成,可以直接进行计算
	返回 0 同步未完成
		 
*/
int one_Stable(Node * node);

void testFunc();
