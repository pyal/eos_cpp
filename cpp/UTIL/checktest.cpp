

#include "lib/ref/file_manip.h"
#include "lib/std/ex_out.h"
#include "lib\ref\commandline.h"

double StndErr=1e-6;



Stroka GenerateDetailedHelp() {
    Stroka ret = Stroka("Program build time ") + __TIMESTAMP__ + "\n";
    return ret;
};


void PrintHelp(map<Stroka, Stroka> par) {
    cout << GenerateDetailedHelp();
}


int CheckDuplicateChars(const string &str) {
	vector<int> test(256,0);
	const char *ptr = str.c_str();
	for(size_t i = 0; i < str.length(); i++)
		if (test[ptr[i]]++ != 0)
			return 1;
	return 0;
}
boolean hasDuplicateChars(const string &str) {
	boolean test[256];
	for(size_t i = 0; i < str.length(); i++) 
		test[str.at(i)] = false;
	for(size_t i = 0; i < str.length(); i++) {
		char val = str.at(i);
		if (test[val])
			return true;
		test[val] = true;
	}
	return false;
}
boolean hasDuplicateChars1(const string &str) {
	int t = 0;
	for(size_t i = 0; i < str.length(); i++) {
		int val = str.at(i) - 'a';
		if (t & (1>>val))
			return true;
		t = t | (1>>val);
	}
	return false;
}
void reverseString(char *str) {
	if (!str)
		return;
	int l = strlen(str);
	for(size_t i = 0; i < size_t(l / 2); i++) 
		swap(str[i], str[l - i - 1]);
}
void removeDuplicate(char *str) {
	char prev = 0, *pos = 0;
	if (!str) 
		return;
	for(size_t i = 0; i < strlen(str); i++) {
		if (prev && prev == str[i]) {
			if (!pos)
				pos = &str[i];
		} else
			if (pos) {
				*pos = str[i];
				pos++;
		}
		prev = str[i];
	}
	if (pos)
		*pos = 0;
}

void ChekDuplicateChars(map<Stroka, Stroka> par) {
	if (hasDuplicateChars1(string(par["Str"].c_str())))
		cout << "Duplicates present\n";
	else
		cout << "No duplicates\n";
}
void ReverseString(map<Stroka, Stroka> par) {
	string reverse((par["Str"].c_str()));
	reverseString((char*)reverse.c_str());
	cout << " Str [" << par["Str"] << "] reverse [" << reverse.c_str() << "]\n";
}
void Check(const char *in, const char *test) {
	if (in == NULL) {
		char *t = NULL;
		removeDuplicate(t);
		if (t !=NULL)
			throw exception("Have  to be null but is not");
		return;
	}

	string buf(in);
	char *t = (char*)buf.c_str();
	removeDuplicate(t);
	if (strcmp(t, test) != 0)
		throw exception(~(Stroka("test failed for str ") + in + " result " + t + " test " + test + "\n"));
}
void RemoveDuplicate(map<Stroka, Stroka> par) {
	string uniq((par["Str"].c_str()));
	removeDuplicate((char*)uniq.c_str());
	cout << " Str [" << par["Str"] << "] uniq [" << uniq.c_str() << "]\n";
	Check("aaa","a");
	Check("assddd","asd");
	Check("adfd","adf");
}
void makeStrStatistics(const char *str, vector<char> &stat) {
	size_t l = strlen(str);
	for(size_t i = 0; i < l; i++)
		stat[str[i]]++;
}
boolean isAnagram(const char *str1, const char *str2) {
	if (!str1 || !str2)
		return (str2?false:true);
	if (strlen(str1) != strlen(str2))
		return false;
	vector<char> stat1(256, 0), stat2(256, 0);
	makeStrStatistics(str1, stat1);
	makeStrStatistics(str2, stat2);
	for(size_t i = 0; i < stat1.size(); i++)
		if (stat1[i] != stat2[i])
			return false;
	return true;
}

#include <algorithm>

boolean isAnagram2(const char *str1, const char *str2) {
	std::string srt1(str1), srt2(str2);
	std::sort(srt1.begin(), srt1.end());
	std::sort(srt2.begin(), srt2.end());
	return srt1.compare(srt2) == 0;

}
void IsAnagram(map<Stroka, Stroka> par) {
	if (isAnagram2(~par["Str1"], ~par["Str2"]))
		cout << "String " << par["Str1"] << " and " << par["Str2"] << " are anagrams\n";
	else
		cout << "String " << par["Str1"] << " and " << par["Str2"] << " are not anagrams\n";
}


double pow_up(double a, int b) {
    if (b == 0)
        return 1;
    double mulFactor = a, ans = 1;
    if (b < 0) {
        b = -b;
        mulFactor = 1./a;
    }
    int curPower = 1;
//cout << a << " " << b << "\n";
    while(curPower <=b) {
        if (curPower & b)
            ans *= mulFactor;
        mulFactor *= mulFactor;
//cout << ans << " " << mulFactor << " " << curPower << "\n";
        curPower = curPower << 1;
    } 
    return ans;

}

void Pow(map<Stroka, Stroka> par) {
	double a = atof(~par["X"]);
	int b = atof(~par["Pow"]);
	double p = pow_up(a, b), t = pow(a, b);
	cout << a << " ** " << b << " = " << p
		 << " cmp " << t << " delta " << p - t << "\n";
}
template <class T> 
bool isPolymorphic() { 
    bool answer=false;
    T *t = new T();
    typeid(answer=true,*t); 
    delete t;
    return answer; 
}
template <class T1, class T> 
T1* myCast(T* data) { 
    T1 *ret = NULL;
    T *t = new T();
    typeid(ret=static_cast<T1*>(data), *t); 
    //ret=static_cast<T1*>(data); 
    delete t;
    return ret; 
}

class PolyBase {
public:
   virtual void func(){}
};

class NPolyBase {
public:
	void func(){}
   //~NBase(){}
};

void Polymorphic(map<Stroka, Stroka> par) {
	//int i;
	//PolyBase pb;
	//NPolyBase npb;
	//cout << " PolyBase isPolymorphic " << myCast<PolyBase, PolyBase>(&pb) << "\n";
	//cout << " NPolyBase isPolymorphic " << myCast<PolyBase, NPolyBase>(&npb) << "\n";
	//cout << " Int isPolymorphic " << myCast<PolyBase, int>(&i) << "\n";
}

//#include <list>
//struct SortedList {
//	list<double> Data;
//	int TailSize;
//	SortedList() : TailSize(0){}
//	static void ChainCheck(list<SortedList> &history) {
//		SortedList &front = history.front();
//		double val = front.Data.front();
//		while(history.size() > 1) {
//			list<SortedList>::iterator it = history.begin();
//			it++;
//			SortedList &prev = *it;
//			if (val > prev.Data.back())
//				break;
//			front.TailSize += prev.TailSize + prev.Data.size();
//			history.erase(it);
//		}
//	}
//	static void AddHead(list<SortedList> &history, double val) {
//		if (history.empty()) {
//			history.push_front(SortedList());
//			history.front().Data.push_front(val);
//			return;
//		}
//		SortedList &head = history.front();
//		if (val > head.Data.front()) {
//			head.Data.push_front(val);
//			return;
//		}
//		if (head.Data.size() == 1) {
//			head.Data.front() = val;
//			head.TailSize++;
//			ChainCheck(history);
//			return;
//		}
//		history.push_front(SortedList());
//		SortedList &newTop = history.front();
//		newTop.Data.push_front(val);
//		ChainCheck(history);
//		
//	}
//	static double GetMin(list<SortedList> &history) {
//		return history.back().Data.back();
//	}
//	static void RemoveTail(list<SortedList> &history) {
//		SortedList &back = history.back();
//		if (back.TailSize > 0) {
//			back.TailSize--;
//			return;
//		}
//		back.Data.pop_back();
//		if (back.Data.size()==0) {
//			history.pop_back();
//			return;
//		}
//		if (history.size() < 2)
//			return;
//		SortedList &next = *(history.begin()++);
//		if (back.Data.back() < next.Data.back())
//			return;
//		next.TailSize += back.Data.size();
//		history.pop_back();
//	}
//};
//
//
//double FindMaxKMin(vector<double> data, int K) {
//	list<SortedList> kTail;
//	double fMax = -1e100;
//	for(size_t i = 0; i < data.size(); i++) {
//		SortedList::AddHead(kTail, data[i]);
//		if ((int)i < K - 1) {
//			continue;
//		}
//		cout << i << " " << data[i] << " ";
//		double curMin = SortedList::GetMin(kTail);
//		cout << curMin << "\n";
//		if (curMin > fMax)
//			fMax = curMin;
//		SortedList::RemoveTail(kTail);
//	}
//	return fMax;
//}
//
//void max_mink(map<Stroka, Stroka> par) {
//	vector<vector<double > > vec;
//	int k = atoi(~par["K"]);
//	File::ReadFile(~par["Data"], vec, "Cannot read file", 1);
//	cout << "found max min from k= " << k << " max= " << FindMaxKMin(vec[1], k) << "\n";
//}

int ClcNumRoutes(int x, int y) {
    if (x == 0 || y == 0)
        return 1;
    return ClcNumRoutes(x-1,y) + ClcNumRoutes(x, y-1);
}

double Fact(int n) {
    if (n <=1)
        return 1;
    return n * Fact(n-1);
}

int ClcNumRoutes(int n) {
    return ClcNumRoutes(n, n);
}

void TestR(int n) {
    double exp = ClcNumRoutes(n,n);
    double strict = Fact(2*n)/sqr(Fact(n));
    if ( exp != strict )
        cout << " Test " << n << " robot " << exp << " strict " << strict << "\n";
    else
        cout << n << " OK\n";
}

void testrobot(map<Stroka, Stroka> par) {
    TestR(1);
    TestR(2);
    TestR(5);
}


//#include <string>
//
//struct PrefixTree {
//	map<char, PrefixTree> Childs;
//	void clear() {
//		Childs.clear();
//	}
//	static void AddString(const char *str, PrefixTree &node) {
//		if (strlen(str) == 0)
//			return;
//		if (node.Childs.find(str[0]) == node.Childs.end())
//			node.Childs[str[0]] = PrefixTree();
//		AddString(&str[1], node.Childs[str[0]]);
//	}
//	static void MakeTree(const char *str, PrefixTree &root) {
//		root.clear();
//		for(size_t i = 0; i < strlen(str); i++) {
//			PrefixTree::AddString(&str[i], root);
//		}
//	}
//
//	static int FindMaxSubstr(const char *str, int pos, PrefixTree &root, string &res) {
//		PrefixTree *curPtr = &root;
//		res.clear();
//		for(int i = pos; i >= 0; i--) {
//			if (curPtr->Childs.find(str[i]) == curPtr->Childs.end())
//				break;
//			res.push_back(str[i]);
//			curPtr = &curPtr->Childs[str[i]];
//		}
//		return res.size();
//	}
//};
//
//
//
//void findmaxanagram(map<Stroka, Stroka> par) {
//	PrefixTree tree;
//	const char *str = ~par["Str"];
//	PrefixTree::MakeTree(str, tree);
//	string maxStr, curStr;
//	int maxLen = -1, curLen;
//	for(int i = strlen(str) - 1; i >= 0; i--) {
//		if ((curLen = PrefixTree::FindMaxSubstr(str, i, tree, curStr)) <= maxLen)
//			continue;
//		maxLen = curLen;
//		maxStr = curStr;
//	}
//	cout << "found maxLen " << maxLen << " str " << maxStr.c_str() << "\n";
//}



//int main( int argc, const char *argv[] )
//{
//    SetLeakTest();
//    Time_struct Time;
//    try {
//		NRef::TCommandParse Cmd(~(Stroka("Usage: ")  + argv[0] + " [Params_Key_Spec] [/][-]key  \n without parameters - help\n"));
//        Cmd.MainHelp += GenerateDetailedHelp();
//        Cmd.Add(PrintHelp, "help", "show help?", "");
//        Cmd.Add(ChekDuplicateChars, "duplicate", "Check duplicate strings", "Str abcdea string to check");
//		Cmd.Add(ReverseString, "reverse", "Reverse strings", "Str abcdea string to reverse");
//		Cmd.Add(RemoveDuplicate, "uniq", "Remove duplicate chars", "Str abcdea string to uniq");
//		Cmd.Add(IsAnagram, "anagram", "Check anargram strings", "Str1 abcdea fst\nStr2 dfgf second string");
//		Cmd.Add(Pow, "pow", "Make X to the power", "X 3 \nPow 3 will return X ** Pow");
//		Cmd.Add(Polymorphic, "polymorph", "test if types polymorph", "");
//		Cmd.Add(testrobot, "testrobot", "test if robot is c from m by n", "");
//		Cmd.Add(max_mink, "maxmin", "find maximum from k minimum", "Data pnt.dat file with data(first column\nK 10 the minimum window size");
//		Cmd.Add(findmaxanagram, "maxanag", "find maximum anagram substring", "Str ThesampletextthatcouldbereadedthesameinbothordersArozaupalanalapuazorA");
//
//        Cmd.SimpleRun(argc, argv);
//    } CATCHMAINEXCEPTION(Stroka(Stroka(argv[0]) + " failed ").c_str());
//
//    LeakTest();
//    cout<<"Done in "<<Time<<"\n";
//    return 0;
//}

// best

#include "lib/ref/data_manip.h"

//113121IB

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <cmath>
#include <algorithm>
#include <list>
#include <set>
#include <strstream>
#include <numeric>
#include "math.h"
 
using namespace std;
 
typedef long long l2;
template<class T>
void ReadData(vector<T> &dat, int s = 0) {
	if (!s)
		cin >> s;
	dat.resize(s);
	for(size_t i = 0; i < dat.size(); i++) {
		cin >> dat[i];
	}
}
template<class T>
void PrintVector(vector<T> &dat, const char *sep = NULL) {
	if (!dat.size())
		return;
	for(size_t i = 0; i < dat.size() - 1; i++)
		if (!sep)
			cout << dat[i] << "\t";
		else
			cout << dat[i] << sep;
	cout << dat[dat.size() - 1];
}
template<class T>
void PrintArray(vector<vector<T> > &dat, const char *sep = NULL) {
	for(size_t i = 0; i < dat.size(); i++) {
		PrintVector(dat[i], sep);
		cout << "\n";
	}
	cout << "\n";
}
#include <stack>
struct TAtom {
	enum Types {Const, Op};
	enum Operations {Add, Sub, Mul, Div, Cmp, L, R, NUL, BAD, DIGIT};
	static vector<char> Char2Operation;
	static vector<char> Operation2Char;
	static vector<char> Operation2Prior;
	int Type, Val, Operation, Position;
	TAtom() {}
	void Clc(stack<int> &data) {
		if (Operation==DIGIT) {
			data.push(Val);
			return;
		}
		int n2 = data.top();data.pop();
		//int n1 = data.top();data.pop();
		int n1 = data.top();
		int res;
		switch (Operation) {
			case Add:	res = n1 + n2; break;
			case Sub:	res = n1 - n2; break;
			case Mul:	res = n1 * n2; break;
			case Div:	res = n1 / n2; break;
			case Cmp:	res = n1 == n2; break;
		}
		data.top() = res;
	}
	void Init(string &str, int &pos) {
		Type = Op;
		Operation = NUL;
		Val = 0;
		Position = pos;
		if (pos >= (int)str.length()) {
			Operation = BAD;
			return;
		}
		Operation = Char2Operation[str[pos]];
		if (Operation == DIGIT) { 
			int i = pos + 1;
			while ( i < (int)str.length() && Char2Operation[str[i]] == DIGIT)
				i++;
			string tmp(str.begin() + pos, str.begin() + i);
			Val = atoi(tmp.c_str());
			Type = Const;
			pos = i;
		} else 
			pos++;
		while(pos < (int)str.length() && isspace(str[pos]))
			pos++;
		
	}
	static void InitStatic() {
		Char2Operation.resize(128, NUL);
		Char2Operation['*'] = Mul;
		Char2Operation['/'] = Div;
		Char2Operation['-'] = Sub;
		Char2Operation['+'] = Add;
		Char2Operation['='] = Cmp;
		Char2Operation['('] = L;
		Char2Operation[')'] = R;
		fill(Char2Operation.begin() + '0', Char2Operation.begin() + '9' + 1, DIGIT);
		Operation2Char.resize(DIGIT + 1, 'd');
		Operation2Char[Mul] = '*';
		Operation2Char[Div] = '/';
		Operation2Char[Sub] = '-';
		Operation2Char[Add] = '+';
		Operation2Char[Cmp] = '=';
		Operation2Char[NUL] = '?';
		Operation2Char[L] = '(';
		Operation2Char[R] = ')';
		Operation2Prior.resize(DIGIT + 1, 0);
		Operation2Prior[Mul] = 2;
		Operation2Prior[Div] = 2;
		Operation2Prior[Sub] = 2;
		Operation2Prior[Add] = 2;
		Operation2Prior[Cmp] = 1;
		Operation2Prior[NUL] = 2;
	}
	TAtom(string &str, int &pos) {
		Init(str, pos);
	}
};
vector<char> TAtom::Char2Operation, TAtom::Operation2Char, TAtom::Operation2Prior;

struct TTriad {
	int *Operation;
	int *D1, *D2;
	//TTriad(){};
	TTriad(int *i1, int *i2, int *oper) :Operation(oper), D1(i1), D2(i2), res(-10) {}
	int res;
};

struct TTriadClc {
	vector<TTriad> Triads;
	int *Answer;
	int Result;
	void Init(vector<TAtom> &expr) {
//PrintVector(expr);cout.flush();
		stack<int*> buf;
		Triads.reserve(expr.size());
		for(int i = 0; i < (int)expr.size(); i++) {
			if (expr[i].Operation==TAtom::DIGIT) {
//cout << " pushing " << expr[i].Val << "\n";
				buf.push(&expr[i].Val);
				continue;
			}
			if (expr[i].Operation==TAtom::Cmp) {
//cout << " poping == " << *buf.top() << "\n";
				int *i2 = buf.top();buf.pop();
//cout << " changing == " << *buf.top() << "\n";
				int *i1 = buf.top();
				Answer = i2;
				Result = *i1;
				break;
			}
//cout << " poping " << *buf.top() << "\n";
			int *i2 = buf.top();buf.pop();
//cout << " changing " << *buf.top() << "\n";
			int *i1 = buf.top();
			Triads.push_back(TTriad(i1, i2, &expr[i].Operation));
			buf.top() = &(Triads.back().res);
		}
	}
	int Clc() {
		for(int i = 0; i < (int)Triads.size(); i++) {
			TTriad &cur = Triads[i];
			if (*cur.Operation == 0) {
				cur.res = *cur.D1 + *cur.D2;
			} else if (*cur.Operation == 1) {
				cur.res = *cur.D1 - *cur.D2;
			} else
				cur.res = *cur.D1 * *cur.D2;
//cout << " ++ " <<  *cur.D1 << " " << *cur.Operation << " " <<  *cur.D2 << " = " << cur.res << "\n";
		}
		return (Result == *Answer);
	}
};

ostream& operator<<(ostream &out, TAtom &atom) {
	cout << " Operation  " << atom.Operation << " " << TAtom::Operation2Char[atom.Operation] << " val " << atom.Val << " pos " << atom.Position << "\n";
	return out;
}
struct TCalculator {
	vector<TAtom> Expression;
	vector<int> Permute;
	string Line;
	TCalculator(string &line) {
		Init(line);
	}
	void AddElem(int addPrior, stack<pair<TAtom, int> > &ops, TAtom &el) {
		int curPrior = TAtom::Operation2Prior[el.Operation] + addPrior;
		while(!ops.empty() && curPrior <= ops.top().second){
			Expression.push_back(ops.top().first);
			ops.pop();
		}
		ops.push(pair<TAtom, int>(el, curPrior));
	}
	void Init(string &line) {
		Line = line;
		int pos = 0, prevPos = 0;;
		stack<pair<TAtom, int> > Ops;
		TAtom nul;nul.Operation = TAtom::NUL;nul.Val = 0;
		TAtom el(line, pos);
		if (el.Operation == TAtom::NUL)
			el.Init(line, pos);
		int addPrior = 0, prevDigit = 0;
		while(el.Operation != TAtom::BAD) {
			nul.Position = prevPos - 1;
			prevPos = pos;
			switch (el.Operation) {
				case TAtom::DIGIT	: if (prevDigit) AddElem(addPrior, Ops, nul);Expression.push_back(el);prevDigit = 1;break;
				case TAtom::L		: if (prevDigit) AddElem(addPrior, Ops, nul);addPrior += 10;prevDigit = 0;break;
				case TAtom::R		: if (!prevDigit) {cout << "bad\n";exit(11);}addPrior -= 10;prevDigit = 1;break;
				case TAtom::Add: case TAtom::Sub: case TAtom::Mul: case TAtom::Div: case TAtom::Cmp: case TAtom::NUL :
					AddElem(addPrior, Ops, el);
					if (!prevDigit) {
						cout << " bad data; trying to add operation after operation\n";
						cout << line << "\n" << &line.c_str()[pos] << "\n";
						PrintVector(Expression, "");cout.flush();
					}

					prevDigit = 0;
					break;
			}
//cout << " pos " << pos;
			el.Init(line, pos);
//cout << " new " << pos << " ";cout << el;cout.flush();

		}
		while(!Ops.empty()){
			Expression.push_back(Ops.top().first);
			Ops.pop();
		}
		for(int i = 0; i < (int)Expression.size(); i++)
			if (Expression[i].Operation == TAtom::NUL) {
				Permute.push_back(i);
			}
	}
	int Clc() {
		stack<int> data;
		for(int i = 0; i < (int)Expression.size(); i++)
			Expression[i].Clc(data);
		return data.top();
	}
	int FindCombUp(stack<int> lDat, int level = 0) {
		//stack<int> lDat;// = data_;
		if (level == Permute.size()) {
			int prevI = 0;
			if (level > 0)
				prevI = Permute[level - 1];
			for(int i = prevI; i < (int)Expression.size(); i++)
				Expression[i].Clc(lDat);
			return lDat.top();
		}
		int prevI = 0, newInd = Permute[level];
		if (level > 0)
			prevI = Permute[level - 1];
		for(int i = prevI; i < newInd; i++)
			Expression[i].Clc(lDat);
		for(int i = TAtom::Mul; i >= TAtom::Add; i--) {
			Expression[Permute[level]].Operation = i;
			if (FindCombUp(lDat, level + 1))
				return 1;
		}
		return 0;
	}
	int FindComb0(int level = 0) {
		stack<int> data;
		return FindCombUp(data, level);
	}
	int FindComb1(int level = 0) {
		if (level == Permute.size()) 
			return Clc();
		for(int i = TAtom::Add; i < TAtom::Div; i++) {
			Expression[Permute[level]].Operation = i;
			if (FindComb1(level + 1))
				return 1;
		}
		return 0;
	}
	int FindComb() {
		TTriadClc calculator;
		calculator.Init(Expression);
		vector<TTriad> &triads = calculator.Triads;
		int n = (int)triads.size();
		if (n == 0)
			return calculator.Clc();
		for(int i = 0 ; i < n; i++)
			*triads[i].Operation = 0;
		while(1) {
			if (calculator.Clc())
				return 1;
			int lev = 0;
			while(*triads[lev].Operation == 2) {
					if (lev >= n - 1)
						return 0;
					*triads[lev].Operation = 0;
					lev++;
			} 
			(*triads[lev].Operation)++;
		}
	}

	int FindComb11() {
		int n = (int)Permute.size();
		if (n == 0)
			return Clc();
		for(int i = 0 ; i < n; i++)
			Expression[Permute[i]].Operation = TAtom::Add;
		while(1) {
			if (Clc())
				return 1;
			int lev = 0;
			while(Expression[Permute[lev]].Operation == TAtom::Div) {
					if (lev >= n -1)
						return 0;
					Expression[Permute[lev]].Operation = TAtom::Add;
					lev++;
			} 
			Expression[Permute[lev]].Operation++;
		}
	}

	string GetLine() {
		string line = Line;
		for(int i = 0; i < (int)Permute.size(); i++)
			line[Expression[Permute[i]].Position] = TAtom::Operation2Char[Expression[Permute[i]].Operation];
		string ret;
		for(int i = 0; i < line.size(); i++)
			if (!isspace(line[i]))
				ret.push_back(line[i]);
		return ret;
	}

};

int main( int argc,   char *argv[] ) {
    freopen("input.txt","r", stdin);
    freopen("output.txt","w", stdout);
	string line;
	getline(cin, line);
	TAtom::InitStatic();
	TCalculator calc(line);
	if (!calc.FindComb())
		line = "-1";
	else
		line = calc.GetLine();

	cout << line << "\n";
    return 0;
}













//map<int, map<int, l2> > Cmn;
//map<int, map<int, map<int, l2> > > Nsdr;
//
//l2 CmnF(int m, int n) {
//	int n1 = m - n;
//	l2 s = 1;
//	if (n > n1)
//		swap(n, n1);
//	if (Cmn[m].size() == 0 || Cmn[m].find(n) ==  Cmn[m].end()) {
//		for(int i = m; i > n1; i--)
//			s *= i;
//		for(int i = n; i > 1; i--)
//			s /= i;
//		Cmn[m][n] = s;
////cout << " m " << m << " n " << n << " s " << s << "\n";
//	}
//	return Cmn[m][n];
//}
//
//l2 NumComb(int sum, int digits, int rank) {
//	if (rank == 0)
//		if (sum == 0)
//			return 1;
//		else
//			return 0;
//	if (Nsdr[sum][digits].size() == 0 || Nsdr[sum][digits].find(rank) == Nsdr[sum][digits].end()) {
//		int iLev = 0;
//		l2 n = 0;
//		for(int iS = sum; iS >=0 && iLev <= digits; iS -= rank, iLev++) {
//			n += NumComb(iS, digits - iLev, rank -1) * CmnF(digits, iLev); 
//		}
//		Nsdr[sum][digits][rank] = n;
////cout << " sum " << sum << " digs " << digits << " rank " << rank << " comb " << n << "\n";
//	}
//	return Nsdr[sum][digits][rank];
//}



//int main( int argc,   char *argv[] ) {
//    freopen("input.txt","r", stdin);
//    freopen("output.txt","w", stdout);
//	int N2;
//	cin >> N2;
//	int N = N2 >> 1;
//	l2 s = 0, a;
//	for(int i = 0; i <= N * 9; i++) {
//		a = NumComb(i, N, 9);
//cout << " sum " << i << " digs " << N << " comb " << a << "\n";
//		s += a * a;
//	}
//	cout << s << "\n";
//    return 0;
//}
//
//

















//int main( int argc,   char *argv[] ) {
//    freopen("input.txt","r", stdin);
//    freopen("output.txt","w", stdout);
//	int N;//, curL = 0, curN, prevN = -1000000;
//	char str[10024];
//	cin >> N;
//	if (N <=1) {
//		cout << N << "\n";
//		return 0;
//	}
//
//	vector<int> maxSame(N, 0);
//	vector<vector<int> > field(N, maxSame), sum, sum2;
//	sum2 = sum = field;
//	for(int i = 0; i < N; i++) {
//		scanf("%s", str);
//		int j0 = 0;
//		while(j0 < 10014 && str[j0] != '0' && str[j0] != '1')
//			j0++;
//		int s = 0;
//		for(int j = 0;j < N; j++)
//			if (str[j + j0] == '1') {
//				field[i][j] = 1;
//				s++;
//				sum[i][j] = s;
//			} else 
//				s = 0;
//	}
//	int maxPossible = 0;
//	for(int j = 0; j < field.size(); j++) {
//		int s = 0;
//		for(int i = 0; i < field.size(); i++) {
//			if (sum[i][j]) {
//				s += sum[i][j];
//				sum2[i][j] = s;
//				maxPossible = max<double>(maxPossible, s);
//			} else
//				s = 0;
//		}
//	}
//	int i;
//	maxPossible = sqrt((double)maxPossible);
////cout << " maxPossible " << maxPossible << "\n";
////cout << "field:\n";PrintArray(field);cout << "\n";
////cout << "sum:\n";PrintArray(sum);cout << "\n";
////cout << "sum2:\n";PrintArray(sum2);cout << "\n";
//	for(i = maxPossible; i >=2; i--)
//		if (FoundSquare(i, field, sum, sum2))
//			break;
//	cout << i * i << "\n";
//    return 0;
//}
//


//void ClcCNM(int n, int m, BigNum &res) {
////cout << " N " << n << " M " << m << "\n";
//	int d1 = m, d2 = n - m;
//	if (d1 < d2)
//		swap(d1, d2);
////cout << d1 << " " << d2 << "\n";
//	BigNum deriv(1);
//	res.Set(1);
//	for(int i = n; i > d1; i--) 
//		res.Mul(BigNum(i));
//	for(int i = d2; i >= 2; i--) 
//		deriv.Mul(BigNum(i));
////cout << res << " " << deriv << "\n";
//	res.Div(deriv);
//}
//l2 ClcMN(int n, int m) {
//	l2 res = 1;
//	for(int i = n; i > n - m; i--) 
//		res *= i;
//	for(int i = m; i >= 2; i--) 
//		res /= i;
//cout << " C [N " << n << " ,M " << m << " ] = " << res << "\n"; 
//	return res;
//}
//l2 ClcNumHalfComb(int order, int sum) {
//	//int maxN = 10 * order - 1, n = order - 1;
//	int n = order - 1, maxN = sum + n;
//	l2 ret = ClcMN(maxN, n);
//	int sub = 0;
//	for(int i = 0; 1;i++) {
//		maxN -= 10;
//		if (maxN < n)
//			break;
//		int coef = -i;
//		//int coef = -1;
//		if (!i)
//			coef = 1;
//		sub += coef * ClcMN(maxN, n);
//cout << " sub " << sub << "\n";
//	}
//	ret -= order * sub;
//cout << 1 - ret << " " << (1 - ret) / order << "\n";
//	return ret;
//}


//    BigNum aNum, estNum, ansNum, num2, aEst;
//	aNum.Set(A);
//	int power = aNum.GetPower();
//	double mantisa = aNum.GetMantisa();
//	if (power%2) {
//		power--;
//		mantisa *= 10;
//	}
//	double mant2 = sqrt(mantisa);
//	estNum.Set(power/2, mant2);
//cout << estNum << "\n";
//	aEst.Set(estNum);aEst.Mul(estNum);
//	ansNum.Set(1);num2.Set(2);
//	while(!ansNum.IsEq(estNum)) {
//cout << estNum << "\n";
//		ansNum.Set(estNum);
//		aEst.Add(aNum).Div(estNum.Mul(num2));
//		estNum.Set(aEst);
//		aEst.Mul(estNum);
//	}
//	if (aNum < aEst) {
//		num2.Set(1);
//		ansNum.Sub(num2);
//	}
//	cout << ansNum << "\n";


//int main( int argc,   char *argv[] ) {
//    freopen("input.txt","r", stdin);
//    freopen("output.txt","w", stdout);
//    l2 M;
//    cin >> M;
//    vector<l2> simple;
//	FindDiv(M, simple);
//    if (simple.size() == 0)
//        cout << "Absent\n";
//    else {
//		//cout << simple.size();
//        //PrintVector(simple, "*");
//        for(size_t i = 0; i < simple.size() - 1; i++)
//            printf("%I64i*", simple[i]);
//		printf("%I64i\n", simple[simple.size() - 1]);
//    }
//    //cout << A / FindNOD(A, B) * B << "\n";
//    return 0;
//}
















//template<class T>
//void ReadData(vector<T> &dat, int s = 0) {
//	if (!s)
//		cin >> s;
//	dat.resize(s);
//	for(size_t i = 0; i < dat.size(); i++) {
//		cin >> dat[i];
//	}
//}
//template<class T>
//void SetVector(vector<vector<T> > &dat, int i, int j, const T &val) {
//	vector<T> raw(j, val);
//	dat.resize(i, raw);
//}
//template<class T>
//void PrintVector(vector<T> &dat, const char *sep = NULL) {
//	if (!dat.size())
//		return;
//	for(size_t i = 0; i < dat.size() - 1; i++)
//		if (!sep)
//			cout << dat[i] << "\t";
//		else
//			cout << dat[i] << sep;
//	cout << dat[dat.size() - 1];
//}
//template<class T>
//void PrintArray(vector<vector<T> > &dat) {
//	for(size_t i = 0; i < dat.size(); i++) {
//		PrintVector(dat[i]);
//		cout << "\n";
//	}
//	cout << "\n";
//}
//
//struct BigNum {
//private:
//    vector<char> Number;
//	int Negative;
//public:
//    BigNum():Negative(0) {
//		Set(0);
//		Number.reserve(1000);
//    }
//    BigNum(l2 l):Negative(0) {
//		Set(l);
//		Number.reserve(1000);
//    }
//	BigNum(const BigNum &n):Number(n.Number), Negative(n.Negative) {
//    }
//    void Set(BigNum &i) {
//        Number = i.Number;
//		Negative = i.Negative;
//    }
//    void Set(l2 i) {
//        Number.clear();
//		Number.reserve(40);
//		if (i < 0) {
//			Negative = 1;
//			long *r = (long*)&i;
//			*r = abs(*r);
//		}
//
//        vector<char> tmp;
//        while(i/10) {
//            Number.push_back(i % 10);
//            i /= 10;
//        }
//        Number.push_back(char(i));
//		Normalize();
//    }
//	//void Set(const char *str) {
//	//	int l = strlen(str);
//	//	Number.clear();
//    //  Number.resize(l, 0);
//	//	for(int i = 0; i < l; i++) {
//	//		Number[l - 1 - i] = str[i] - '0';
//	//	}
//	//	Normalize();
//	//}
//	//BigNum(const char *str):Negative(0) {
//	//	Set(str);
//	//}
//	//void Set(int power, double mantisa) {
//	//	if (mantisa==0) {
//	//		Set((l2)0);
//	//		return;
//	//	}
//	//	while (mantisa >= 10) {
//	//		mantisa /= 10;
//	//		power++;
//	//	}
//	//	while (mantisa < 1) {
//	//		mantisa *= 10;
//	//		power--;
//	//	}
//	//	Number.clear();
//	//	for(int i = 0; i <= power; i++) {
//	//		int m = int(mantisa);
//	//		AddDigit(Number, m);
//	//		mantisa = 10 * (mantisa - m);
//	//	}
//	//}
//	//int GetPower() {
//	//	return max<int>(0, (int)Number.size() - 1);
//	//}
//	//double GetMantisa() {
//	//	double ret = 0;
//	//	double div = 1;
//	//	for(int i = Number.size() - 1; i >=0 && i >= (int)Number.size() - 17; i--) {
//	//		ret += Number[i] / div;
//	//		div *= 10;
//	//	}
//	//	return ret;
//	//}
//	bool IsEq(BigNum &num) {
//		Normalize(); num.Normalize();
//		return (Number == num.Number);
//	}
//	void Normalize() {
//		int i = Number.size() - 1;
//		while(i >= 0  && Number[i] == 0)
//			i--;
//		if (i < 0) {
//			Number.clear();
//			Number.resize(1, 0);
//			return;
//		}
//		if (i == Number.size() - 1)
//			return;
//		Number.resize(i + 1);
//	}
//	int IsNegative(int newVal = -1) {
//		if (newVal >= 0)
//			Negative = newVal;
//		return Negative;
//	}
//	bool AbsLess(BigNum &num) {
//		Normalize();
//		num.Normalize();
//		if (num.Number.size() != Number.size())
//			return Number.size() < num.Number.size();
//		for(int i = Number.size() - 1; i >= 0; i--) {
//			if (Number[i] == num.Number[i])
//				continue;
//			if (Number[i] < num.Number[i])
//				return true;
//			return false;
//		}
//		return false;
//	}
//	bool operator<(BigNum &num) {
//		if (Negative ^ num.Negative)
//			return Negative;
//		return (bool)Negative ^ AbsLess(num);
//	}
//    BigNum &Add(BigNum &num) {
//        int add = 0, s1 = Number.size(), s2 = num.Number.size();
//		vector<char> sum;
//		sum.reserve(max<double>(s1, s2) + 1);
//		for(int i = 0; i < max<double>(s1, s2); i++) {
//			int n1 = 0, n2 = 0;
//			if (i < s1)
//				n1 = Number[i];
//			if (i < s2)
//				n2 = num.Number[i];
//            int n = n1 + n2 + add;
//            add = 0;
//            if (n >= 10) {
//                add = n / 10;
//                n -= add * 10;
//            }
//			sum.push_back(n);
//        }
//		if (add)
//			sum.push_back(add);
//		swap(sum, Number);
//		Normalize();
//        return *this;
//    }
//    BigNum &Sub(BigNum &num) {
//        int credit = 0, s1 = Number.size(), s2 = num.Number.size(), willBeNeg = 0;
//		vector<char> *fst = &Number, *sec = &num.Number;
//		vector<char> sum;
//		sum.reserve(max<double>(s1, s2) + 1);
//		if (AbsLess(num)) {
//			swap(s1, s2);
//			swap(fst, sec);
//			willBeNeg = 1;
//		}
//		for(int i = 0; i < s1; i++) {
//			int n1 = (*fst)[i], n2 = 0;
//			if (i < s2)
//				n2 = (*sec)[i];
//            int n = n1 - n2;
//			if (credit) {
//				if (n > 0) {
//					credit = 0;
//					n--;
//				} else
//					n += 9;
//			} else {
//				if (n < 0) {
//					n += 10;
//					credit = 1;
//				}
//			}
//			sum.push_back(n);
//        }
//		swap(sum, Number);
//		Negative = willBeNeg;
//		Normalize();
//        return *this;
//    }
//    BigNum &Mul(BigNum &num) {
//        int add = 0, s1 = Number.size(), s2 = num.Number.size();
//		vector<char> *fst = &Number, *sec = &num.Number;
//		if (s2 < s1) {
//			swap(s1, s2);
//			swap(fst, sec);
//		}
//		BigNum sum, cur;
//        for(int i1 = 0; i1 < s1; i1++) {
//            int c = (*fst)[i1];
//			cur.Number.clear();
//			cur.Number.resize(i1, 0);
//			for(int i2 = 0; i2 < s2 || add; i2++) {
//				int n2 = 0;
//				if (i2 < s2)
//					n2 = (*sec)[i2];
//                int s = n2 * c + add;
//                add = 0;
//                if (s >= 10) {
//                    add = s / 10;
//                    s -= add * 10;
//                }
//                cur.Number.push_back(s);
//				
//			}
//
//			sum.Add(cur);
//        }
//		swap(sum.Number, Number);
//		Negative = Negative^num.Negative;
//		Normalize();
//        return *this;
//    }
//	void AddDigit(char d) {
//		AddDigit(Number, d);
//		Normalize();
//	}
//	void AddDigit(vector<char> &num, char d) {
//		vector<char> tmp(num.size() + 1);
//		copy(num.begin(), num.end(), tmp.begin() + 1);
//		tmp[0] = d;
//		swap(num, tmp);
//	}
//	BigNum &Div(BigNum &num) {
//		BigNum zero;
//		if (zero.IsEq(num))
//			exit(11);
//		if (AbsLess(num)) {
//			Set((l2)0);
//			return *this;
//		}
//        int s1 = Number.size(), s2 = num.Number.size();
//		BigNum div, cur;
//		cur.Number.assign(Number.begin() + s1 - s2 + 1, Number.end());
//        for(int i = 0; i <= s1 - s2; i++) {
//			AddDigit(cur.Number, Number[s1 - i - s2]);
//			int divN = 0;
//			while(!cur.AbsLess(num)) {
//				divN++;
//				cur.Sub(num);
//			}
//			div.Number.push_back(divN);
//		}
//		Number.resize(div.Number.size());
//		copy(div.Number.rbegin(), div.Number.rend(), Number.begin());
//		Negative = Negative^num.Negative;
//		Normalize();
//        return *this;
//    }
//    string ToChar() const {
//		BigNum tmp(*this);
//		tmp.Normalize();
//		int i = tmp.Number.size() - 1;
//		string ret;ret.reserve(i);
//        char z = '0';
//		if (Negative)
//			ret.push_back('-');
//        for( ; i >= 0; i--)
//            ret.push_back(tmp.Number[i] + z);
//        ret.push_back(0);
//        return ret;
//    }
//
//};
//ostream & operator<<(ostream &out, const BigNum &num) {
//	out << num.ToChar().c_str();
//	return out;
//}
//
//int main( int argc,   char *argv[] ) {
//    freopen("input.txt","r", stdin);
//    freopen("output.txt","w", stdout);
//	int N;//, curL = 0, curN, prevN = -1000000;
//	cin >> N;
//	vector<double> probUnit(5, 1e-1); 
//	//cout << 0 << " " << log(probUnit[0]) << " ";
//	for(int i = 1; i < probUnit.size(); i++) {
//		probUnit[i] *= pow(0.1, i);
//		//cout << log(probUnit[i]) << " ";
//	}
//	//cout << "\n";
//	for(int i = 1; i < N; i++) {
//		cout << i << " ";
//		for(int j = 0; j < probUnit.size(); j++)
//			cout << 2*log(1000.) - (log(1 - pow(1 - probUnit[j], i)) - log(1 - pow(1 - probUnit[j]/1000, i)) )<< " ";
//		cout << "\n";
//	}
//    return 0;
//}
