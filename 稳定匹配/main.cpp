#include <iostream>
using namespace std;

//��ת�������б��ɱ�Ŷ�λ����
int** inverse(int** person, int n) {
	int** prefer = new int* [n];
	for (int i = 0; i < n; i++) {
		prefer[i] = new int[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			prefer[i][person[i][j] - 1] = j; //�ɱ�Ŷ�λ����
		}
	}
	return prefer;

}

//�ж��Ƿ������ɵ���(��֤����ĳ����ʿ���ɣ���һ������δ����Ůʿ)
bool hasSingleMan(int* manSingle, int n) {
	for (int i = 0; i < n; i++) {
		if (manSingle[i] == 0) {
			return true;
		}
	}
	return false;
}

//ѡ��һλ������ʿ��ѡ���������㷨ִ�н��û��Ӱ�죩
//�õ��ı�Ŵ�0��ʼ
int selectASingleMan(int* manSingle, int n) {
	for (int i = 0; i < n; i++) {
		if (manSingle[i] == 0) {
			return i;
		}
	}
}

//G-S�㷨
int* GS(int n, int* manSingle, int** man, int* womanSingle, int** womanPrefer) {
	int* engageManToWoman = new int[n]; //�����б�-��ʿȷ��Ůʿ
	int* engageWomanToMan = new int[n]; //�����б�-Ůʿȷ����ʿ
	int m; //��ǰ��ѡ����ʿm
	int w; //mδ����б���������һ��Ůʿw
	int pos; //w��m�����б��λ��
	int m1; //w���ܴ��ڵ��Ѷ������m1
	
	while (hasSingleMan(manSingle, n)) {
		m = selectASingleMan(manSingle, n);
		for (int j = 0; j < n; j++) {
			if (man[m][j] != -1) {
				w = man[m][j] - 1;//��Ůʿ�ı��תΪ��0��ʼ
				pos = j;
				man[m][pos] = -1; //����m�Ѿ������w��
				break;
			}
		}
		if (womanSingle[w] == 0) { //wΪ���ɵ�
			manSingle[m] = 1; //m��Ϊ����
			womanSingle[w] = 1; //w��Ϊ����
			engageManToWoman[m] = w;
			engageWomanToMan[w] = m;
		}
		else {
			m1 = engageWomanToMan[w]; //w�Ѿ���m1����
			if (womanPrefer[w][m1] < womanPrefer[w][m]) { //w��ϲ��m1
				manSingle[m] = 0; //m�������ֵ���
			}
			else { //w��ϲ��m
				manSingle[m1] = 0; //m1�ָ�����
				manSingle[m] = 1; //m��Ϊ����
				engageManToWoman[m] = w;
				engageWomanToMan[w] = m;
			}
		}
	}
	return engageManToWoman;
}

int main() {
	//������Ů����N������Ů�����б�
	int N;
	cin >> N;
	int** MAN = new int* [N];
	int** WOMAN = new int* [N];
	for (int i = 0; i < N; i++) {
		MAN[i] = new int[N];
		WOMAN[i] = new int[N];
	}
	//����
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
	//�����Ƿ����ɵ����飬����Ϊ0������Ϊ1������ʼ��Ϊ0
	int* ManSingle = new int[N];
	int* WomanSingle = new int[N];
	for (int i = 0; i < N; i++) { 
		ManSingle[i] = 0;
		WomanSingle[i] = 0;
	}
	//���庯���������б�ת���ɱ�Ŷ�λ����
	int** WomanPrefer = inverse(WOMAN, N);
	int** ManPrefer = inverse(MAN, N);
	

	//��ʼG-S�㷨
	int* engage = GS(N, ManSingle, MAN, WomanSingle, WomanPrefer);

	for (int i = 0; i < N ; i++) {
		cout << engage[i] + 1 << " ";
	}
	return 0;
}