#pragma once
#include "node.h"

/*
	�������ܣ����� 0 ͨ�Ű�
	������Node * node ���ͷ��ڵ�ָ��
*/
void head_Send_Commun_Zero(Node * node);

/*
	�������ܣ����� 0 ͨ�Ű�
	������Node * node ���ͷ��ڵ�ָ��
*/
void send_Commun_Zero(Node * node);

/*
	�������ܣ����� 1 ͨ�Ű�
	������Node * node  ���ͷ��ڵ�ָ��
*/
void send_Commun_One(Node * node);

/*
	�������ܣ����͸��°�
	������Node * node  ���ͷ��ڵ�ָ��
*/
void send_Update(Node * node);

/*
	�������ܣ����հ�
	������Node * node  ���շ��ڵ�ָ��
		 int meg[1][2] ��Ϣ���ݰ�
*/
void received_Meg(Node * node,int meg[1][2]);

/*
	�������ܣ�����δȷ��λ�õ��Լ��� 1 ���Ƿ�ͬ�����
	������Node * node  �Լ��ڵ�ָ��
	���� 1 ͬ�����,����ֱ�ӽ��м���
	���� 0 ͬ��δ���
		 
*/
int one_Stable(Node * node);

void testFunc();
