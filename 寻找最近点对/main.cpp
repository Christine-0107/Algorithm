#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
//节点类
class Node 
{
public:
	double x;
	double y;
};

//归并排序
void sort_merge(Node* nodes, int low, int mid, int high, int n, int tag) {
	Node* temp = new Node[n];
	for (int k = low; k <= high; k++) {
		temp[k] = nodes[k];
	}
	
	if (tag == 0) { //按照x排序
		int i, j, k;
		for (i = low, j = mid + 1, k = i; i <= mid && j <= high; k++) {
			if (temp[i].x < temp[j].x) {
				nodes[k] = temp[i];
				i++;
			}
			else if (temp[i].x == temp[j].x) {
				if (temp[i].y <= temp[j].y) {
					nodes[k] = temp[i];
					i++;
				}
				else {
					nodes[k] = temp[j];
					j++;
				}
			}
			else {
				nodes[k] = temp[j];
				j++;
			}
		}
		while (i <= mid) {
			nodes[k] = temp[i];
			k++;
			i++;
		}
		while (j <= high) {
			nodes[k] = temp[j];
			k++;
			j++;
		}
	}
	else { //按照y排序
		int i, j, k;
		for (i = low, j = mid + 1, k = i; i <= mid && j <= high; k++) {
			if (temp[i].y < temp[j].y) {
				nodes[k] = temp[i];
				i++;
			}
			else if (temp[i].y == temp[j].y) {
				if (temp[i].x <= temp[j].x) {
					nodes[k] = temp[i];
					i++;
				}
				else {
					nodes[k] = temp[j];
					j++;
				}
			}
			else {
				nodes[k] = temp[j];
				j++;
			}
		}
		while (i <= mid) {
			nodes[k] = temp[i];
			k++;
			i++;
		}
		while (j <= high) {
			nodes[k] = temp[j];
			k++;
			j++;
		}
	}
	delete[]temp;
	
}
void sort(Node* nodes, int low, int high, int n, int tag) {
	if (low < high) {
		int mid = (low + high) / 2;
		sort(nodes, low, mid, n, tag);
		sort(nodes, mid + 1, high, n, tag);
		sort_merge(nodes, low, mid, high, n, tag);
	}
}

//求两点间距离
double dis(Node node1, Node node2) {
	double x1 = node1.x;
	double x2 = node2.x;
	double y1 = node1.y;
	double y2 = node2.y;
	double dist = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	return dist;
}

//找最小距离
double closest_pair(Node* node_x, Node* node_y, int n) {
	//n小于等于3，直接求
	//if (n <= 3) {
	//	if (n <= 1) {
	//		return 10000.00;
	//	}
	//	float min = dis(node_x[0], node_x[1]);
	//	for (int i = 0; i < n; i++) {
	//		for (int j = i + 1; j < n; j++) {
	//			if (dis(node_x[i], node_x[j]) < min) {
	//				min = dis(node_x[i], node_x[j]);
	//			}
	//		}
	//	}
	//	//cout << min << endl;
	//	return min;
	//}
	if (n == 1) {
		return 2 << 20;
	}
	if (n == 2) {
		return dis(node_x[0], node_x[1]);
	}
	if (n == 3) {
		double dis1 = dis(node_x[0], node_x[1]);
		double dis2 = dis(node_x[0], node_x[2]);
		double min1 = min(dis1, dis2);
		double dis3 = dis(node_x[1], node_x[2]);
		return min(min1, dis3);
	}



	//划分，构造左右两侧的node_x和node_y
	int ceil_n, floor_n;
	if (n % 2 == 0) {
		ceil_n = n / 2;
		floor_n = n / 2;
	}
	else {
		ceil_n = n / 2 + 1;
		floor_n = n / 2;
	}
	Node* left_x=new Node[ceil_n];
	Node* left_y=new Node[ceil_n];
	Node* right_x = new Node[floor_n];
	Node* right_y = new Node[floor_n];
	int* flag = new int[n];
	for (int i = 0; i < n; i++) {
		flag[i] = 0;
	}
	int i;
	for (i = 0; i < ceil_n; i++) {
		left_x[i] = node_x[i];
	}
	double x1 = left_x[i - 1].x;
	int rightI = 0;
	for (i = ceil_n; i < n; i++) {
		right_x[rightI] = node_x[i];
		rightI++;
		
	}
	double x2 = node_x[ceil_n].x;
	int k = 0;
	if (x1 != x2) {
		for (int j = 0; j < n; j++) {
			if (k >= ceil_n) {
				break;
			}
			if (node_y[j].x <= x1 && k < ceil_n) {
				left_y[k] = node_y[j];
				k++;
				flag[j] = -1;
			}
			
		}
	}
	else {
		for (int j = 0; j < n; j++) {
			if (k >= ceil_n) {
				break;
			}
			if (node_y[j].x < x1 && k < ceil_n) {
				left_y[k] = node_y[j];
				k++;
				flag[j] = -1;
			}

		}
		for (int j = 0; j < n; j++) {
			if (k >= ceil_n) {
				break;
			}
			if (node_y[j].x == x1 && k < ceil_n) {
				left_y[k] = node_y[j];
				k++;
				flag[j] = -1;
			}
		}
	}


	
	k = 0;
	for (int j = 0; j < n; j++) {
		if (k >= floor_n) {
			break;
		}
		if (flag[j] == 0 && k<floor_n) {
			right_y[k] = node_y[j];
			k++;
			flag[j] = -1;
		}
	}

	//左右两侧递归求各自的最小距离
	double leftDis = closest_pair(left_x, left_y, ceil_n);
	double rightDis = closest_pair(right_x, right_y, floor_n);
	if (leftDis * leftDis == 136.00) {
		cout << "left" << endl;
	}
	if (rightDis * rightDis == 136.00) {
		cout << "right" << endl;
	}
	
	//定义窄带宽度
	double minTemp = min(leftDis, rightDis);
	//定义直线L
	double x_L = x1;
	//定义窄带中的点集
	int m = 0;
	for (int i = 0; i < n; i++) {
		if (node_y[i].x >= x_L - minTemp && node_y[i].x <= x_L + minTemp) {
			m++;
		}
	}
	Node* node_Sy = new Node[m];
	int p = 0;
	for (int i = 0; i < n; i++) {
		if (p >= m) {
			break;
		}
		if (node_y[i].x >= x_L - minTemp && node_y[i].x <= x_L + minTemp) {
			node_Sy[p] = node_y[i];
			p++;
		}
	}

	//计算边界处的最小距离
	//minTemp2 = dis(node_Sy[0], node_Sy[1]);
	for (int i = 0; i < p; i++) {
		for (int j = i + 1;  j<p&&node_Sy[j].y-node_Sy[i].y<minTemp; j++) {
			//if (node_Sy[j].y - node_Sy[i].y < 2 * minTemp && abs(node_Sy[j].x - node_Sy[i].x) < 2 * minTemp) {
			double disTemp = dis(node_Sy[i], node_Sy[j]);
			if (disTemp * disTemp == 136.00) {
				cout << "disTemp" << endl;
			}
			if (disTemp < minTemp) {
				minTemp = disTemp;
			}
				
		}
	}
	
	
	delete[]left_x;
	delete[]right_x;
	delete[]left_y;
	delete[]right_y; 
	delete[]node_Sy;
	delete[]flag;

	return minTemp;
	
}



int main()
{
	int n;
	cin >> n;
	Node* nodes = new Node[n];
	for (int i = 0; i < n; i++) {
		cin >> nodes[i].x;
		cin >> nodes[i].y;
	}
	
	Node* nodesX = new Node[n];
	Node* nodesY = new Node[n];
	for (int i = 0; i < n; i++) {
		nodesX[i] = nodes[i];
		nodesY[i] = nodes[i];
	}
	sort(nodesX, 0, n - 1, n, 0); //nodes按照x大小排序
	sort(nodesY, 0, n - 1, n, 1); //nodes按照y大小排序
	double distance = closest_pair(nodesX, nodesY, n);
	cout << setiosflags(ios::fixed) << setprecision(2);
	cout << distance*distance;


}
