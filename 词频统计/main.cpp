#include <iostream>
#include <fstream>
#include <map>
using namespace std;

//定义链表类
class List;
class Node
{
	friend class List;
public:
	string word;
	int num;
	Node* next;
public:
	Node() { next = nullptr; }
	Node(string s) { word = s; num = 1; next = nullptr; }
};
class List
{
protected:
	Node* head;
public:
	List();
	Node* getHead();
	int findElem(string s);
	bool insert(string s);
	void display();
};
List::List()
{
	head = new Node();
}
Node* List::getHead()
{
	return head;
}
int List::findElem(string s)
{
	Node* p = head;
	while (p != nullptr)
	{
		if (p->word == s)
		{
			p->num++;
			return 1;
		}
		p = p->next;
	}
	return 0;
}
bool List::insert(string s)
{
	Node* p = head;
	Node* temp;
	temp = new Node(s);
	while (p != nullptr)
	{
		if (p->next != nullptr)
		{
			if (s > p->word && s < p->next->word)
			{
				temp->next = p->next;
				p->next = temp;
				return true;
			}
			else
			{
				p = p->next;
			}
		}
		else
		{
			if (s > p->word)
			{
				p->next = temp;
				temp->next = nullptr;
				return true;
			}
			else
				return false;
		}
	}
}
void List::display()
{
	Node* p = head->next;
	if (p == nullptr)
	{
		cout << "无" << endl;
		return;
	}
	else
	{
		while (p->next != nullptr)
		{
			cout << p->word << ":" << p->num << endl;
			p = p->next;
		}
		cout << p->word << ":" << p->num << endl;
	}
}

//定义存放表头指针的数组
Node* h[26];

int main()
{
	List wordfre[26];
	for (int i = 0; i < 26; i++)
	{
		h[i] = wordfre[i].getHead();
	}
	std::map<string, bool> mapword;//用map来匹配停词表中单词
	string filestopword = "停词表.txt";
	ifstream input1(filestopword);
	if (!input1)
	{
		cout << "打开失败" << endl;
		return 0;
	}
	string stopword;
	while (!input1.eof())
	{
		input1 >> stopword;
		mapword.insert(pair<string, bool>(stopword, 1));
	}
	input1.close();
	string filename = "HarryPotter.txt";
	ifstream input(filename);
	if (!input)
	{
		cout << "打开失败" << endl;
		return 0;
	}
	char c;
	string s;
	char first;
	int flag;
	char str[100];
	int i = 0;
	bool flag2;
	int flag3;
	while (!input.eof())
	{
		input.get(c);
		if (c >= 'A' && c <= 'Z')
		{
			c = c + 'a' - 'A';
		}
		if ((c >= 'a' && c <= 'z') || (c == '\''))
		{
			if (i == 0 && c == '\'')
			{
				continue;
			}
			str[i] = c;
			i++;
		}
		else
		{
			str[i] = '\0';
			if (i==0)
			{
				continue;
			}
			map<string, bool>::iterator key = mapword.find(str);
			if (key != mapword.end())
			{
				continue;
			}
			s = str;
			first = s[0];
			flag = wordfre[(int)(first - 'a')].findElem(s);
			if (flag == 0)
			{
				flag2=wordfre[(int)(first - 'a')].insert(s);
				//wordfre[(int)(first - 'a')].display();
			}
			i = 0;
		}
		
	}
	input.close();
	//按照词频降序排列
	for (int m = 0; m < 26; m++)
	{
		int len = 0;
		Node* ptr = h[m];
		while (ptr->next != nullptr)
		{
			ptr = ptr->next;
			len++;
		}
		for (int i = 0; i < len - 1; i++)
		{
			Node* ph = h[m];
			Node* p = ph->next;
			Node* q = p->next;
			for (int j = 0; j < len - 1 - i; j++)
			{
				if (p->num < q->num)
				{
					ph->next = q;
					p->next = q->next;
					q->next = p;
					Node* temp = p;
					p = q;
					q = temp;
				}
				ph = ph->next;
				p = p->next;
				q = q->next;
			}
		}
		cout << "首字母为" << (char)(m + 'a') << "的单词：" << endl;
		wordfre[m].display();
		cout << endl;
	}
	
	
	return 0;
}