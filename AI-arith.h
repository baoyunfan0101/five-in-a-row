#include "resource.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <windows.h>
using namespace std;

typedef class state {	//����һ��״̬
public:
	/* ��״̬�Ļ�����Ϣ */
	int CB[15][15] = { 0 };	//ChessBoard��AI��MAX��1�����֣�MIN��-1����0
	int Last_i = -1;	//��һ��������
	int Last_j = -1;	//��һ��������
	int eva = INT_MIN;	//���ڴ洢�����ۺ�����ֵ
	/* ����CB[Last_i][Last_j]�����ж���һ�����巽�Ǻ���/���壬��MAX/MIN */

	/* �������Ľ��� */
	state* father;	//�����
	vector<state*> child;	//�ӽ�㣨�������洢��

	/* ��-�¼�֦��� */
	int alpha = INT_MIN;
	int beta = INT_MAX;

	/* state��ĳ�Ա���� */
	int F();	//���ۺ���
	int GoalTest();	//Ŀ����Ժ�����AI��MAX��ʤ����1�����֣�MIN��ʤ����-1�����෵��0
	state* minimax(int depth);	//��С����ֵ������������һ��������״̬

	void clear();	//���³�ʼ������һ����������ʱ����
}state;