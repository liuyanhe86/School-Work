#include <iostream>
#include <cmath>

using namespace std;

//��ṹ��
struct Term
{ 
	int coef;
	int exp;
	Term Set(int c, int e) { coef = c; exp = e; return *this; };
};

//ǰ������
class CircularListWithHead;

//�ڵ㶨��
class ChainNode {
	friend class CircularListWithHead;
	friend class ChainIterator;
public:
	ChainNode(Term element=Term(), ChainNode* next = 0)
	{
		data = element; link = next;
	}
private:
	Term data;
	ChainNode *link;
};

//����������
class ChainIterator {
public:
	ChainIterator(ChainNode* startNode = 0)
	{
		current = startNode;
	}
	Term& operator *() const { return current->data; }
	Term* operator ->() const { return &current->data; }
	ChainIterator& operator ++() // preincrement
	{
		current = current->link;
		return *this;
	}
	ChainIterator& operator ++(int) // postincrement
	{
		ChainIterator old = *this;
		current = current->link;
		return old;
	}
	// equality testing
	bool operator !=(const ChainIterator right) const
	{
		return current != right.current;
	}
	bool operator == (const ChainIterator right) const
	{
		return current == right.current;
	}
private:
	ChainNode* current;
};

//ӵ��ͷ�ڵ��ѭ������
class CircularListWithHead {
public:
	CircularListWithHead(){
		head = new ChainNode(Term().Set(0, -1));
		head->link=head;
	}
	CircularListWithHead operator=(CircularListWithHead a);
	ChainIterator begin()const { return ChainIterator(head); }
	void InsertBack(const Term&);
private:
	ChainNode *head;
};

//ѭ������ֵ����
CircularListWithHead CircularListWithHead::operator=(CircularListWithHead a)
{
	ChainNode* old = head->link;
	while (old != head)
	{
		ChainNode*p = old;
		old = old->link;
		delete p;
	}
	head = new ChainNode(Term().Set(a.head->data.coef, a.head->data.exp));
	head->link = head;
	delete old;
	ChainNode *temp = a.head->link;
	while (temp != a.head)
	{
		InsertBack(Term().Set(temp->data.coef, temp->data.exp));
		temp = temp->link;
	}
	return *this;
}

//�����½ڵ㵽���
void CircularListWithHead::InsertBack(const Term& e)
{
	if (head->data.exp!=-1&&head->link!=head) { // nonempty chain
		ChainNode* temp = head->link;
		while (temp!=head)
		{
			ChainNode* mark = temp;
			while (mark != head)
			{
				if (mark->data.exp == e.exp)
				{
					mark->data.coef += e.coef;
					return;
				}
				else
				{
					temp = mark;
					mark = mark->link;
				}
			}
			if (temp->link == head)
			{
				temp->link = new ChainNode(e);
				temp->link->link = head;
				return;
			}
			else
				temp = temp->link;
		}
	}
	else if(head->data.exp==-1){
		head = new ChainNode(e);
		head->link=head;
	}
	else if(head->link==head)
	{
		head->link = new ChainNode(e);
		head->link->link = head;
	}
}

//����ʽ����
class Polynomial {
	friend istream& operator>>(istream& is, Polynomial& x);
	friend ostream& operator<<(ostream& os, Polynomial& x);
public:
	Polynomial() { poly = CircularListWithHead(); }
	Polynomial(const Polynomial& a) { poly = a.poly; }
	const Polynomial& operator=(const Polynomial& a);
	~Polynomial() {}
	Polynomial operator+(const Polynomial& b)const;
	Polynomial operator-(const Polynomial& b)const;
	Polynomial operator*(const Polynomial& b)const;
	float Evaluate(float x);
private:
	CircularListWithHead poly;
};

//����ʽ��ֵ
const Polynomial & Polynomial::operator=(const Polynomial & a)
{
	poly = a.poly;
	return *this;
}

//����ʽ���
Polynomial Polynomial::operator + (const Polynomial& b) const
{ 
	Term temp;
	ChainIterator ai = poly.begin(),bi = b.poly.begin();
	Polynomial c; //assume constructor sets head��exp = -1
	while(1){
		if (ai->exp == bi->exp) {
			if (ai->exp == -1) return c;
			int sum = ai->coef + bi->coef;
			if (sum) c.poly.InsertBack(temp.Set(sum, ai->exp));
			ai++; bi++; // to next term
		}
		else if (ai->exp < bi->exp) {
			c.poly.InsertBack(temp.Set(bi->coef, bi->exp));
			bi++; // next term of b
		}
		else {
			c.poly.InsertBack(temp.Set(ai->coef, ai->exp));
			ai++; // next term of a
		}
		if (ai == poly.begin() && bi == b.poly.begin())
			break;
	} 
	return c;
}

//����ʽ���
Polynomial Polynomial::operator-(const Polynomial & b)const
{
	Term temp;
	ChainIterator ai = poly.begin(), bi = b.poly.begin();
	Polynomial c; //assume constructor sets head��exp = -1
	while (1) {
		if (ai->exp == bi->exp) {
			if (ai->exp == -1) return c;
			int sum = ai->coef - bi->coef;
			if (sum) c.poly.InsertBack(temp.Set(sum, ai->exp));
			ai++; bi++; // to next term
		}
		else if (ai->exp < bi->exp) {
			c.poly.InsertBack(temp.Set(bi->coef, bi->exp));
			bi++; // next term of b
		}
		else {
			c.poly.InsertBack(temp.Set(ai->coef, ai->exp));
			ai++; // next term of a
		}
		if (ai == poly.begin() && bi == b.poly.begin())
			break;
	}
	return c;
}

//����ʽ���
Polynomial Polynomial::operator*(const Polynomial & b)const
{
	Term term;
	Polynomial c,d;
	ChainIterator ai = poly.begin(), bi = b.poly.begin();
	if (ai->exp == bi->exp)
		if (ai->exp == -1) return c;
	while (1)
	{
		while (1)
		{
			int coef = ai->coef*bi->coef;
			int exp = ai->exp + bi->exp;
			c.poly.InsertBack(term.Set(coef, exp));
			++bi;
			if (bi == b.poly.begin())
				break;
		}
		++ai;
		if (ai == poly.begin())
			return c;
	}
}

//��ֵ
float Polynomial::Evaluate(float x)
{
	ChainIterator it = poly.begin();
	float result = 0.0;
	do
	{
		result += it->coef*pow(x, it->exp);
		it++;
	} while (it!=poly.begin());
	return result;
}

//�������������
istream & operator>>(istream & is, Polynomial & x)
{
	int n = 0,c = 0,e = 0;
	is >> n;
	for (int i = 0; i < n; ++i)
	{
		is >> c >> e;
		x.poly.InsertBack(Term().Set(c,e));
	}
	return is;
}

//������������
ostream & operator<<(ostream & os, Polynomial & x)
{
	ChainIterator it = x.poly.begin();
	if (it->exp != -1)
	{
		cout << it->coef << "x^" << it->exp;
		++it;
		while (it != x.poly.begin())
		{
			if (it->exp == 1)
				cout << '+' << it->coef << 'x';
			else if (it->exp == 0)
				cout << '+' << it->coef;
			else if (it->coef == 1)
				cout << '+' << "x^" << it->exp;
			else
				cout << '+' << it->coef << "x^" << it->exp;
			++it;
		}
		cout << endl;
	}
	else
		cout << 0 << endl;
	return os;
}

//���Ժ���
int main()
{
	cout << "������һ������ʽ������������һ��ϵ������һ��ָ�����ڶ���ϵ�������������һ��ָ����" << endl;
	Polynomial p;
	cin >> p;
	cout << "����Ķ���ʽΪ��"<<p << endl;
	Polynomial q(p);
	cout <<"�ÿ������췽�������Ķ���ʽ����Ϊ��"<< q << endl;
	Polynomial r = p + q;
	cout << "������ʽ֮��Ϊ��"<<r << endl;
	Polynomial s = p - q;
	cout << "������ʽ֮��Ϊ��" << s<<endl;
	Polynomial t = p * q;
	cout << "������ʽ�˻�Ϊ��" << t << endl;
	float x = 1.2;
	x=p.Evaluate(x);
	cout <<"��x=1.2���������Ķ���ʽ��ֵΪ��"<< x << endl;
	system("pause");
}
