#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <time.h>

using namespace std;

int imove[8] = { -1,0,1,1,1,0,-1,-1 }, jmove[8] = { 1,1,1,0,-1,-1,-1,0 };//ǰ����������

bool isFull(int **count, int n, int m);//����ש�Ƿ���ȫ������

void random_walk(int n, int m, int i, int j);//���ģ������n:������������ͼ�߶ȣ�m:������������ͼ��ȣ�i��j:��ʼ����

int main()
{
	cout << "(1)��СΪ15*15����ʼ����Ϊ(9,9)ʱ��" << endl;
	random_walk(15, 15, 9, 9);
	cout << "(2)��СΪ39*19����ʼ����Ϊ(0,0)ʱ��" << endl;
	random_walk(39, 19, 0, 0);
	system("pause");
}
bool isFull(int **count, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			if (count[i][j] == 0)//����û�߹��򷵻ؼ�
				return false;
	}
	return true;
}

void random_walk(int n, int m, int i, int j)
{
	int tm = (int)time(NULL);
	srand(tm);//���������
	int **count = new int*[n];//��������
	for (int i = 0; i < n; ++i)
	{
		count[i] = new int[m];
	}
	for (int i = 0; i < n; ++i)//��ʼ��
	{
		for (int j = 0; j < m; ++j)
		{
			count[i][j] = 0;
		}
	}

	int pace = 0;//���߲���
	int random = 0;//�������
	while (pace < 50000)//50000��������Ч
	{
		count[i][j]++;
		//cout << count[i][j] << "  " << i << "   " << j << "   " << pace << endl;
		random = rand() % 8;
		int g = i + imove[random], h = j + jmove[random];
		while (g == -1 || g == n || h == -1 || h == m)//��Խ��ʱ����ѡ����
		{
			random = rand() % 8;
			g = i + imove[random]; h = j + jmove[random];
		}
		i = g; j = h;
		pace++;
		if (isFull(count, n, m))//ȫ������ʱ�˳�ѭ��
			break;
		else
			continue;
	}
	cout << "�Ϸ��ܲ���Ϊ��" << pace << endl;
	cout << "����countΪ��" << endl;
	for (int i = 0; i < n; ++i)//���count����
	{
		for (int j = 0; j < m; ++j)
		{
			cout << setw(4) << count[i][j];
		}
		cout << endl;
	}
}