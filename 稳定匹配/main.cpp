#include <iostream>
using namespace std;

//反转的优先列表，由编号定位排名
int** inverse(int** person, int n) {
	int** prefer = new int* [n];
	for (int i = 0; i < n; i++) {
		prefer[i] = new int[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			prefer[i][person[i][j] - 1] = j; //由编号定位排名
		}
	}
	return prefer;

}

//判断是否还有自由的人(可证，当某个男士自由，它一定有尚未求婚的女士)
bool hasSingleMan(int* manSingle, int n) {
	for (int i = 0; i < n; i++) {
		if (manSingle[i] == 0) {
			return true;
		}
	}
	return false;
}

//选择一位自由男士（选择次序对于算法执行结果没有影响）
//得到的编号从0开始
int selectASingleMan(int* manSingle, int n) {
	for (int i = 0; i < n; i++) {
		if (manSingle[i] == 0) {
			return i;
		}
	}
}

//G-S算法
int* GS(int n, int* manSingle, int** man, int* womanSingle, int** womanPrefer) {
	int* engageManToWoman = new int[n]; //订婚列表-男士确定女士
	int* engageWomanToMan = new int[n]; //订婚列表-女士确定男士
	int m; //当前所选的男士m
	int w; //m未求婚列表中排名第一的女士w
	int pos; //w在m优先列表的位置
	int m1; //w可能存在的已订婚对象m1
	
	while (hasSingleMan(manSingle, n)) {
		m = selectASingleMan(manSingle, n);
		for (int j = 0; j < n; j++) {
			if (man[m][j] != -1) {
				w = man[m][j] - 1;//把女士的编号转为从0开始
				pos = j;
				man[m][pos] = -1; //代表m已经邀请过w了
				break;
			}
		}
		if (womanSingle[w] == 0) { //w为自由的
			manSingle[m] = 1; //m不为单身
			womanSingle[w] = 1; //w不为单身
			engageManToWoman[m] = w;
			engageWomanToMan[w] = m;
		}
		else {
			m1 = engageWomanToMan[w]; //w已经与m1订婚
			if (womanPrefer[w][m1] < womanPrefer[w][m]) { //w更喜欢m1
				manSingle[m] = 0; //m继续保持单身
			}
			else { //w更喜欢m
				manSingle[m1] = 0; //m1恢复自由
				manSingle[m] = 1; //m不为单身
				engageManToWoman[m] = w;
				engageWomanToMan[w] = m;
			}
		}
	}
	return engageManToWoman;
}

int main() {
	//声明男女人数N，和男女优先列表
	int N;
	cin >> N;
	int** MAN = new int* [N];
	int** WOMAN = new int* [N];
	for (int i = 0; i < N; i++) {
		MAN[i] = new int[N];
		WOMAN[i] = new int[N];
	}
	//输入
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> MAN[i][j];
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> WOMAN[i][j];
		}
	}
	//设立是否自由的数组，自由为0，订婚为1，并初始化为0
	int* ManSingle = new int[N];
	int* WomanSingle = new int[N];
	for (int i = 0; i < N; i++) { 
		ManSingle[i] = 0;
		WomanSingle[i] = 0;
	}
	//定义函数将优先列表反转，由编号定位排名
	int** WomanPrefer = inverse(WOMAN, N);
	int** ManPrefer = inverse(MAN, N);
	

	//开始G-S算法
	int* engage = GS(N, ManSingle, MAN, WomanSingle, WomanPrefer);

	for (int i = 0; i < N ; i++) {
		cout << engage[i] + 1 << " ";
	}
	return 0;
}