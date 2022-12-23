#include <iostream>
using namespace std;
int M, N, k;
int** a, ** b, * num_0, * odd, * even, * com;
int MaxCount = 0, num = 0;

void GetData()//依据输入确定M和N的大小，并读入数据
{
	cout << "请依次输入行数N、列数M、激光照射次数k(3≤N≤100，3≤M≤20，1<=k<=M):";
	cin >> N >> M >> k;
	a = new int* [N];
	b = new int* [N];
	for (int i = 0; i < N; i++)
	{
		a[i] = new int[M];
		b[i] = new int[M];
	}

	cout << "请输入数据阵列：" << endl;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			cin >> a[i][j];
			b[i][j] = a[i][j];
		}
}

void Trans(int** arr, int m)//列反转
{
	int n = N;
	while (n--)
		arr[n][m] = abs(arr[n][m] - 1);
}

int Judge(int** arr)//计算导通行数
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

int partition(int a[], int low, int high)//划分操作，分为两个子表
{
	int pivot = a[low];//一般将第一个元素设为枢轴
	while (low < high)
	{
		while (low < high && a[high]>=pivot)
			high--;
		a[low] = a[high];
		while (low < high && a[low]<=pivot)
			low++;
		a[high] = a[low];
	}
	a[low] = pivot; //枢轴元素存到最终位置
	return low; //返回存放枢轴的位置
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

int turnToTen(int** a,int i)//i表示行数
{
	int tag=0;
	for (int j = 0; j < M; j++)
	{
		tag += a[i][j] * pow(10, M - 1 - j);
	}
	return tag;
}

void Count(int** arr)//计数每行0的个数存在num_0里,0的个数为偶数且小于k的存于even，类似的奇数存于odd里
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
		cout << "最大导通行数为：0" << endl;
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
	cout << "最大导通行数为：" << max << endl;
}

int main()
{
	GetData();
	Count(a);
	GetResult();
	
}
