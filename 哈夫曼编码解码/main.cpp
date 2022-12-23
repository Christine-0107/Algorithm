#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include<cstring>
using namespace std;
//����������
class List;
class Node
{
	friend class List;
public:
	char c;
	int num;
	Node* next;
public:
	Node() { next = nullptr; }
	Node(char s) { c = s; num = 1; next = nullptr; }
};
class List
{
protected:
	Node* head;
public:
	List();
	Node* getHead();
	int findElem(char s);
	void insert(char s);
	
};
List::List()
{
	head = new Node();
}
Node* List::getHead()
{
	return head;
}
int List::findElem(char s)
{
	Node* p = head->next;
	while (p != nullptr)
	{
		if (p->c == s)
		{
			p->num++;
			return 1;
		}
		p = p->next;
	}
	return 0;
}
void List::insert(char s)
{
	Node* new_node;
	new_node = new Node(s);
	new_node->next = head->next;
	head->next = new_node;
}


class HTree;
class HNode
{
	friend class HTree;
public:
	int left=-1, right=-1;
	int parent = -1;
	int weight = 1000;
	char data=' ';
public:
	HNode() {}
	HNode(char d, int w);//��ʼ������ʱ�Ĺ��캯��
	
};
//HNode�ĺ���
HNode::HNode(char d, int w)
{
	data = d;
	weight = w;
}

class HTree
{
public:
	int n;
	HNode** tree;
public:
	HTree() {}
	HTree(int size);
	void create(List l);
	void searchMin(int& min1, int& min2);
	void createHTree();

};
//HTree�ĺ���
HTree::HTree(int size)
{
	n = size;
	tree = new HNode * [2 * n - 1];
}
void HTree::create(List l)
{
	Node* p = l.getHead()->next;
	for (int i = 0; i < n; i++)
	{
		tree[i] = new HNode(p->c, p->num);
		p = p->next;
	}
	for (int i = n; i < 2 * n - 1; i++)
	{
		tree[i] = new HNode();
	}
}
void HTree::searchMin(int& min1, int& min2)
{
	for (int i = 0; i < 2 * n - 1; i++)
	{
		if (tree[i]->parent == -1)
		{
			min1 = i;
			break;
		}
	}
	for (int i = 0; i < 2 * n - 1; i++)
	{
		if (tree[i]->parent == -1 && tree[i]->weight < tree[min1]->weight)
		{
			min1 = i;
		}
	}
	for (int i = 0; i < 2 * n - 1; i++)
	{
		if (tree[i]->parent == -1 && i != min1)
		{
			min2 = i;
			break;
		}
	}
	for (int i = 0; i < 2 * n - 1; i++)
	{
		if (tree[i]->parent == -1 && tree[i]->weight < tree[min2]->weight && i != min1)
		{
			min2 = i;
		}
	}
	int temp;
	if (tree[min1]->weight > tree[min2]->weight)
	{
		temp = min1;
		min1 = min2;
		min2 = temp;
	}
}
void HTree::createHTree()
{
	int minPos1, minPos2;
	for (int i = 0; i < n - 1; i++)//��Ҫ����n-1�κϲ�����
	{
		searchMin(minPos1, minPos2);
		tree[i + n]->weight = tree[minPos1]->weight + tree[minPos2]->weight;
		tree[i + n]->left = minPos1;
		tree[i + n]->right = minPos2;
		tree[minPos1]->parent = i + n;
		tree[minPos2]->parent = i + n;
	}
}


class HCode
{
public:
	int n;
	char** code;
public:
	HCode() {}
	HCode(int size);
	void createCode(HTree H);
};
HCode::HCode(int size)
{
	n = size;
	code = new char* [n]; //��Ӧn���ַ��ı���
}
void HCode::createCode(HTree H) //��������
{
	char* temp = new char[n];//����һ����ʱ��char�����飬�洢ĳ���ַ��ı���
	temp[n - 1] = '\0';//�ַ���ĩβΪ\0,�Ӻ���ǰ��������
	for (int i = 0; i < n; i++)//���δ���ÿ���ַ�
	{
		int start = n - 1;
		int c = i;
		int f = H.tree[i]->parent;
		while (f != -1)//���ݵ����ڵ�Ϊֹ
		{
			start--;
			if (c == H.tree[f]->left)
				temp[start] = '0';
			else
				temp[start] = '1';
			c = f;
			f = H.tree[f]->parent;//���ϻ���
		}
		code[i] = new char[n - start];
		for (int j = start; j <= n - 1; j++)
		{
			code[i][j - start] = temp[j];
		}
	}
}


int main()
{
	//�����ļ�һ
	List doc1;//Ϊ�ļ�һ���ַ���������
	Node* head;
	head = doc1.getHead();
	string file1 = "inputfile1.txt";
	ifstream input1(file1);
	if (!input1)
	{
		cout << "��ʧ��" << endl;
		return 0;
	}
	char c;
	int flag;
	for(;;)
	{
		input1.get(c);
		if (input1.eof())
			break;
		flag = doc1.findElem(c); //�ҵ�������+1
		if (flag == 0)
		{
			doc1.insert(c); //û�ҵ��Ͳ���
		}
	}
	input1.close();
	int size = 0;
	Node* p = head;
	while (p->next != nullptr)
	{
		size++;
		p = p->next;
	} //��������Ĵ�С

	HTree hufftree(size);
	hufftree.create(doc1);
	hufftree.createHTree();
	HCode huffcode(size);
	huffcode.createCode(hufftree);

	//"outputfile1.txt"������ַ����ֵĴ����Ͷ�Ӧ�ı���
	ofstream output1("outputfile1.txt");
	if (!output1)
	{
		cout << "��ʧ��" << endl;
		return 0;
	}
	output1 << setw(6) << setiosflags(ios::left) << "�ַ�";
	output1 << setw(10) << setiosflags(ios::left) << "���ִ���";
	output1 << setw(50) << setiosflags(ios::left) << "��Ӧ�ı���" << endl;
	Node* q = head->next;
	for (int i = 0; i < size; i++)
	{
		output1 << setw(10) << setiosflags(ios::left) << q->c;
		output1 << setw(14) << setiosflags(ios::left) << q->num;
		output1 << setw(54) << setiosflags(ios::left) << huffcode.code[i] << endl;
		q = q->next;
	}
	output1.close();

	//ʹ��map�����ַ��ͱ���Ķ�Ӧ
	std::map<char, char*> mapcode;
	for (int i = 0; i < size; i++)
	{
		mapcode.insert(pair<char, char*>(hufftree.tree[i]->data,huffcode.code[i]));
	}

	ifstream input2("inputfile2.txt");
	if (!input2)
	{
		cout << "��ʧ��" << endl;
		return 0;
	}
	ofstream output2("outputfile.bin", ios::binary);
	if (!output2)
	{
		cout << "��ʧ��" << endl;
		return 0;
	}
	string totals = "";
	string total = "";
	
	

	for(;;)
	{
		input2.get(c);
		if (input2.eof())
			break;
		map<char, char*>::iterator key = mapcode.find(c);
		if (key != mapcode.end())
		{
			totals += key->second;
		}
		if (totals.length() > 8)
		{
			string temp = "";
			temp = temp.append(totals.begin() + 8, totals.end());
			//��totals�г���8λ�Ĳ��ָ���temp
			int n = 0;
			unsigned char d;
			for (int k = 0; k < 8; k++)
			{
				n = (n << 1) + (int)totals[k] - 48;
				d = (unsigned char)n;
				
			}
			output2.write((char*)&d, 1);
			n = 0;
			totals = temp;
			continue;
		}
		else if (totals.length() == 8)
		{
			int n = 0;
			unsigned char d;
			for (int k = 0; k < 8; k++)
			{
				n = (n << 1) + (int)totals[k] - 48;
				d = (unsigned char)n;
				
			}
			output2.write((char*)&d, 1);
			n = 0;
			totals = "";
			continue;
		}
		else if (totals.length() < 8)
		{
			continue;
		}
	}
	int len = totals.length();
	if (len != 0)
	{
		totals = totals.append("0");
		int m = 8 - totals.length();
		for (int i = 0; i < m; i++)
		{
			totals = totals.append("0");
		}
		int n = 0;
		unsigned char d;
		for (int k = 0; k < 8; k++)
		{
			n = (n << 1) + (int)totals[k] - 48;
			d = (unsigned char)n;
			
		}
		output2.write((char*)&d, 1);
		n = 0;
	}
	cout << len << endl;
	output2.write((char*)&len, sizeof(char));
	input2.close();
	output2.close();
	

	ifstream input3("outputfile.bin", ios::binary);
	if (!input3)
	{
		cout << "��ʧ��" << endl;
		return 0;
	}
	string totals2 = "";
	string totals3 = "";
	unsigned char e;
	
	while (input3.read((char*)&e, 1))
	{
		string s = "abcdefgh";
		for (int i = 7; i >= 0; i--)
		{
			s[i] = char(((e >> (7 - i)) & 1) + 48);
		}
		totals2.append(s);
	}
	cout << totals2 << endl;
	
	input3.close();
	int len2 = totals2.length();
	int sum = 0;
	int co = 0;
	for (int x = len2 - 8; x < len2; x++)
	{
		if (totals2[x] == '1')
		{
			int y = pow(2, 7 - co);
			sum += y;
		}
		co++;
	}
	cout << sum << endl;
	totals2[len2 - (8-sum)-8] = '\0'; //����Ϊ����һ��char�����ϵļ�ȥ
	//cout << totals2 << endl;
	//cout << endl;
	//cout << len << endl;
	string destr = "";//�����������ַ���
	int root;
	for (int i = 0; i < 2 * size - 1; i++)
	{
		if (hufftree.tree[i]->parent == -1)
		{
			root = i;
			break;
		}
	}
	int count = 0;
	char cc;
	int tag = root;
	for (int i = 0; i < len2-8-(8-sum); i++)
	{
		cc = totals2[i];
		cout << cc;
		
		if (cc == '0')
		{
			if (hufftree.tree[tag]->left <size)
			{
				int l = hufftree.tree[tag]->left;
				destr+= hufftree.tree[l]->data;
				count++;
				tag = root;
				continue;
			}
			else
			{
				tag = hufftree.tree[tag]->left;
				continue;
			}
		}
		else if (cc == '1')
		{
			if (hufftree.tree[tag]->right <size)
			{
				int r = hufftree.tree[tag]->right;
				destr += hufftree.tree[r]->data;
				count++;
				tag = root;
				continue;
			}
			else
			{
				tag = hufftree.tree[tag]->right;
				continue;
			}
		}
	}
	destr[count] = '\0';
	//cout << destr << endl;

	ofstream output3("outputfile2.txt");
	if (!output3)
	{
		cout << "��ʧ��" << endl;
		return 0;
	}
	output3 << destr;
	output3.close();

	return 0;

}