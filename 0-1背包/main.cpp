#include <iostream>
#include <queue>
using namespace std;


//��Ʒ��
class Goods {
public:
	int number; //��Ʒ���
	int weight; //��Ʒ����
	int value; //��Ʒ��ֵ
};
//��֧�޽編�Ľڵ���
class TreeNode {
public:
	int weight;
	int value;
	int treeDeep;
	int flag; //��ʶ�Ƿ����
};

int m(Goods a, Goods b) {
	return (a.value / a.weight) > (b.value / b.weight);
}

//������
class Bag {
private:
	Goods* goods;//�����п�ѡ��Ʒ
	int W; //������
	int N; //��Ʒ����
	int* choice; //��Ӧÿ����Ʒ��ѡ��1��ʾѡ��0��ʾ��ѡ
	int bestValue; //���Ž�ʱ��õ��������
	//�������У���ŷ�֧�޽編�Ľڵ�
	queue<TreeNode> nodeQueue;

	int directMethodHelp(int i, int curValue, int curWeight, int* curChoice); //ֱ�����ĺ��Ĵ���
	int backMethodTrack(int deep, int curValue, int curWeight, int* curChoice); //���ݷ����ݲ���
	int bound(int deep, int curValue, int curWeight); //�޽纯��
	void joinQueue(int deep, int curValue, int curWeight, int flag); //���ڵ�������

public:
	Bag(int W, int N, Goods* goods);
	int getBestValue() { return bestValue; }
	int* getChoice() { return choice; }
	int getN() { return N; }

	int directMethod(); //����ֱ����ⷽ��
	int dpMethod(); //��̬�滮����
	int backMethod(); //���û��ݷ�
	int branchMethod(); //���÷�֧�޽編


};
Bag::Bag(int W, int N, Goods* goods) {
	this->W = W;
	this->N = N;
	this->goods = new Goods[N];
	this->goods = goods;
	this->choice = new int[N];
	for (int i = 0; i < N; i++) {
		choice[i] = 0;
	}
	this->bestValue = 0;
}

int Bag::directMethodHelp(int deep, int curValue, int curWeight, int* curChoice) {
	if (deep == N) { //�ݹ����
		if (bestValue < curValue && curWeight <= W) {
			for (int j = 0; j < N; j++) {
				choice[j] = curChoice[j]; //�洢ѡ��
			}
			bestValue = curValue;
		}
		return bestValue;
	}
	//���Ƚ���Ʒ�𽥷�����У����Ƿ�����
	curValue += goods[deep].value;
	curWeight += goods[deep].weight;
	curChoice[deep] = 1;
	directMethodHelp(deep + 1, curValue, curWeight, curChoice);
	//����μ�����Ʒ�����бȽ�
	curValue -= goods[deep].value;
	curWeight -= goods[deep].weight;
	curChoice[deep] = 0;
	directMethodHelp(deep + 1, curValue, curWeight, curChoice);
	return bestValue;
}
int Bag::directMethod() {
	int curValue = 0;
	int curWeight = 0;
	int* curChoice = new int[N];
	bestValue = 0; //����bestValue
	for (int i = 0; i < N; i++) {
		choice[i] = 0; //����choice
		curChoice[i] = 0;
	}
	bestValue = directMethodHelp(0, curValue, curWeight, curChoice);
	return bestValue;
}

int Bag::dpMethod() {
	int** OPT = new int* [N + 1]; //OPT(i,j)��ʾǰi����Ʒ��������Ϊj�ı����ܻ�õ�����ֵ
	for (int i = 0; i <= N; i++) {
		OPT[i] = new int[W + 1];
	}
	for (int i = 0; i <= N; i++) { //��ʼ����0��
		OPT[i][0] = 0;
	}
	for (int j = 0; j <= W; j++) { //��ʼ����0��
		OPT[0][j] = 0;
	}
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= W; j++) {
			if (j < goods[i - 1].weight) {
				OPT[i][j] = OPT[i - 1][j];
			}
			else {
				OPT[i][j] = max(OPT[i - 1][j], goods[i - 1].value + OPT[i - 1][j - goods[i - 1].weight]);
			}
		}
	}
	for (int i = N, j = W; i > 0; i--) {
		if (OPT[i][j] > OPT[i - 1][j]) { //��ʱ��ʾѡ���i����Ʒ
			choice[i - 1] = 1;
			j -= goods[i - 1].weight;
		}
		else { //��ʱ��ʾ��ѡ��ڶ�����Ʒ
			choice[i - 1] = 0;
		}
	}
	bestValue = OPT[N][W];
	return OPT[N][W];
}

int Bag::backMethod() {
	int deep = 0;
	int curValue = 0;
	int curWeight = 0;
	int* curChoice = new int[N];
	sort(goods, goods + N, m);
	bestValue = backMethodTrack(deep, curValue, curWeight, curChoice);
	return bestValue;
}
int Bag::backMethodTrack(int deep, int curValue, int curWeight, int* curChoice) {
	if (deep == N) { //�ݹ���ڣ��Ѿ�����Ҷ�ӽڵ�
		if (bestValue < curValue) {
			for (int j = 0; j < N; j++) {
				choice[goods[j].number] = curChoice[j]; //�洢ѡ��
			}
			bestValue = curValue;
		}
		return bestValue;
	}
	if (goods[deep].weight + curWeight <= W) { //����Լ������������������
		curWeight += goods[deep].weight; //��ǰʣ������
		curValue += goods[deep].value; //��ǰ�ܼ�ֵ
		curChoice[deep] = 1;
		backMethodTrack(deep + 1, curValue, curWeight, curChoice);
		curWeight -= goods[deep].weight;
		curValue -= goods[deep].value;
	}
	if (bound(deep + 1, curValue, W - curWeight) > bestValue) { //�����޽�����������������
		curChoice[deep] = 0;
		backMethodTrack(deep + 1, curValue, curWeight, curChoice);
	}
	return bestValue;
}

int Bag::bound(int deep, int curValue, int curWeight) {
	int left = curWeight; //ʣ������
	int temp = curValue; //��ǰ��ֵ
	while (deep < N && goods[deep].weight <= left) { //����������
		left -= goods[deep].weight;
		temp += goods[deep].value;
		deep++;
	}
	if (deep < N) { //��������ȫ���룬����������������
		temp += (left / goods[deep].weight) * goods[deep].value;
	}
	return temp;
}

void Bag::joinQueue(int deep, int curValue, int curWeight, int flag) {
	TreeNode node;
	//���ڵ㸳ֵ
	node.weight = curWeight;
	node.value = curValue;
	node.treeDeep = deep;
	node.flag = flag;
	if (deep == N) { //������ײ㣬���������
		if (node.value > bestValue) {
			bestValue = node.value;
		}
	}
	else { //������ײ㣬�������
		nodeQueue.push(node);
	}
}

int Bag::branchMethod() {
	int deep = 1;
	TreeNode tag; //tag�ڵ�Ϊ��ǽڵ㣬��ʾ��ͬһ���
	tag.weight = -1; //���������ڵ㣬�ж��Ƿ������һ���
	tag.value = 0;
	tag.treeDeep = 0;
	tag.flag = 0;
	TreeNode node; //node�ڵ��ʾ��ǰ����չ���
	node.weight = 0;
	node.value = 0;
	node.treeDeep = 1;
	node.flag = 0;
	nodeQueue.push(tag);//������ǰ������ǽڵ�
	while (true) {
		//Լ���������ж��������Ƿ����
		if (node.weight + goods[deep - 1].weight <= W) { //��������������չ�ڵ�������
			joinQueue(deep, node.value + goods[deep - 1].value, node.weight + goods[deep - 1].weight, 1);
		}
		//�޽��������ж��������Ƿ����
		if (bound(deep, node.value, W - node.weight) > bestValue) {
			joinQueue(deep, node.value, node.weight, 0);
		}
		//��¼��ȥ��������ǰ��ĵ�
		node = nodeQueue.front();
		nodeQueue.pop();
		//�ж��Ƿ�Ϊͬһ���
		if (node.weight == -1) {
			if (nodeQueue.empty()) {
				break;
			}
			//��ȡ��һ���ڵ�
			node = nodeQueue.front();
			nodeQueue.pop();
			nodeQueue.push(tag);
			deep++;
		}
	}
	return bestValue;

}

Bag* input() {
	int W, N;
	Goods* goods;
	int* weights;
	int* values;
	cout << "�����뱳���������أ�";
	cin >> W;
	cout << "��������Ʒ���ܸ�����";
	cin >> N;
	goods = new Goods[N];
	weights = new int[N];
	values = new int[N];
	cout << "��ֱ�������Ʒ��������";
	for (int i = 0; i < N; i++) {
		cin >> weights[i];
	}
	cout << "��ֱ�������Ʒ�ļ�ֵ��";
	for (int i = 0; i < N; i++) {
		cin >> values[i];
	}
	for (int i = 0; i < N; i++) {
		goods[i].number = i;
		goods[i].weight = weights[i];
		goods[i].value = values[i];
	}
	Bag* bag = new Bag(W, N, goods);
	return bag;
}

void output(Bag* bag) {
	cout << "����ֵΪ��" << bag->getBestValue() << endl;
	cout << "��Ӧ����Ʒѡ��Ϊ��" << endl;
	int n = bag->getN();
	int* choices = new int[n];
	choices = bag->getChoice();
	for (int i = 0; i < n; i++) {
		cout << choices[i] << " ";
	}
	cout << endl;
}
void output1(Bag* bag) {
	cout << "����ֵΪ��" << bag->getBestValue() << endl;
}

int main()
{
	cout << "ֱ����ⷨ��" << endl;
	Bag* bag1 = input();
	bag1->directMethod();
	output(bag1);

	cout << "��̬�滮����" << endl;
	Bag* bag2 = input();
	bag2->dpMethod();
	output(bag2);

	cout << "���ݷ�" << endl;
	Bag* bag3 = input();
	bag3->backMethod();
	output(bag3);

	cout << "��֧�޽編" << endl;
	Bag* bag4 = input();
	bag4->branchMethod();
	output1(bag4);

}