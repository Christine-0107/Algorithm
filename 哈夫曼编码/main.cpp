#include <iostream>
#include <iomanip>
using namespace std;

class HNode
{
	//friend class HTree;
public:
	int left = -1, right = -1;
	int parent = -1;
	int weight = 1000;
	int length = -1;

public:
	HNode() {}
	HNode(int w);//初始创造结点时的构造函数

};
//HNode的函数
HNode::HNode(int w)
{
	weight = w;
}

void searchMin(int& min1, int& min2, int n, HNode** hNode)
{
	for (int i = 0; i < 2 * n - 1; i++)
	{
		if (hNode[i]->parent==-1)
		{
			min1 = i;
			break;
		}
	}
	for (int i = 0; i < 2 * n - 1; i++)
	{
		if (hNode[i]->parent == -1 && hNode[i]->weight < hNode[min1]->weight)
		{
			min1 = i;
		}
	}

	for (int i = 0; i < 2 * n - 1; i++)
	{
		if (hNode[i]->parent == -1 && i != min1)
		{
			min2 = i;
			break;
		}
	}
	for (int i = 0; i < 2 * n - 1; i++)
	{
		if (hNode[i]->parent == -1 && hNode[i]->weight < hNode[min2]->weight && i != min1)
		{
			min2 = i;
		}
	}
	
}

void createHTree(int n, HNode** hNode)
{
	int minPos1, minPos2;

	for (int i = 0; i < n - 1; i++)//需要进行n-1次合并操作
	{
		searchMin(minPos1, minPos2, n, hNode);
		hNode[i + n]->weight = hNode[minPos1]->weight + hNode[minPos2]->weight;
		hNode[i + n]->left = minPos1;
		hNode[i + n]->right = minPos2;
		hNode[minPos1]->parent = i + n;
		hNode[minPos2]->parent = i + n;
	}
}



void getLength(int n, HNode** hNode) {
	int count;
	for (int i = 0; i < 2 * n - 1; i++) {
		HNode* p = hNode[i];
		count = 0;
		while (p->parent != -1) {
			count++;
			p = hNode[p->parent];
			
		}
		hNode[i]->length = count;
	}
}



int main()
{
	int N;
	int weight;
	cin >> N; //N表示字符种类

	HNode** hNode = new HNode*[2 * N - 1];
	//hNode[i]表示每个字符
	int sumWeight = 0;
	
	for (int i = 0; i < N; i++) {
		cin >> weight;
		hNode[i] = new HNode(weight);
		sumWeight += weight;
	}
	for (int i = N; i < 2 * N - 1; i++) {
		hNode[i] = new HNode(sumWeight+1);
	}
	createHTree(N, hNode);
	getLength(N, hNode);
	int sum = 0;
	for (int i = 0; i < N; i++) {
		sum += hNode[i]->weight  * hNode[i]->length;
	}
	
	float avg = sum * (1.0) / sumWeight;
	cout << setiosflags(ios::fixed) << setprecision(2);
	cout << avg;
	return 0;


}


