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

int f(int a[], int n, int P)	//����һ��/��/б�ߵľ�̬��ֵ��n>=6Ϊ����/��/б�߳��ȣ�PΪ�������ࣩ
{
	int res = 0;
	for (int i = 0; i <= n - 7; ++i)
		if (a[i] == -P && a[i + 1] == 0 && a[i + 2] == P && a[i + 3] == P && a[i + 4] == P && a[i + 5] == 0 && a[i + 6] == -P)	//����
			res += 10;

	for (int i = 0; i <= n - 6; ++i)
		if (a[i] == -P) {
			if (a[i + 1] == P) {
				if (a[i + 2] == P) {
					if (a[i + 3] == P) {
						if (a[i + 4] == P) {
							if (a[i + 5] == 0)	//����
								res += 100;
						}
						else if (a[i + 4] == 0) {
							if (a[i + 5] == 0)	//����
								res += 10;
						}
					}
					else if (a[i + 3] == 0) {
						if (a[i + 4] == P) {
							if (a[i + 5] == 0)	//����
								res += 10;
						}
						else if (a[i + 4] == 0) {
							if (a[i + 5] == 0)	//�߶�
								res += 1;
						}
					}
				}
				else if (a[i + 2] == 0) {
					if (a[i + 3] == P) {
						if (a[i + 4] == P) {
							if (a[i + 5] == 0)	//����
								res += 10;
						}
						else if (a[i + 4] == 0) {
							if (a[i + 5] == 0)	//�߶�
								res += 1;
						}
					}
					else if (a[i + 3] == 0) {
						if (a[i + 4] == P) {
							if (a[i + 5] == 0)	//�߶�
								res += 1;
						}
					}
				}
			}
		}
		else if (a[i] == 0) {
			if (a[i + 1] == P) {
				if (a[i + 2] == P) {
					if (a[i + 3] == P) {
						if (a[i + 4] == P) {
							if (a[i + 5] == -P)	//����
								res += 100;
							else if (a[i + 5] == 0)	//����
								res += 1000;
						}
						else if (a[i + 4] == 0) {
							if (a[i + 5] == 0)	//����
								res += 100;
						}
					}
					else if (a[i + 3] == 0) {
						if (a[i + 4] == P) {
							if (a[i + 5] == -P)	//����
								res += 10;
							else if (a[i + 5] == 0)	//����
								res += 100;
						}
					}
				}
				else if (a[i + 2] == 0) {
					if (a[i + 3] == P) {
						if (a[i + 4] == P) {
							if (a[i + 5] == -P)	//����
								res += 10;
							else if (a[i + 5] == 0)	//����
								res += 100;
						}
					}
					else if (a[i + 3] == 0) {
						if (a[i + 4] == P) {
							if (a[i + 5] == -P)	//�߶�
								res += 1;
							else if (a[i + 5] == 0)	//���
								res += 10;
						}
					}
				}
			}
			else if (a[i + 1] == 0) {
				if (a[i + 2] == P) {
					if (a[i + 3] == P) {
						if (a[i + 4] == P) {
							if (a[i + 5] == -P)	//����
								res += 10;
							else if (a[i + 5] == 0)	//����
								res += 100;
						}
					}
					else if (a[i + 3] == 0) {
						if (a[i + 4] == P) {
							if (a[i + 5] == -P)	//�߶�
								res += 1;
						}
					}
				}
				else if (a[i + 2] == 0) {
					if (a[i + 3] == P) {
						if (a[i + 4] == P) {
							if (a[i + 5] == -P)	//�߶�
								res += 1;
						}
					}
				}
			}
		}

	for (int i = 0; i <= n - 5; ++i)
		if (a[i] == P) {
			if (a[i + 1] == P) {
				if (a[i + 2] == P) {
					if (a[i + 3] == P) {
						if (a[i + 4] == P)	//����
							res += 10000;
					}
					else if (a[i + 3] == 0) {
						if (a[i + 4] == P)	//����
							res += 100;
					}
				}
				else if (a[i + 2] == 0) {
					if (a[i + 3] == P) {
						if (a[i + 4] == P)	//����
							res += 100;
					}
					else if (a[i + 3] == 0) {
						if (a[i + 4] == P)	//����
							res += 10;
					}
				}
			}
			else if (a[i + 1] == 0) {
				if (a[i + 2] == P) {
					if (a[i + 3] == P) {
						if (a[i + 4] == P)	//����
							res += 100;
					}
					else if (a[i + 3] == 0) {
						if (a[i + 4] == P)	//����
							res += 10;
					}
				}
				else if (a[i + 2] == 0) {
					if (a[i + 3] == P) {
						if (a[i + 4] == P)	//����
							res += 10;
					}
					else if (a[i + 3] == 0) {
						if (a[i + 4] == P)	//�߶�
							res += 1;
					}
				}
			}
		}
		else if (a[i] == 0) {
			if (a[i + 1] == P) {
				if (a[i + 2] == P) {
					if (a[i + 3] == 0) {
						if (a[i + 4] == 0)	//���
							res += 10;
					}
				}
				else if (a[i + 2] == 0) {
					if (a[i + 3] == P) {
						if (a[i + 4] == 0)	//���
							res += 10;
					}
				}
			}
			else if (a[i + 1] == 0) {
				if (a[i + 2] == P) {
					if (a[i + 3] == P) {
						if (a[i + 4] == 0)	//���
							res += 10;
					}
				}
			}
		}

	return res;
}

int state::F()
{
	int res = 0;
	for (int i = 0; i < 15; ++i) {	//"-"����
		int a[17];
		for (int j = 0; j < 15; ++j)
			a[j + 1] = CB[i][j];
		a[0] = a[16] = -1;	//���߽統���������Ӵ���
		res += f(a, 17, 1);
		a[0] = a[16] = 1;
		res -= f(a, 17, -1);
	}

	for (int j = 0; j < 15; ++j) {	//"|"����
		int a[17];
		for (int i = 0; i < 15; ++i)
			a[i + 1] = CB[i][j];
		a[0] = a[16] = -1;	//���߽統���������Ӵ���
		res += f(a, 17, 1);
		a[0] = a[16] = 1;
		res -= f(a, 17, -1);
	}

	for (int i = 4; i < 15; ++i) {	//"/"����
		int a[17];
		for (int j = 0; j <= i; ++j)
			a[j + 1] = CB[i - j][j];
		a[0] = a[i + 2] = -1;	//���߽統���������Ӵ���
		res += f(a, i + 3, 1);
		a[0] = a[i + 2] = 1;
		res -= f(a, i + 3, -1);

		if (i == 14)
			break;
		for (int j = 0; j <= i; ++j)
			a[j + 1] = CB[14 - j][14 - i + j];
		a[0] = a[i + 2] = -1;	//���߽統���������Ӵ���
		res += f(a, i + 3, 1);
		a[0] = a[i + 2] = 1;
		res -= f(a, i + 3, -1);
	}

	for (int i = 0; i < 11; ++i) {	//"\"����
		int a[17];
		for (int j = 0; j <= 14 - i; ++j)
			a[j + 1] = CB[i + j][j];
		a[0] = a[16 - i] = -1;	//���߽統���������Ӵ���
		res += f(a, 17 - i, 1);
		a[0] = a[16 - i] = 1;
		res -= f(a, 17 - i, -1);

		if (i == 0)
			continue;
		for (int j = 0; j <= 14 - i; ++j)
			a[j + 1] = CB[j][i + j];
		a[0] = a[16 - i] = -1;	//���߽統���������Ӵ���
		res += f(a, 17 - i, 1);
		a[0] = a[16 - i] = 1;
		res -= f(a, 17 - i, -1);
	}

	return res;
}

int state::GoalTest()
{
	int judge = 0;
	for (int i = 0; i < 15; ++i)	//"-"����
		for (int j = 0; j < 11; ++j) {
			judge = 0;
			for (int k = 0; k < 5; ++k)
				judge += this->CB[i][j + k];
			if (judge == 5)
				return 1;
			else if (judge == -5)
				return -1;
		}

	for (int i = 0; i < 11; ++i)	//"|"����
		for (int j = 0; j < 15; ++j) {
			judge = 0;
			for (int k = 0; k < 5; ++k)
				judge += this->CB[i + k][j];
			if (judge == 5)
				return 1;
			else if (judge == -5)
				return -1;
		}

	for (int i = 4; i < 15; ++i)	//"/"����
		for (int j = 0; j < 11; ++j) {
			judge = 0;
			for (int k = 0; k < 5; ++k)
				judge += this->CB[i - k][j + k];
			if (judge == 5)
				return 1;
			else if (judge == -5)
				return -1;
		}

	for (int i = 0; i < 11; ++i)	//"\"����
		for (int j = 0; j < 11; ++j) {
			judge = 0;
			for (int k = 0; k < 5; ++k)
				judge += this->CB[i + k][j + k];
			if (judge == 5)
				return 1;
			else if (judge == -5)
				return -1;
		}

	return 0;
}

bool GT(state* a, state* b)	//����compare����������
{
	return a->eva > b->eva;
}

bool LT(state* a, state* b)	//����compare������С��
{
	return a->eva < b->eva;
}

state* state::minimax(int depth)	//����һ�Σ��ݹ�����
{
	/* AI��MAX������ */
	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 15; ++j)
			if (this->CB[i][j] == 0) {	//��ǰ��λ��Ϊ�գ���������
				int t = 0;	//tΪ(i, j)��Χ��������
				for (int m = ((i - 1 > 0) ? i - 1 : 0); m <= ((i + 1 < 14) ? i + 1 : 14); ++m)
					for (int n = ((j - 1 > 0) ? j - 1 : 0); n <= ((j + 1 < 14) ? j + 1 : 14); ++n)
						if (this->CB[m][n] != 0)
							++t;
				if (t == 0)	//��(i, j)��Χ3*3�ķ�Χ�ڶ�û�����ӣ���ֱ������
					continue;
				/* һ������²�����Զ���Ѵ��ڵ����Ӵ����壬���������Ѵ��ڵ�������Χ3*3�ķ�Χ���Դ������ӽ��������������ǰ������ */
				state* p = new state;
				for (int i = 0; i < 15; ++i)
					for (int j = 0; j < 15; ++j)
						p->CB[i][j] = this->CB[i][j];
				p->CB[i][j] = 1;
				p->Last_i = i;
				p->Last_j = j;

				if (p->GoalTest() == 1)	//���Ѿ�ʤ����������������
					return p;

				p->eva = p->F();	//������ӽ�����ۺ�����ֵ
				p->father = this;
				this->child.push_back(p);
			}

	/* ���֣�MIN������ */
	sort(this->child.begin(), this->child.end(), GT);	//�������ӽ������������Ż���֦
	int temp = -1;
	for (unsigned int k = 0; k < this->child.size(); ++k) {	//���������ӽ��
		bool judge = false;
		for (int i = 0; i < 15; ++i) {
			for (int j = 0; j < 15; ++j) {
				if (this->child[k]->CB[i][j] == 0) {	//��ǰ��λ��Ϊ�գ���������
					int t = 0;	//tΪ(i, j)��Χ��������
					for (int m = ((i - 1 > 0) ? i - 1 : 0); m <= ((i + 1 < 14) ? i + 1 : 14); ++m)
						for (int n = ((j - 1 > 0) ? j - 1 : 0); n <= ((j + 1 < 14) ? j + 1 : 14); ++n)
							if (this->child[k]->CB[m][n] != 0)
								++t;
					if (t == 0)	//��(i, j)��Χ3*3�ķ�Χ�ڶ�û�����ӣ���ֱ�Ӽ�֦
						continue;
					state* p = new state;
					for (int i = 0; i < 15; ++i)
						for (int j = 0; j < 15; ++j)
							p->CB[i][j] = this->child[k]->CB[i][j];
					p->CB[i][j] = -1;

					if (p->GoalTest() == -1 && k + 1 != this->child.size()) {	//���Ѿ�ʧ���Ҳ������һ���ӽ�㣬��ֱ������
						judge = true;
						break;
					}

					p->Last_i = i;
					p->Last_j = j;
					p->father = this->child[k];
					if (depth > 2) {
						state* NewState = p->minimax(depth - 2);	//�ݹ����
						p->eva = p->beta;
					}
					else
						p->eva = p->F();	//������ӽ�����ۺ�����ֵ
					if (p->eva < this->child[k]->beta) {
						this->child[k]->beta = p->eva;
						if (this->child[k]->beta < this->alpha) {
							judge = true;
							break;
						}
					}
				}
			}
			if (judge)
				break;
		}
		if (judge)
			continue;
		this->child[k]->alpha = this->child[k]->beta;
		if (this->child[k]->alpha > this->alpha) {
			this->alpha = this->child[k]->alpha;
			temp = k;
		}
	}
	this->beta = this->alpha;

	return this->child[temp];
}

void state::clear()
{
	this->alpha = INT_MIN;	//�����һ����������ʱ����
	this->beta = INT_MAX;
	for (unsigned int k = 0; k < this->child.size(); ++k)	//�ͷ��ڴ�
		delete this->child[k];
	this->child.clear();	//������ӽ�㣨���д�����һ��MIN�������ӽ�㣩
}