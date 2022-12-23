#include <iostream>
#include <queue>
using namespace std;


//物品类
class Goods {
public:
	int number; //物品编号
	int weight; //物品重量
	int value; //物品价值
};
//分支限界法的节点类
class TreeNode {
public:
	int weight;
	int value;
	int treeDeep;
	int flag; //标识是否放入
};

int m(Goods a, Goods b) {
	return (a.value / a.weight) > (b.value / b.weight);
}

//背包类
class Bag {
private:
	Goods* goods;//存所有可选物品
	int W; //最大承重
	int N; //物品总数
	int* choice; //对应每个物品的选择，1表示选，0表示不选
	int bestValue; //最优解时获得的最大收益
	//创建队列，存放分支限界法的节点
	queue<TreeNode> nodeQueue;

	int directMethodHelp(int i, int curValue, int curWeight, int* curChoice); //直接求解的核心代码
	int backMethodTrack(int deep, int curValue, int curWeight, int* curChoice); //回溯法回溯部分
	int bound(int deep, int curValue, int curWeight); //限界函数
	void joinQueue(int deep, int curValue, int curWeight, int flag); //将节点加入队列

public:
	Bag(int W, int N, Goods* goods);
	int getBestValue() { return bestValue; }
	int* getChoice() { return choice; }
	int getN() { return N; }

	int directMethod(); //调用直接求解方法
	int dpMethod(); //动态规划方法
	int backMethod(); //调用回溯法
	int branchMethod(); //调用分支限界法


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
	if (deep == N) { //递归出口
		if (bestValue < curValue && curWeight <= W) {
			for (int j = 0; j < N; j++) {
				choice[j] = curChoice[j]; //存储选择
			}
			bestValue = curValue;
		}
		return bestValue;
	}
	//首先将物品逐渐放入包中，看是否满足
	curValue += goods[deep].value;
	curWeight += goods[deep].weight;
	curChoice[deep] = 1;
	directMethodHelp(deep + 1, curValue, curWeight, curChoice);
	//再逐次减少物品，进行比较
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
	bestValue = 0; //重置bestValue
	for (int i = 0; i < N; i++) {
		choice[i] = 0; //重置choice
		curChoice[i] = 0;
	}
	bestValue = directMethodHelp(0, curValue, curWeight, curChoice);
	return bestValue;
}

int Bag::dpMethod() {
	int** OPT = new int* [N + 1]; //OPT(i,j)表示前i个物品放入容量为j的背包能获得的最大价值
	for (int i = 0; i <= N; i++) {
		OPT[i] = new int[W + 1];
	}
	for (int i = 0; i <= N; i++) { //初始化第0列
		OPT[i][0] = 0;
	}
	for (int j = 0; j <= W; j++) { //初始化第0行
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
		if (OPT[i][j] > OPT[i - 1][j]) { //此时表示选择第i个物品
			choice[i - 1] = 1;
			j -= goods[i - 1].weight;
		}
		else { //此时表示不选择第二个物品
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
	if (deep == N) { //递归出口，已经到达叶子节点
		if (bestValue < curValue) {
			for (int j = 0; j < N; j++) {
				choice[goods[j].number] = curChoice[j]; //存储选择
			}
			bestValue = curValue;
		}
		return bestValue;
	}
	if (goods[deep].weight + curWeight <= W) { //满足约束条件，搜索左子树
		curWeight += goods[deep].weight; //当前剩余容量
		curValue += goods[deep].value; //当前总价值
		curChoice[deep] = 1;
		backMethodTrack(deep + 1, curValue, curWeight, curChoice);
		curWeight -= goods[deep].weight;
		curValue -= goods[deep].value;
	}
	if (bound(deep + 1, curValue, W - curWeight) > bestValue) { //满足限界条件再搜索右子树
		curChoice[deep] = 0;
		backMethodTrack(deep + 1, curValue, curWeight, curChoice);
	}
	return bestValue;
}

int Bag::bound(int deep, int curValue, int curWeight) {
	int left = curWeight; //剩余容量
	int temp = curValue; //当前价值
	while (deep < N && goods[deep].weight <= left) { //构造右子树
		left -= goods[deep].weight;
		temp += goods[deep].value;
		deep++;
	}
	if (deep < N) { //若不能完全放入，将它“尽量”放满
		temp += (left / goods[deep].weight) * goods[deep].value;
	}
	return temp;
}

void Bag::joinQueue(int deep, int curValue, int curWeight, int flag) {
	TreeNode node;
	//给节点赋值
	node.weight = curWeight;
	node.value = curValue;
	node.treeDeep = deep;
	node.flag = flag;
	if (deep == N) { //到达最底层，不加入队列
		if (node.value > bestValue) {
			bestValue = node.value;
		}
	}
	else { //不是最底层，加入队列
		nodeQueue.push(node);
	}
}

int Bag::branchMethod() {
	int deep = 1;
	TreeNode tag; //tag节点为标记节点，表示在同一深度
	tag.weight = -1; //区别其他节点，判断是否进入下一深度
	tag.value = 0;
	tag.treeDeep = 0;
	tag.flag = 0;
	TreeNode node; //node节点表示当前的扩展结点
	node.weight = 0;
	node.value = 0;
	node.treeDeep = 1;
	node.flag = 0;
	nodeQueue.push(tag);//队列最前面加入标记节点
	while (true) {
		//约束条件，判断左子树是否可行
		if (node.weight + goods[deep - 1].weight <= W) { //满足条件，则将扩展节点加入队列
			joinQueue(deep, node.value + goods[deep - 1].value, node.weight + goods[deep - 1].weight, 1);
		}
		//限界条件，判断右子树是否可行
		if (bound(deep, node.value, W - node.weight) > bestValue) {
			joinQueue(deep, node.value, node.weight, 0);
		}
		//记录并去掉队列最前面的点
		node = nodeQueue.front();
		nodeQueue.pop();
		//判断是否为同一深度
		if (node.weight == -1) {
			if (nodeQueue.empty()) {
				break;
			}
			//获取下一个节点
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
	cout << "请输入背包的最大承重：";
	cin >> W;
	cout << "请输入物品的总个数：";
	cin >> N;
	goods = new Goods[N];
	weights = new int[N];
	values = new int[N];
	cout << "请分别输入物品的重量：";
	for (int i = 0; i < N; i++) {
		cin >> weights[i];
	}
	cout << "请分别输入物品的价值：";
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
	cout << "最大价值为：" << bag->getBestValue() << endl;
	cout << "对应的物品选择为：" << endl;
	int n = bag->getN();
	int* choices = new int[n];
	choices = bag->getChoice();
	for (int i = 0; i < n; i++) {
		cout << choices[i] << " ";
	}
	cout << endl;
}
void output1(Bag* bag) {
	cout << "最大价值为：" << bag->getBestValue() << endl;
}

int main()
{
	cout << "直接求解法：" << endl;
	Bag* bag1 = input();
	bag1->directMethod();
	output(bag1);

	cout << "动态规划法：" << endl;
	Bag* bag2 = input();
	bag2->dpMethod();
	output(bag2);

	cout << "回溯法" << endl;
	Bag* bag3 = input();
	bag3->backMethod();
	output(bag3);

	cout << "分支限界法" << endl;
	Bag* bag4 = input();
	bag4->branchMethod();
	output1(bag4);

}