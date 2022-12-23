#include <iostream>
using namespace std;
int M, N, k;
int** a, ** b, * num_0, * odd, * even, * com;
int MaxCount = 0, num = 0;

void GetData()//��������ȷ��M��N�Ĵ�С������������
{
	cout << "��������������N������M�������������k(3��N��100��3��M��20��1<=k<=M):";
	cin >> N >> M >> k;
	a = new int* [N];
	b = new int* [N];
	for (int i = 0; i < N; i++)
	{
		a[i] = new int[M];
		b[i] = new int[M];
	}

	cout << "�������������У�" << endl;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			cin >> a[i][j];
			b[i][j] = a[i][j];
		}
}

void Trans(int** arr, int m)//�з�ת
{
	int n = N;
	while (n--)
		arr[n][m] = abs(arr[n][m] - 1);
}

int Judge(int** arr)//���㵼ͨ����
{
	int count = 0;
	for (int i = 0; i < N; i++)
	{
		bool L = true;
		for (int j = 0; j < M; j++)
		{
			if (arr[i][j] == 0)
			{
				L = false;
				break;
			}
		}
		if (L)
			count++;
	}
	return count;
}

int partition(int a[], int low, int high)//���ֲ�������Ϊ�����ӱ�
{
	int pivot = a[low];//һ�㽫��һ��Ԫ����Ϊ����
	while (low < high)
	{
		while (low < high && a[high]>=pivot)
			high--;
		a[low] = a[high];
		while (low < high && a[low]<=pivot)
			low++;
		a[high] = a[low];
	}
	a[low] = pivot; //����Ԫ�ش浽����λ��
	return low; //���ش�������λ��
}
void quickSort(int a[], int low, int high)
{
	if (low < high)
	{
		int pivotPos = partition(a, low, high);
		quickSort(a, low, pivotPos - 1);
		quickSort(a, pivotPos + 1, high);
	}

}

int turnToTen(int** a,int i)//i��ʾ����
{
	int tag=0;
	for (int j = 0; j < M; j++)
	{
		tag += a[i][j] * pow(10, M - 1 - j);
	}
	return tag;
}

void Count(int** arr)//����ÿ��0�ĸ�������num_0��,0�ĸ���Ϊż����С��k�Ĵ���even�����Ƶ���������odd��
{
	num_0 = new int[N];
	odd = new int[N];
	even = new int[N];
	com = new int[N];

	int count0 = 0, count1 = 0;

	for (int i = 0; i < N; i++)
	{
		int temp = 0;
		for (int j = 0; j < M; j++)
			if (arr[i][j] == 0)
				temp++;
		num_0[i] = temp;
	}

	for (int i = 0; i < N; i++)
	{
		if (num_0[i] <= k && num_0[i] % 2 == 0)
		{
			even[count0++]=turnToTen(arr, i);
		}
		if (num_0[i] <= k && num_0[i] % 2 == 1)
		{
			odd[count1++] =turnToTen(arr, i);
		}
	}
	quickSort(even, 0, count0 - 1);
	quickSort(odd, 0, count1 - 1);
	if (k % 2 == 0)
	{
		for (int i = 0; i < count0; i++)
			com[i] = even[i];
		num = count0;
	}
	if (k % 2 == 1)
	{
		for (int i = 0; i < count1; i++)
			com[i] = odd[i];
		num = count1;
	}
	
}

void GetResult()
{
	if (num == 0)
	{
		cout << "���ͨ����Ϊ��0" << endl;
		return;
	}
	int max = 1;
	int tag = com[0];
	int count = 1;
	for (int i = 1; i < num; i++)
	{
		if (com[i] == tag)
		{
			count++;
		}
		else
		{
			tag = com[i];
			
			if (count > max)
			{
				max = count;
			}
			count = 1;
		}
	}
	if (count > max)
	{
		max = count;
	}
	cout << "���ͨ����Ϊ��" << max << endl;
}

int main()
{
	GetData();
	Count(a);
	GetResult();
	
}
