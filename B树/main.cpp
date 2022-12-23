#include <iostream>
#include <queue>
#include <graphics.h>
#include <conio.h>
using namespace std;

enum Error_code { duplicate_error, overflow, success, not_present };

class BTree;
class BNode
{
	friend class BTree;
public:
	//bool isLeaf;
	int keyNum;
	int *data;
	BNode** branch;
	BNode(int order) 
	{  
		keyNum = 0; 
		data = new int[order - 1];
		branch = new BNode * [order];
		for (int i = 0; i < order; i++)
			branch[i] = nullptr;
	}
};
class BTree
{
private:
	int order;
	BNode* root;

	Error_code push_down(BNode* current, const int& new_entry,
		int& median, BNode* &right_branch)
	{
		
		Error_code result;
		int position=0;
		if (current == nullptr)
		{
			median = new_entry;
			right_branch = nullptr;
			result = overflow;
		}
		else
		{
			if (search_node(current, new_entry, position) == success)
			{
				result = duplicate_error;
			}
			else
			{
				int extra_entry;
				BNode* extra_branch;
				result = push_down(current->branch[position], new_entry, 
					extra_entry, extra_branch);
				if (result == overflow)
				{
					if (current->keyNum < order - 1)
					{
						result = success;
						push_in(current, extra_entry, extra_branch, position);
					}
					else
					{
						split_node(current, extra_entry, extra_branch, 
							position, right_branch, median);
					}
				}
			}
		}
		return result;
	}

	void push_in(BNode* current, const int& entry,
		BNode* right_branch, int position)
	{
		for (int i = current->keyNum; i > position; i--)
		{
			current->data[i] = current->data[i - 1];
			current->branch[i + 1] = current->branch[i];
		}
		current->data[position] = entry;
		current->branch[position + 1] = right_branch;
		current->keyNum++;
	}

	void split_node(BNode* current, const int& extra_entry,
		BNode* extra_branch, int position,
		BNode* &right_half, int& median)
	{
		right_half = new BNode(order);
		int mid = order / 2;
		if (position <= mid) //extra_entry belongs to left half
		{
			for (int i = mid; i < order - 1; i++) //move entries to right half
			{
				right_half->data[i - mid] = current->data[i];
				right_half->branch[i + 1 - mid] = current->branch[i + 1];
			}
			current->keyNum = mid;
			right_half->keyNum = order - 1 - mid;
			push_in(current, extra_entry, extra_branch, position);
		}
		else //extra_entry belongs to right half
		{
			mid++;
			for (int i = mid; i < order - 1; i++)
			{
				right_half->data[i - mid] = current->data[i];
				right_half->branch[i + 1 - mid] = current->branch[i + 1];
			}
			current->keyNum = mid;
			right_half->keyNum = order - 1 - mid;
			push_in(right_half, extra_entry, extra_branch, position-mid);
		}
		median = current->data[current->keyNum - 1];
		right_half->branch[0] = current->branch[current->keyNum];
		current->keyNum--;
	}

	Error_code recursive_remove(BNode* current, const int& target)
	{
		Error_code result;
		int position;
		if (current == nullptr)
			result = not_present;
		else
		{
			if (search_node(current, target, position) == success)
			{
				result = success;
				if (current->branch[position] != nullptr)
				{
					copy_in_predecessor(current, position);
					recursive_remove(current->branch[position], current->data[position]);
				}
				else
				{
					remove_data(current, position);
				}
			}
			else
			{
				result = recursive_remove(current->branch[position], target);
			}
			if (current->branch[position] != nullptr)
			{
				if (current->branch[position]->keyNum < (order - 1) / 2)
				{
					restore(current, position);
				}
			}
		}
		return result;
	}

	void remove_data(BNode* current, int position)
	{
		for (int i = position; i < current->keyNum - 1; i++)
		{
			current->data[i] = current->data[i + 1];
		}
		current->keyNum--;
	}

	void copy_in_predecessor(BNode* current, int position)
	{
		BNode* leaf = current->branch[position];
		while (leaf->branch[leaf->keyNum] != nullptr)
		{
			leaf = leaf->branch[leaf->keyNum];
		}
		current->data[position] = leaf->data[leaf->keyNum - 1];
	}

	void restore(BNode* current, int position)
	{
		if (position == current->keyNum)
		{
			if (current->branch[position - 1]->keyNum > (order - 1) / 2)
				move_right(current, position - 1);
			else
				combine(current, position);
		}
		else if (position == 0)
		{
			if (current->branch[1]->keyNum > (order - 1) / 2)
				move_left(current, 1);
			else
				combine(current, 1);
		}
		else
		{
			if (current->branch[position - 1]->keyNum > (order - 1) / 2)
				move_right(current, position - 1);
			else if (current->branch[position + 1]->keyNum > (order - 1) / 2)
				move_left(current, position + 1);
			else
				combine(current, position);
		}
	}

	void move_left(BNode* current, int position)
	{
		BNode* left_branch = current->branch[position - 1],
			* right_branch = current->branch[position];
		left_branch->data[left_branch->keyNum] = current->data[position - 1];
		left_branch->branch[++left_branch->keyNum] = right_branch->branch[0];
		current->data[position - 1] = right_branch->data[0];
		right_branch->keyNum--;
		for (int i = 0; i < right_branch->keyNum; i++)
		{
			right_branch->data[i] = right_branch->data[i + 1];
			right_branch->branch[i] = right_branch->branch[i + 1];
		}
		right_branch->branch[right_branch->keyNum] = 
			right_branch->branch[right_branch->keyNum + 1];
	}

	void move_right(BNode* current, int position)
	{
		BNode* right_branch = current->branch[position + 1];
		BNode* left_branch = current->branch[position];
		right_branch->branch[right_branch->keyNum + 1] =
			right_branch->branch[right_branch->keyNum];
		for (int i = right_branch->keyNum; i > 0; i--)
		{
			right_branch->data[i] = right_branch->data[i - 1];
			right_branch->branch[i] = right_branch->branch[i - 1];
		}
		right_branch->keyNum++;
		right_branch->data[0] = current->data[position];
		right_branch->branch[0] = left_branch->branch[left_branch->keyNum--];
		current->data[position] = left_branch->data[left_branch->keyNum];
	}

	void combine(BNode* current, int position)
	{
		int i;
		BNode* left_branch = current->branch[position - 1];
		BNode* right_branch = current->branch[position];
		left_branch->data[left_branch->keyNum] = current->data[position - 1];
		left_branch->branch[++left_branch->keyNum] = right_branch->branch[0];
		for (i = 0; i < right_branch->keyNum; i++)
		{
			left_branch->data[left_branch->keyNum] = right_branch->data[i];
			left_branch->branch[++left_branch->keyNum] = right_branch->branch[i + 1];
		}
		current->keyNum--;
		for (i = position - 1; i < current->keyNum; i++)
		{
			current->data[i] = current->data[i + 1];
			current->branch[i + 1] = current->branch[i + 2];
		}
		delete right_branch;
	}

public:
	BTree(int order)
	{
		this->order = order;
		root = nullptr;
	}

	Error_code search_node(BNode* current, const int& target, int& position)
	{
		position = 0;
		while (position<current->keyNum && target>current->data[position])
		{
			position++;
		}
		if (position < current->keyNum && target == current->data[position])
		{
			return success;
		}
		else
		{
			return not_present;
		}
	}

	Error_code insert(const int& new_entry)
	{
		int median;
		BNode* right_branch, * new_root;
		Error_code result = push_down(root, new_entry, median, right_branch);
		
		if (result == overflow)
		{
			new_root = new BNode(order);
			new_root->keyNum = 1;
			new_root->data[0] = median;
			new_root->branch[0] = root;
			new_root->branch[1] = right_branch;
			root = new_root;
			result = success;
		}
		return result;
	}

	Error_code remove(const int& target)
	{
		Error_code result;
		result = recursive_remove(root,target);
		if (root != nullptr && root->keyNum == 0)
		{
			BNode* old_root = root;
			root = root->branch[0];
			delete old_root;
		}
		return result;
	}

	void show()
	{
		cleardevice(); //每次操作前先清屏
		int width, length, margin;
		length = 10;
		width = 20;
		margin = 30;
		if (root == nullptr)
			return;
		queue<BNode*> qBn;
		queue<int> qLeft, qTop;
		qBn.push(root);
		qBn.push(nullptr);
		qLeft.push(0);
		qTop.push(0);
		qLeft.push(-1);
		qTop.push(-1);
		int rowLeft = 0;
		while (qBn.size() > 1)
		{
			BNode* current = qBn.front();
			int left, top;
			left = qLeft.front();
			top = qTop.front();
			qBn.pop();
			qLeft.pop();
			qTop.pop();
			if (current == nullptr)
			{
				qBn.push(nullptr);
				qLeft.push(-1);
				qTop.push(-1);
				rowLeft = 0;
				continue;
			}
			int totalLength = 0;
			int preCharRihgt = left;
			for (int i = 0; i < current->keyNum; i++)
			{ //显示当前节点的数据
				//TCHAR s[100];
				wchar_t s[100];
				swprintf_s(s, _T("%d"), current->data[i]);
				int size = 0;
				while (s[size++] != 0);
				size--;
				rectangle(preCharRihgt, top, preCharRihgt + size * length, top + width);
				//画一个矩形显示数据
				outtextxy(preCharRihgt + 2, top + 2, s);
				//显示数据
				totalLength += size * length;
				if (current->branch[i] != nullptr)
				{ //访问当前节点的孩子节点
					qBn.push(current->branch[i]); //把孩子节点放入队列
					qLeft.push(rowLeft);
					qTop.push(top + width + margin);
					POINT pts[] = { {preCharRihgt, top + width}, {rowLeft, top + width + margin} };//两个点的坐标
					polyline(pts, 2); //画连线
					rowLeft += current->branch[i]->keyNum * 20 + margin;
				}
				preCharRihgt = preCharRihgt + size * length;
			}
			if (current->branch[current->keyNum] != nullptr)
			{
				qBn.push(current->branch[current->keyNum]);
				qLeft.push(rowLeft);
				qTop.push(top + width + margin);
				POINT pts[] = { {preCharRihgt, top + width}, {rowLeft, top + width + margin} };
				polyline(pts, 2);
				rowLeft += current->branch[current->keyNum]->keyNum * 20 + margin;
			}
		}
	}

};



bool initBTree(BTree &b,int count)
{
	//初始化
	cout << "请输入关键字，以逗号分隔" << endl;
	int num; //用来存放关键字
	int count2 = 0;//用来判断是否输入超过count
	string s;
	string ss = "";
	cin >> s;
	int i = 0;
	
	while (i < s.length())
	{
		bool flag = true; 
		while (s[i] != ',')
		{
			if (i == s.length())
				break;
			if (s[i] < '0' || s[i]>'9')
			{
				flag = false;
			}

			ss += s[i];
			i++;
		}
		if (flag == false)
		{
			cout << "输入的数据类型错误,忽略此输入！" << endl;
			ss = "";
			i++;
			continue;
		}
		count2++;
		num = atoi(ss.c_str());
		if (count2 > count)
		{
			break;
		}
		Error_code e = b.insert(num);
		if (e == duplicate_error)
		{
			cout << "有重复的关键字，忽略此输入" << endl;
			count2--;
		}
		
		ss = "";
		i++;
	}
	cout << "建树成功" << endl;
	b.show();
	return true;
}
bool inOrDe_implement(BTree& b, string tag)
{
	if (tag == "I")
	{
		int num;
		cin >> num;
		Error_code e = b.insert(num);
		if (e == duplicate_error)
		{
			cout << "有重复的关键字" << endl;
			b.show();
			return false;
		}
		b.show();
		return true;
	}
	if (tag == "D")
	{
		int target;
		cin >> target;
		Error_code e = b.remove(target);
		if (e == not_present)
		{
			cout << "未找到相应的关键字" << endl;
			b.show();
			return false;
		}
		b.show();
		return true;
	}
	if (tag == "MI")
	{
		string s;
		cin >> s;
		int num;
		string ss = "";
		int i = 0;
		int size = s.length();
		int flag2 = false;
		while (i<size-1)
		{
			while (s[i] != ',')
			{
				if (i>=size-1)
					break;
				
				if ((s[i] < '0' || s[i]>'9')&&(s[i]!='-'))
				{
					cout << "输入的数据类型错误！" << endl;
					return false;
				}

				ss += s[i];
				i++;
			}
			num = atoi(ss.c_str());
			Error_code e = b.insert(num);
			if (e == duplicate_error)
			{
				cout << "有重复的关键字" << endl;
				b.show();
			}
			else
			{
				b.show();
			}
			ss = "";
			i++;
		}
		return true;
	}
	if (tag == "MD")
	{
		string s;
		cin >> s;
		int target;
		string ss = "";
		int i = 0;
		int size = s.length();
		while (i < size - 1)
		{
			while (s[i] != ',')
			{
				if (i>=size-1)
					break;
				if ((s[i] < '0' || s[i]>'9') && (s[i] != '-'))
				{
					cout << "输入的数据类型错误！" << endl;
					return false;
				}

				ss += s[i];
				i++;
			}
			target = atoi(ss.c_str());
			Error_code e = b.remove(target);
			if (e == not_present)
			{
				cout << "未找到相应的关键字" << endl;
				b.show();
			}
			else
			{
				b.show();
			}
			ss = "";
			i++;
		}
		return true;
	}
}
void inOrDe(BTree &b)
{
	//cout << "yes" << endl;
	//插入或删除
	string tag; //插入或删除或连续操作的标志
	cout << "进行插入或者删除操作：" << endl;
	cout << "格式要求：" << endl;
	cout << "I data  表示插入data" << endl;
	cout << "D data  表示删除data" << endl;
	cout << "可以连续进行插入操作或删除操作，也可以交替进行插入和删除操作" << endl;
	cout << "MI data1,data2,...,datan#  表示插入n个关键字，以逗号分隔，井号结束" << endl;
	cout << "MD data1,data2,...,datan#  表示删除n个关键字，以逗号分隔，井号结束" << endl;
	cout << "请输入希望操作的次数：" << endl;
	int count;
	cin >> count;
	bool flag2 = true;
	for (int i = 0; i <= count; i++)
	{
		if (i == count)
		{
			cout << "操作结束" << endl;
		}
		else
		{
			cout << "BTree>";
			cin >> tag;
			flag2 = inOrDe_implement(b, tag);
		}
		
	}
	
}



int main()
{
	initgraph(900, 550, SHOWCONSOLE);
	cout << "建立B树，请输入关键字的个数和B树的阶数，以空格分隔" << endl;
	int count;
	int order;
	cin >> count >> order;
	BTree btree(order);
	initBTree(btree,count);
	inOrDe(btree);
	_getch(); //让窗口先保持
	closegraph();
	return 0;
}
