#include <iostream>
using namespace std;

struct near { //保存距离当前点最近的点和该边上的权重
	int neighborVertex;
	int smallWeight;
};

void initGraph(int** edgeWeight,int vertexNum,int edgeNum) {
	int v1, v2, w;
	for (int i = 1; i <= edgeNum; i++) {
		cin >> v1 >> v2 >> w;//输入每条边的两个定点，和权重
		edgeWeight[v1][v2] = w;
		edgeWeight[v2][v1] = w; //无向图，两方向都赋值
	}
}

int getNear(near* nearest, int vertexNum) { //选一个距离当前点最近的点
	int vertex_target = -1;
	for (int i = 1; i <= vertexNum; i++) {
		if (nearest[i].smallWeight > 0) {
			vertex_target = i; //先为vertex_target做初始化
			break;
		}
	}
	for (int i = vertex_target + 1; i <= vertexNum; i++) {
		if (nearest[i].smallWeight > 0 && (nearest[i].smallWeight < nearest[vertex_target].smallWeight)) {
			vertex_target = i;
		}
	}
	return vertex_target;
}

int prim(int** edgeWeight, int vertexNum, int edgeNum) {
	int minWeight = 0; //初始最小权重之和为0
	int vertex_start = 1; //从顶点1开始
	near* nearest = new near[vertexNum+1]; //保存了每个点最近的点（权值最小）
	for (int i = 0; i <= vertexNum; i++) { //初始化
		nearest[i].neighborVertex = 0;
		nearest[i].smallWeight = 1000;
	}
	//初始时先将所有顶点的最邻近点设为顶点1
	for (int i = 1; i <= vertexNum; i++) {
		nearest[i].neighborVertex = vertex_start;
		nearest[i].smallWeight = edgeWeight[i][vertex_start];
	}
	nearest[vertex_start].smallWeight = 0;//已包含节点到内部节点的权值为0

	for (int i = 0; i < vertexNum-1; i++) { //循环顶点数-1次，把其他顶点圈进来
		int vertex_target = getNear(nearest, vertexNum);
		minWeight += nearest[vertex_target].smallWeight;
		nearest[vertex_target].smallWeight = 0;
		for (int j = 1; j <= vertexNum; j++) {
			if (nearest[j].smallWeight > edgeWeight[j][vertex_target]) {
				nearest[j].neighborVertex = vertex_target;
				nearest[j].smallWeight = edgeWeight[j][vertex_target];
			}
		}
	}
	return minWeight;

}

int main() {
	int vertexNum = 0; //顶点数
	int edgeNum = 0; //边数
	int** edgeWeight; //保存两顶点之间边的权重
	cin >> vertexNum >> edgeNum; //输入顶点个数和边数
	edgeWeight = new int* [vertexNum+1];
	for (int i = 0; i <= vertexNum; i++) {
		edgeWeight[i] = new int[vertexNum +1];
	}
	for (int i = 0; i <= vertexNum; i++) {
		for (int j = 0; j <= vertexNum; j++) {
			edgeWeight[i][j] = 1000; //本题中权重不超过100，先初始化为1000
		}
	}
	initGraph(edgeWeight,vertexNum,edgeNum); //初始化，用二维数组存图
	int minWeight;
	minWeight = prim(edgeWeight, vertexNum, edgeNum); //调用prim，返回最小权重之和
	cout << minWeight;
}