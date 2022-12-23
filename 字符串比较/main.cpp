//定义两个非空字符不匹配开销为a；
//定义空隙罚分为k;
#include <iostream>
#include <cmath>
using namespace std;


int spaceEfficientAlignment(char* x,char* y,int k) {
	int m = 0; //x的长度
	int n = 0; //y的长度
	
	for (int i = 0; x[i] != '\0'; i++) {
		m++;
	}
	for (int i = 0; y[i] != '\0'; i++) {
		n++;
	}
	int** foldArray = new int* [m+1];
	for (int i = 0; i <= m; i++) {
		foldArray[i] = new int[2];
	}
	for (int i = 0; i <= m; i++) {
		foldArray[i][0] = i * k;
	}
	int result1, result2, result3;
	for (int j = 1; j <= n; j++) {
		foldArray[0][1] = j * k;
		for (int i = 1; i <= m; i++) {
			result1 = (abs(x[i-1] - y[j-1])) + foldArray[i - 1][0];
			result2 = k + foldArray[i - 1][1];
			result3 = k + foldArray[i][0];
			int min = result1;
			if (result2 < min) {
				if (result3 < result2) {
					min = result3;
				}
				else {
					min = result2;
				}
			}
			else {
				if (result3 < result1) {
					min = result3;
				}
				else {
					min = result1;
				}
			}
			foldArray[i][1] = min;
		}
		for (int i = 0; i <= m; i++) {
			foldArray[i][0] = foldArray[i][1];
		}
	}
	return foldArray[m][1];
}

int main() {
	char x[2000];
	char y[2000];
	int k;
	cin >> x;
	cin >> y;
	cin >> k; //对上空隙的距离值

	

	int result;
	result=spaceEfficientAlignment(x, y, k);
	cout << result << endl;
}

