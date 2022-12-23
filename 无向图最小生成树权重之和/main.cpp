#include <iostream>
using namespace std;

struct near { //������뵱ǰ������ĵ�͸ñ��ϵ�Ȩ��
	int neighborVertex;
	int smallWeight;
};

void initGraph(int** edgeWeight,int vertexNum,int edgeNum) {
	int v1, v2, w;
	for (int i = 1; i <= edgeNum; i++) {
		cin >> v1 >> v2 >> w;//����ÿ���ߵ��������㣬��Ȩ��
		edgeWeight[v1][v2] = w;
		edgeWeight[v2][v1] = w; //����ͼ�������򶼸�ֵ
	}
}

int getNear(near* nearest, int vertexNum) { //ѡһ�����뵱ǰ������ĵ�
	int vertex_target = -1;
	for (int i = 1; i <= vertexNum; i++) {
		if (nearest[i].smallWeight > 0) {
			vertex_target = i; //��Ϊvertex_target����ʼ��
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
	int minWeight = 0; //��ʼ��СȨ��֮��Ϊ0
	int vertex_start = 1; //�Ӷ���1��ʼ
	near* nearest = new near[vertexNum+1]; //������ÿ��������ĵ㣨Ȩֵ��С��
	for (int i = 0; i <= vertexNum; i++) { //��ʼ��
		nearest[i].neighborVertex = 0;
		nearest[i].smallWeight = 1000;
	}
	//��ʼʱ�Ƚ����ж�������ڽ�����Ϊ����1
	for (int i = 1; i <= vertexNum; i++) {
		nearest[i].neighborVertex = vertex_start;
		nearest[i].smallWeight = edgeWeight[i][vertex_start];
	}
	nearest[vertex_start].smallWeight = 0;//�Ѱ����ڵ㵽�ڲ��ڵ��ȨֵΪ0

	for (int i = 0; i < vertexNum-1; i++) { //ѭ��������-1�Σ�����������Ȧ����
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
	int vertexNum = 0; //������
	int edgeNum = 0; //����
	int** edgeWeight; //����������֮��ߵ�Ȩ��
	cin >> vertexNum >> edgeNum; //���붥������ͱ���
	edgeWeight = new int* [vertexNum+1];
	for (int i = 0; i <= vertexNum; i++) {
		edgeWeight[i] = new int[vertexNum +1];
	}
	for (int i = 0; i <= vertexNum; i++) {
		for (int j = 0; j <= vertexNum; j++) {
			edgeWeight[i][j] = 1000; //������Ȩ�ز�����100���ȳ�ʼ��Ϊ1000
		}
	}
	initGraph(edgeWeight,vertexNum,edgeNum); //��ʼ�����ö�ά�����ͼ
	int minWeight;
	minWeight = prim(edgeWeight, vertexNum, edgeNum); //����prim��������СȨ��֮��
	cout << minWeight;
}