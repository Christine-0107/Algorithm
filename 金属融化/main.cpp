#include <iostream>
using namespace std;
class MeltMental 
{
private:
	int N;
	int T;
	char** array1;
	bool** visited;
	int hm;
	int count;
	int** direction;
	int tag;
	int tag2;
public:
	MeltMental(int num);
	void input();
	void output();
	void resetVisited();
	bool DFSToMarkCount(int x,int y);
	void judgeMelt(int x,int y);
	bool DFSMelt(int x,int y);
	bool meltChange(int x,int y);
	void Melt();
	void judgeFlow(int x, int y);
	void DFSToWater(int x, int y);
	void MarkHollow();

};
MeltMental::MeltMental(int num)
{
	N = num;
	T = 0;
	count = 0;
	tag = 0;
	tag2 = 0;
	array1 = new char* [N];
	visited = new bool* [N];
	for (int i = 0; i < N; i++)
	{
		array1[i] = new char[N];
		visited[i] = new bool[N];
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			array1[i][j] = NULL;
			visited[i][j] = false;
		}
	}
	hm = 0;
	direction = new int* [2];
	for (int i = 0; i < 2; i++)
	{
		direction[i] = new int[4];
	}
	//上下左右
	direction[0][0] = 1;
	direction[0][1] = -1;
	direction[0][2] = 0;
	direction[0][3] = 0;
	direction[1][0] = 0;
	direction[1][1] = 0;
	direction[1][2] = 1;
	direction[1][3] = -1;
}
void MeltMental::input()
{
	cout << "请输入初始形态" << endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cin >> array1[i][j];
		}
	}
}
void MeltMental::output()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << array1[i][j];
		}
		cout << endl;
	}
	cout << endl;
}
void MeltMental::resetVisited()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			visited[i][j] = false;
		}
	}
}
bool MeltMental::DFSToMarkCount(int x,int y)
{
	if (x < 0 || x >= N || y < 0 || y >= N)
		return false;
	visited[x][y] = true;
	if (array1[x][y] == '#')
	{
		count++;
	}
	for (int i = 0; i < 4; i++)
	{
		int xx = x + direction[0][i];
		int yy = y + direction[1][i];
		if (xx >= 0 && xx < N && yy >= 0 && yy < N&&visited[xx][yy]==false)
		{
			DFSToMarkCount(xx, yy);
		}
	}
}
void MeltMental::DFSToWater(int x, int y)
{
	if (x < 0 || x >= N || y < 0 || y >= N || array1[x][y] != '_')
		return;
	visited[x][y] = true;
	array1[x][y] = '+';
	for (int i = 0; i < 4; i++)
	{
		int xx = x + direction[0][i];
		int yy = y + direction[1][i];
		if (xx >= 0 && xx < N && yy >= 0 && yy < N && visited[xx][yy] == false)
		{
			DFSToWater(xx, yy);
		}
	}
}
void MeltMental::MarkHollow()
{
	resetVisited();
	for (int i = 0; i < N; i++)
	{
		if (array1[0][i] == '_')
			DFSToWater(0, i);
		if (array1[N - 1][i] == '_')
			DFSToWater(N - 1, i);
		if (array1[i][0] == '_')
			DFSToWater(i, 0);
		if (array1[i][N - 1] == '_')
			DFSToWater(i, N - 1);
	}
}
void MeltMental::judgeFlow(int x, int y)
{
	//int flag2 = 0;
	for (int i = 0; i < 4; i++)
	{
		int xx = x + direction[0][i];
		int yy = y + direction[1][i];
		if (xx >= 0 && xx < N && yy >= 0 && yy < N)
		{
			if (array1[xx][yy] == '_')
			{
				//flag2++;
				array1[xx][yy] = '1';
				judgeFlow(xx, yy);
			}
		}
	}
	

}
void MeltMental::judgeMelt(int x, int y)
{
	int flag = 0;
	if (x>0&&array1[x - 1][y] == '+')
		flag++;
	if (x<N-1&&array1[x + 1][y] == '+')
		flag++;
	if (y>0&&array1[x][y - 1] == '+')
		flag++;
	if (y<N-1&&array1[x][y + 1] == '+')
		flag++;
	if (flag >= 2)
	{
		array1[x][y] = '1'; //把此次能够融化的做上标记
		judgeFlow(x, y);
		count--;
		if (count == tag)
			tag2 = 1;

	}
		
}
bool MeltMental::DFSMelt(int x, int y)
{
	if (x < 0 || x >= N || y < 0 || y >= N)
		return false;
	visited[x][y] = true;
	if (array1[x][y] == '#')
		judgeMelt(x, y);
	for (int i = 0; i < 4; i++)
	{
		int xx = x + direction[0][i];
		int yy = y + direction[1][i];
		if (xx >= 0 && xx < N && yy >= 0 && yy < N&&visited[xx][yy]==false)
		{
			DFSMelt(xx, yy);
		}
	}
}
bool MeltMental::meltChange(int x,int y)
{
	if (x < 0 || x >= N || y < 0 || y >= N)
		return false;
	visited[x][y] = true;
	if (array1[x][y] == '1')
	{
		array1[x][y] = '+'; //把能融化的变为液体
	}
	for (int i = 0; i < 4; i++)
	{
		int xx = x + direction[0][i];
		int yy = y + direction[1][i];
		if (xx >= 0 && xx < N && yy >= 0 && yy < N && visited[xx][yy] == false)
		{
			meltChange(xx, yy);
		}
	}
}
void MeltMental::Melt()
{
	while (true)
	{
		resetVisited();
		count = 0;
		DFSToMarkCount(0, 0); //计算需要融化的金属个数
		if (count == 0) //没有金属
			break;
		resetVisited();
		DFSMelt(0, 0);
		resetVisited();
		meltChange(0, 0);
		if (count == tag&&tag2!=1)
		{
			cout << "无法融化" << endl;
			T = 0;
			break;
		}
		tag = count;
		T++;
		//hollow();
		cout << "T=" << T << endl;
		output();
	}
	cout << "所用时间为：";
	cout << T << endl;
}


int main()
{
	int N;
	cout << "请输入N：";
	cin >> N;
	MeltMental m(N);
	m.input();
	m.MarkHollow();
	cout << endl;
	m.output();
	m.Melt();
	return 0;

}