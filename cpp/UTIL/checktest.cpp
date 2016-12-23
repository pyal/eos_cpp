

#include "lib/ref/file_manip.h"
#include "lib/std/ex_out.h"
#include "lib/ref/commandline.h"

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

//// We are trying to solve the problem - given a list of 9 numbers 5 - insert all possible operations and breaks between them
//// Find the first possitive number which cannot be obtained by the obtained formulas.
//
//#include <string>
//#include <map>
//#include <iostream>
//#include <vector>
//#include <list>
//#include <stdio.h>
//#include <cmath>
//#include <algorithm>
//#include <list>
//#include <set>
//#include <strstream>
//#include <numeric>
//#include "math.h"
// 
//using namespace std;
// 
//// Get all vars after adding next number
//// Make all possible operations with the input list +-*/
//void GetVars(int num, set<int> &vars) {
//	set<int> was = vars;
//	vars.clear();
//	for(set<int>::iterator it = was.begin(); it != was.end(); it++) {
//		int i = *it;
//// operations +-* do not depend on the order
//		vars.insert(i + 5);
//		vars.insert(i - 5);
//		vars.insert(i * 5);
//// operation / depend on the order - we get 2 different choices
//// after the division we are making rounding of the answer
//		vars.insert(int(i / 5));
//// we prohibit infinity. We can try to take it into account of cause.
//		if (i)
//			vars.insert(int(5 / i));
//	}
//}
//int main( int argc,   char *argv[] ) {
//// in our case numOfNum = 9 number of numbers in the formula
//// and the value of number is 5
//	int number = 5, numOfNum = 9;
//// set of possible results done by the formula
//	set<int> vars;
//// put initial number - result for numOfNum = 1
//	vars.insert(number);
//	vars.insert(-number);
//	for(int i = 2; i <= numOfNum; i++) {
//// one by one constructing the results for increasing length of the formula
//		GetVars(number, vars);
//	}
//// finding the first gap in the results
//	int foundMin = 0;
//	set<int>::iterator it = vars.begin();
//// skip negative numbers
//	while(it != vars.end() && *it <= 0)
//		it++;
//// break as soon as the gap is found
//	while(it != vars.end() && *it == foundMin + 1) {
//		foundMin++;
//		it++;
//	}
//	cout << foundMin + 1 << "\n";
//    return 0;
//}
//

// We are trying to solve the problem - given a list of 9 numbers 5 - insert all possible operations and breaks between them
// Find the first possitive number which cannot be obtained by the obtained formulas.

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
 
/*
ID: pyal1
PROG:  preface
LANG: C++
*/

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
#include <fstream>
#include <stack>
#include <string.h>
#include <queue>


using namespace std;
 
typedef long long l2;
typedef pair<int, int> p2;
typedef pair<double, double> d2;
template<class T>
vector<T> &ReadData(vector<T> &dat, int s = 0) {
    if (!s)
        cin >> s;
    dat.resize(s);
    for(size_t i = 0; i < dat.size(); i++)
        cin >> dat[i];
    return dat;
}
template<class T, class T1>
ostream& operator<<(ostream &out, const pair<T, T1> &pnt) {
    out << pnt.first << " " << pnt.second << " ";
    return out;
};
template<class T, class T1>
istream& operator>>(istream &in, pair<T, T1> &pnt) {
    in >> pnt.first >> pnt.second;
    return in;
};

template<class T>
ostream& operator<<(ostream &out, const set<T> &s) {
    for(set<T>::const_iterator i = s.begin(); i != s.end(); i++) {
        out << *i << " ";
        //if ((i+1) != s.end())
        //    out << " ";
    }
    return out;
};

template<class T>
ostream& operator<<(ostream &out, const vector<T> &vec) {
    for(size_t i = 0; i < vec.size(); i++) {
        out << vec[i];
        if (i != vec.size() -1 )
            out << " ";
    }
    return out;
};

template<class T>
void PrintVector(const vector<T> &dat, const char *sep = NULL) {
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
}
template<class T>
map<T, int> Vector2Map(const vector<T> &vec, int InitByInd = 1) {
    map<T, int> res;
    for (size_t i = 0; i < vec.size(); i++)
        if (InitByInd)
            res[vec[i]] = (int)i;
        else
            res[vec[i]] = 0;
    return res;
}
template<class T1, class T2>
map<T1, T2> Vector2Map(const vector<pair<T1, T2> > &vec) {
    map<T1, T2> res;
    for (size_t i = 0; i < vec.size(); i++)
            res[vec[i].first] = vec[i].second;
    return res;
}

template <class T>
struct TCmpVector {
    vector<T> Vector;

    TCmpVector() {};
    TCmpVector(const vector<T> &srcV) : Vector(srcV) {};
    TCmpVector(const TCmpVector &src) : Vector(src.Vector){}
    TCmpVector& operator=(const TCmpVector &src) {
        Vector = src.Vector;
        return *this;
    }
    int operator<(const TCmpVector &src) const {
        for(size_t i = 0; i < Vector.size(); i++) {
            if (Vector[i] > src.Vector[i])
                return 0;
            if (Vector[i] < src.Vector[i])
                return 1;
        }
        return 0;
    }
    int operator==(const TCmpVector &src) {
        for(size_t i = 0; i < Vector.size(); i++)
            if (Vector[i] != src.Vector[i])
                return 0;
        return 1;
    }
};
template<class T>
void ReadAll(istream &in, vector<T> &vec) {
    T el;
    vec.clear();
    while(1) {
        in >> el;
        if (!in)
            break;
        vec.push_back(el);
    }
}
void *StaticSortArray;
template<class T>
bool greaterWeight( int w1, int w2 ){
    return ((T*)StaticSortArray)[w1] > ((T*)StaticSortArray)[w2];
}
template<class T>
bool lessWeight( int w1, int w2 ){
    return ((T*)StaticSortArray)[w1] < ((T*)StaticSortArray)[w2];
};
template<class T>
std::vector<int> Sort(const std::vector<T> &vec, int AscendingOrder){
    std::vector<int> ind(vec.size());
    for(size_t i = 0;i<ind.size();i++)
        ind[i] = (int)i;
    StaticSortArray = (void*)&vec[0 ];
    if (AscendingOrder)
        std::sort(ind.begin(), ind.end(), lessWeight<T>);
    else
        std::sort(ind.begin(), ind.end(), greaterWeight<T>);
    return ind;
};

void HashInsert(map<char, int> &hashCode, vector<char> &hashV, char ch) {
    int p = hashCode[ch];
    while (hashV[p] != '0')
        if (++p >= (int)hashV.size())
            p = 0;
    hashV[p] = ch;
}

int IsPossible(map<char, int> &hashCode, vector<char> &hashV) {
    vector<char> order = hashV;
    sort(order.begin(), order.end());
    int j = 0;
    do {
        //cout << j++ << " ";
        vector<char> buf(hashV.size(), '0');
        for(size_t i = 0; i < order.size(); i++)
            HashInsert(hashCode, buf, order[i]);
        if (buf == hashV)
            return 1;
    } while(next_permutation(order.begin(), order.end()));
    cout << "\n";
    return 0;
}


typedef pair<int, int> i2i;  // value2weight
istream &operator>>(istream &in, i2i &x) {
    in >> x.first >> x.second;
    return in;
}
bool operator<(i2i &fst, i2i &sec) {
    return (double)fst.first / fst.second < (double)sec.first / sec.second;
}

void Set(vector<int> &index, int p, int q) {
    q = index[q];
    p = index[p];
    for(size_t i = 0; i < index.size(); i++)
        if (index[i] == p)
            index[i] = q;
}
int Parent(int i, vector<int> &uni) {
    while(uni[i] != i)
        i = uni[i];
    return i;
}
void Set(int p, int q, vector<int> &uni, vector<int> &sUni) {
    p = Parent(p, uni);
    q = Parent(q, uni);
        swap(p, q);
    if (sUni[p] > sUni[q])
        swap(p, q);
    sUni[q] += sUni[p];
    uni[p] = uni[q];
}



void PrintLevel() {
    vector<int> index(10), s(10, 1);
    int p, i = 0;
    while(i < 10 && scanf("%i", &p) == 1) {
        index[i] = p;
        i++;
    }
    cout << "\n" << index << "\n";
    for(size_t i = 0; i < index.size(); i++) {
        int j = 0, c = i;
        cout << i ;
        while(index[c] != c) {
            c = index[c];
            cout << " -> " << c;
            s[c]++;
            if (j++ > 10) {
                cout << " \nfound circle\n";
                return;
            }
        }
        cout << "\n";
    }
    for(size_t i = 0; i < index.size(); i++) {
        cout << i << "(" << s[i] <<")  ";
    }
    cout << "\n";
}



int main( int argc,   char *argv[] ) {
    freopen(argv[1],"r", stdin);
    for(int i = 0; i < 5; i++)
        PrintLevel();
    exit(1);
    vector<int> index(10);
    vector<int> s(index.size(), 1);
    for(size_t i = 0; i < index.size(); i++)
        index[i] = i;
    int p, q;
    while(scanf("%i-%i", &p, &q)== 2) {
        Set(index, p, q);
        //Set(p, q, index, s);
    cout << index << "    " << p << " " << q << "\n";
    }
    cout << index << "\n";
    //freopen("output.txt","w", stdout);
    //fstream in("input.txt", ios::in);
    Time_struct time;

    return 0;
}

















//struct TKnapSack {
//    vector<int> Order;
//    vector<i2i> Value2Weight;
//    int NumItems, CapacityK;
//    void ReadData() {
//        cin >> NumItems >> CapacityK;
//        //Value2Weight.resize(NumItems);
//        ::ReadData(Value2Weight, NumItems);
//        Order = Sort(Value2Weight, 0);
//    }
//    int MaxValueGreedy(vector<int> &solution) {
//        solution.resize(NumItems, 0);
//        int capLeft = CapacityK, value = 0;
//        for(size_t i = 0; i < Order.size(); i++) {
//            if (Value2Weight[Order[i]].second <= capLeft) {
//                capLeft -= Value2Weight[Order[i]].second;
//                value += Value2Weight[Order[i]].first;
//                solution[Order[i]] = 1;
//            }
//        }
//        return value;
//    }
//    int MaxValueStrict(vector<int> &solution) {
//        solution.clear();
//        solution.resize(NumItems, 0);
//        vector<vector<int> > Item2Size_Value;
//        for(size_t item = 0; item < Order.size(); item++) {
//            Item2Size_Value.push_back(vector<int>(CapacityK + 1, 0));
//            if (item == 0) {
//                for(size_t cap = Value2Weight[Order[item]].second; cap < Item2Size_Value[item].size(); cap++)
//                    Item2Size_Value[item][cap] = Value2Weight[Order[item]].first;
//                continue;
//            }
//            for(size_t cap = 0; cap < (size_t)Value2Weight[Order[item]].second && cap < Item2Size_Value[item].size(); cap++)
//                Item2Size_Value[item][cap] = Item2Size_Value[item - 1][cap];
//            for(size_t cap = Value2Weight[Order[item]].second; cap < Item2Size_Value[item].size(); cap++)
//                Item2Size_Value[item][cap] = max(Item2Size_Value[item - 1][cap - Value2Weight[Order[item]].second] + Value2Weight[Order[item]].first, Item2Size_Value[item -1][cap]); 
//        }
//        int curCap = CapacityK, curValue = 0;
//        int value = Item2Size_Value[Item2Size_Value.size() - 1][curCap];
//        for(size_t item = Order.size() - 1; item > 0; item--) {
//            if (Item2Size_Value[item][curCap] != Item2Size_Value[item - 1][curCap]) {
//                solution[Order[item]] = 1;
//                curCap -= Value2Weight[Order[item]].second;
//            }
//        }
//        if (Item2Size_Value[0][curCap] != 0)
//            solution[Order[0]] = 1;
//        return value;
//    }
//    int Present(int curCap, vector<int> &steps) {
//        for(size_t i = 0; i < steps.size(); i++)
//            if (steps[i] == curCap)
//                return 1;
//        return 0;
//    }
//    int MaxValueStrict1(vector<int> &solution) {
//        solution.clear();
//        solution.resize(NumItems, 0);
//        vector<vector<int> > Item2Size_Step(NumItems);
//        vector<int> size_ValueCur(CapacityK + 1, 0), size_ValueNext(CapacityK + 1, 0);
//        for(size_t item = 0; item < Value2Weight.size(); item++) {
//            for(int cap = 0; cap < Value2Weight[item].second && cap < CapacityK + 1; cap++)
//                size_ValueNext[cap] = size_ValueCur[cap];
//            for(int cap = Value2Weight[item].second; cap < CapacityK + 1; cap++) {
//                int newVal = size_ValueCur[cap - Value2Weight[item].second] + Value2Weight[item].first;
//                if (newVal > size_ValueCur[cap]) {
//                    size_ValueNext[cap] = newVal;
//                    Item2Size_Step[item].push_back(cap);
//                } else
//                    size_ValueNext[cap] = size_ValueCur[cap];
//            }
//            size_ValueCur = size_ValueNext;
//        }
//        int curCap = CapacityK, curValue = 0;
//        int value = size_ValueCur[curCap];
//        for(int item = Value2Weight.size() - 1; item >= 0; item--) {
//            if (Present(curCap, Item2Size_Step[item])) {
//            //if (Item2Size_Value[item][curCap] != Item2Size_Value[item - 1][curCap]) {
//                solution[item] = 1;
//                curCap -= Value2Weight[item].second;
//            }
//        }
//        //if (Item2Size_Value[0][curCap] != 0)
//        //    solution[Order[0]] = 1;
//        return value;
//    }
//    double UnitProbSize() {
//        double sum = 0;
//        for(size_t item = 0; item < Order.size(); item++) {
//            double sig = (double)Value2Weight[Order[item]].first / Value2Weight[Order[item]].second;
//            double s = log(Value2Weight[Order[item]].second + 2.);
//            sum += Value2Weight[Order[item]].second * sig * s;
//        }
//        return sum;
//    }
//    vector<int> RndSol(TKnapSack &shortSack, double partToMake) {
////cout << "partToMake " << partToMake << "\n";
//        vector<int> rndSol(Value2Weight.size(), 0);
//        int toGet = (1 - partToMake) * CapacityK, got = 0;
//        double pSingle = CapacityK / UnitProbSize();
//        pSingle *= 4;
////cout << "pSingle " << pSingle << "\n";
////cout << "toGet " << toGet << " lowLim " << toGet - (CapacityK - toGet) * 0.1 << "\n";
//        for(size_t item = 0; item < Order.size() && got < toGet - (CapacityK - toGet) * 0.1; item++) {
//            double sig = (double)Value2Weight[Order[item]].first / Value2Weight[Order[item]].second;
//            double s = log(Value2Weight[Order[item]].second + 2.);
//            int curS = Value2Weight[Order[item]].second;
//            if (!((double)rand() / (RAND_MAX + 1) < pSingle * sig * s && got + curS <= toGet))
//                continue;
//            got += curS;
//            rndSol[Order[item]] = 1;
//        }
//        shortSack.NumItems = NumItems;
//        shortSack.CapacityK = CapacityK - got;
//        shortSack.Value2Weight = Value2Weight;
//        shortSack.Order.clear();
//        for(size_t item = 0; item < Order.size(); item++) {
//            if (rndSol[Order[item]] == 0)
//                shortSack.Order.push_back(Order[item]);
//        }
////cout << "GotSize " << got << "\n";
////cout << "left items " << shortSack.Order.size() << " left size " << shortSack.CapacityK << "\n";
//        return rndSol;
//    }
//    double EstimatePart() {
//        double time = (double)CapacityK * (double)NumItems / 500000.;
//        double coef = time / 300.;
//        return 1 / pow(coef, 1);
//    }
//    vector<int> AppendSol(vector<int> &fst, vector<int> sec) {
//        vector<int> ret(fst);
//        for(size_t i = 0; i < ret.size(); i++)
//            if (sec[i])
//                ret[i] = 1;
//        return ret;
//    }
//    int GetValue(vector<int> &sol) {
//        int s = 0;
//        for(size_t i = 0; i < sol.size(); i++)
//            if (sol[i])
//                s += Value2Weight[i].first;
//        return s;
//    }
//
//};
//int main( int argc,   char *argv[] ) {
//    freopen(argv[1],"r", stdin);
//    //freopen("output.txt","w", stdout);
//    //fstream in("input.txt", ios::in);
//    TKnapSack knap;
//    knap.ReadData();
//    vector<int> solution;
//    //int maxValue = knap.MaxValueGreedy(solution);
//    //cout << maxValue << " 0\n";
//    //cout << solution << "\n";
//    Time_struct time;
//        int maxValue = knap.MaxValueStrict1(solution);
//        cout << maxValue << " 1\n";
//        cout << solution << "\n";
//        //cout << "size " << knap.CapacityK * knap.NumItems / 500000. << " time " << time.Print() << "\n";
//        return 0;
//}
//
//







//int main( int argc,   char *argv[] ) {
//    freopen(argv[1],"r", stdin);
//    //freopen("output.txt","w", stdout);
//    //fstream in("input.txt", ios::in);
//    TKnapSack knap;
//    knap.ReadData();
//    vector<int> solution;
//    //int maxValue = knap.MaxValueGreedy(solution);
//    //cout << maxValue << " 0\n";
//    //cout << solution << "\n";
//    Time_struct time;
//    double part = (double)knap.CapacityK * (double)knap.NumItems / 500000. ;
////cout << "part " << part << "\n";cout.flush();
//    if (part < 0) {
//        int maxValue = knap.MaxValueStrict(solution);
//        cout << maxValue << " 1\n";
//        cout << solution << "\n";
//        //cout << "size " << knap.CapacityK * knap.NumItems / 500000. << " time " << time.Print() << "\n";
//        return 0;
//    }
//    int numRnd = 10;
//    part = knap.EstimatePart() / sqrt((double)numRnd);
//    part = 1 / 25.;
////cout << " EndPart " << 1. / part << "\n";cout.flush();return 0;
//    int bestValue = 0;
//    vector<int> bestSol;
//    for(int i = 0; i < numRnd; i++) {
//        Time_struct t;
//        TKnapSack shortKnap;
//        vector<int> sol1 = knap.RndSol(shortKnap, part), sol2;
////cout << " gotRnd " << knap.GetValue(sol1) << "\n";
//        shortKnap.MaxValueStrict(sol2);
//        vector<int> sol = knap.AppendSol(sol1, sol2);
//        int gotValue = knap.GetValue(sol);
////cout << " res " << gotValue << " shortS " << shortKnap.CapacityK << " capCoef " << knap.CapacityK / (double)shortKnap.CapacityK << " ";
////cout << t.Print() << "\n";cout.flush();
//        if (gotValue <= bestValue)
//            continue;
//        bestValue = gotValue;
//        bestSol = sol;
//    }
//    cout << bestValue << " 0\n";
//    cout << bestSol << "\n";
//    //cout << "size " << knap.CapacityK * knap.NumItems / 500000. << " time " << time.Print() << "\n";
//    return 0;
//}

//    vector<i2i> Insert(vector<i2i> cap2value, int cap, int value) {
//        if (cap > CapacityK)
//            return cap2value;
//        vector<i2i> ret;
//        size_t prevInd = 0, curInd = -1;
//        int curCap = 0, curValue = 0;
//        for(; curInd < cap2value.size(); prevInd++) {
//            if (curInd >= 0) {
//                curCap = cap2value[curInd].first;
//                curValue = cap2value[curInd].second;
//            }
//            for(; prevInd < cap2value.size() && cap2value[prevInd].first < cap + curCap; prevInd++) 
//                ret.push_back(cap2value[prevInd]);
//            if (cap2value[prevInd].first == cap + curCap) {
//                if (cap2value[prevInd].second < value + curValue) {
//                    ret.insert(i2i(cap, value));
//                }
//            }
//                ret.push_back(cap2value[i]);
//            }
//        }
//        if (ret.size() == cap2value.size()) {
//            ret.insert(i2i(cap, value));
//            return ret;
//        }
//        size_t j = i + 1;
//        for(; j < cap2value.size(); j++) {
//            if (cap2value[i].first + cap < cap2value[j].first
//        }
//    }
//    int MaxValueSF(vector<int> &solution) {
//        solution.clear();
//        solution.resize(NumItems, 0);
//        vector<vector<i2i> > Item2Size_Value;
//        for(size_t item = 0; item < Order.size(); item++) {
//            //Item2Size_Value.push_back(vector<int>(CapacityK + 1, 0));
//            if (item == 0) {
//                Item2Size_Value.push_back(vector<i2i>);
//                Item2Size_Value[item].push_back(i2i(Value2Weight[Order[item]].second, Value2Weight[Order[item]].first));
//                continue;
//            }
//            Item2Size_Value.push_back(Insert(Item2Size_Value[item - 1], Value2Weight[Order[item]].second, Value2Weight[Order[item]].first);
//        }
//        int curCap = CapacityK, curValue = 0;
//        int value = Item2Size_Value[Item2Size_Value.size() - 1][Item2Size_Value[Item2Size_Value.size() - 1].size() - 1].second;
//        for(size_t item = Order.size() - 1; item > 0; item--) {
//            if (Item2Size_Value[item][curCap] != Item2Size_Value[item - 1][curCap]) {
//                solution[Order[item]] = 1;
//                curCap -= Value2Weight[Order[item]].second;
////curValue += Value2Weight[Order[item]].first;
////cout << curCap << " " << curValue << " " << Order[item] << "\n";
//            }
//        }
//        if (Item2Size_Value[0][curCap] != 0)
//            solution[Order[0]] = 1;
//        return value;
//    }























//int main( int argc,   char *argv[] ) {
//    freopen(argv[1],"r", stdin);
//    //freopen("test.out","w", stdout);
//
//    Time_struct t, t0;
//    int knapSize;
//    vector<TItem> items;
//    cin >> knapSize;
//    knapSize++;
//    ReadData(items);
//    vector<int> row(knapSize, 0);
//    vector<vector<int> > A(items.size(), row);
//    for(int s = items[0].Weight; s < knapSize; s++) 
//        A[0][s] = items[0].Value;
//    for(size_t i = 1; i < items.size(); i++) {
//        for(int s = 0; s < knapSize; s++){
//            int noItem = A[i - 1][s];
//            int plusItem = s - items[i].Weight >= 0 ? A[i - 1][s - items[i].Weight] + items[i].Value : 0;
//            A[i][s] = max(noItem, plusItem);
//        }
//    }
//    cout << A[items.size() - 1][knapSize - 1] << "\n";
//    cout << t0;
//    return 0;
//}


//int IsGood(vector<int> &curPos, int level) {
//    for(int i = 0; i < level; i++)
//        if (curPos[i] == curPos[level] || curPos[i] + level - i == curPos[level] || curPos[i] - level + i == curPos[level])
//            return 0;
//    return 1;
//}
//int SetNext(vector<int> &curPos, int level) {
//    if (level < 0)
//        return 0;
//    int setPos = curPos[level] + 1;
//    while(curPos[level] <= (int)curPos.size()) {
//        int badPos = 0;
//        for(int i = 0; i < level; i++) {
//            int t = curPos[i], br = 0;
//            if (t == setPos || t + level - i == setPos || t - level + i == setPos) {
//                badPos = 1;
//                break;
//            }
//        }
//        if (!badPos)
//            break;
//        setPos++;
//    }
//    curPos[level] = setPos;
//    if (curPos[level] > (int)curPos.size()) {
//        curPos[level] = 0;
//        if (!SetNext(curPos, level - 1))
//            return 0;
//        return SetNext(curPos, level);
//    }
//    return 1;
//}























//int GetCombs(int money, vector<int> &coins, set<TCmpVector<int> > &combinations) {
//    combinations.clear();
//    if (money < 0)
//        return 0;
//    if (money == 0) {
//        combinations.insert(TCmpVector<int>(vector<int>(coins.size())));
//        return 1;
//    }
//    for(size_t i = 0; i < coins.size(); i++) {
//        set<TCmpVector<int> > curCombs;
//        GetCombs(money - coins[i], coins, curCombs);
//        for(set<TCmpVector<int> >::iterator it = curCombs.begin(); it != curCombs.end(); it++) {
//            TCmpVector<int> comb = *it;
//            comb.Vector[i]++;
//            combinations.insert(comb);
//        }
//    }
//    return combinations.size();
//}
//
//
//int GetAllCombs(int money, vector<int> &coins){
//    set<TCmpVector<int> > combinations;
//    int ret = GetCombs(money, coins, combinations);
//for(set<TCmpVector<int> >::iterator it = combinations.begin(); it != combinations.end(); it++)   cout <<  it->Vector << "\n";
//    return ret;
//}
//
//int main( int argc,   char *argv[] ) {
//    freopen("test.in","r", stdin);
//    freopen("test.out","w", stdout);
//
//    //freopen("milk3.in","r", stdin);
//    //freopen("milk3.out","w", stdout);
//    int money;
//    vector<int> coins;
//    cin >> money;
//    ReadData(coins);
//    cout << GetAllCombs(money, coins) << "\n";
//    return 0;
//}
//

//
//
//
//set<int> FindAllPQ(int pqMax) {
//}
//int IsGood(int a, int b, set<int> numbers) {
//}
//
//
//int main( int argc,   char *argv[] ) {
//    freopen("test.in","r", stdin);
//    freopen("test.out","w", stdout);
//
//    //freopen("clocks.in","r", stdin);
//    //freopen("clocks.out","w", stdout);
//    int lArithmProgr, pqMax;
//    cin >> lArithmProgr >> pqMax;
//    set<int> gotNums = FindAllPQ(pqMax);
//    if (gotNums.size() < lArithmProgr) {
//        cout << "NONE\n";
//        return 0;
//    }
//    vector<pair<int, int> > res;
//    for(int a = *gotNums.begin(); a < *(gotNums.end() - lArithmProgr); a++) {
//        for(int b = 1; b < (*(gotNums.end()-1) - *gotNums.begin()) / lArithmProgr); b++)
//            if (IsGood(a, b, gotNums))
//                res.push_back(pair<int, int>(b, a));
//    }
//    sort(res.begin(), res,end());
//    for(size_t i = 0; i < res.size(); i++)
//        cout << res[i].second << " " << res[i].second << "\n";
//    return 0;
//}


//#include <string>
//#include <map>
//#include <iostream>
//#include <vector>
//#include <list>
//#include <stdio.h>
//#include <cmath>
//#include <algorithm>
//#include <list>
//#include <set>
//#include <strstream>
//#include <numeric>
//#include "math.h"
//#include <fstream>
//#include <stack>
//
//
//using namespace std;
// 
//typedef long long l2;
//template<class T>
//void ReadData(vector<T> &dat, int s = 0) {
//    if (!s)
//        cin >> s;
//    dat.resize(s);
//    for(size_t i = 0; i < dat.size(); i++) {
//        cin >> dat[i];
//    }
//}
//
//template<class T>
//void PrintVector(const vector<T> &dat, const char *sep = NULL) {
//    if (!dat.size())
//        return;
//    for(size_t i = 0; i < dat.size() - 1; i++)
//        if (!sep)
//            cout << dat[i] << "\t";
//        else
//            cout << dat[i] << sep;
//    cout << dat[dat.size() - 1];
//}
//template<class T>
//void PrintArray(vector<vector<T> > &dat, const char *sep = NULL) {
//    for(size_t i = 0; i < dat.size(); i++) {
//        PrintVector(dat[i], sep);
//        cout << "\n";
//    }
//    cout << "\n";
//}
//template<class T>
//map<T, int> Vector2Map(const vector<T> &vec, int InitByInd = 1) {
//    map<T, int> res;
//    for (size_t i = 0; i < vec.size(); i++)
//        if (InitByInd)
//            res[vec[i]] = (int)i;
//        else
//            res[vec[i]] = 0;
//    return res;
//}
//struct TEncodeChar {
//    char Ch;
//    int Length;
//    int PosBeg;
//    int PosEnd;
//    TEncodeChar(char ch = 0, int pos = -1, length = 1)
//        : Ch(ch)
//        , PosBeg(pos)
//        , PosEnd(pos)
//        , Length(length) {
//    }
//    static vector<TEncodeChar> CvtStr(string &str_) {
//        vector<TEncodeChar> ret;
//        string str(str_.size(), ' ');
//        transform(str_.begin(), str_.end(), str.begin(), ::tolower);
//        for(size_t i = 0; i < str.size(); i++) {
//            if (str[i] >= 'a' && str[i] <= 'z') {
//                if (ret.size() && ret.back().Ch == str[i]) {
//                    ret.back().Length++;
//                    ret.back().PosEnd = i;
//                }
//                ret.push_back(TEncodeChar(str[i], i));
//            }
//        }
//        return ret;
//    }
//};
//
//ostream &operator<<(ostream &out, const TEncodeChar &ch) {
//    for(int i = 0; i < ch.Length; i++)
//        out << ch.Ch;
//    return out;
//}
//string ReadFile() {
//    char buf[1256];
//    string ret;
//    while(1) {
//        cin.getline(&buf[0], 1256);
//        if (!cin)
//            break;
//        if (ret.size())
//            ret.append("\n");
//        ret.append(buf);
//    }
//    return ret;
//}
//
//int GetPolyL(vector<TEncodeChar> encStr, int center, pair<int, int> &bnd) {
//    int i = 1;
//    int c2 = center / 2;
//    int shift = (center%2);
//    int len = (shift)?0:encStr[c2].Length();
//    bnd = (shift)? pair<int, int>(0, 0) : pair<int, int>(encStr[c2].PosBeg, encStr[c2].PosEnd);
//    for(; i <= min(c2, (int)encStr.size() - 1 - c2 + shift); i++) {
//        if (encStr[c2 - i].Ch != encStr[c2 + i - shift].Ch)
//            break;
//        if (encStr[c2 - i].Length != encStr[c2 + i - shift].Length) {
//            len += 2 * min(encStr[c2 - i].Length, encStr[c2 + i - shift].Length);
//            bnd = pair<int, int>(encStr[c2].PosBeg, encStr[c2].PosEnd)
//            break;
//        }
//        len += 2 * encStr[c2 - i].Length;
//    }
//    return len;
//}
//string GetPolyStr(string inStr, vector<TEncodeChar> encStr, int polyL, int polyCent) {
//    int shift = polyL % 2;
//    int fr = polyCent / 2 - polyL / 2, to = polyCent / 2 + polyL / 2 - 1 + shift;
//    int lft = encStr[fr].Pos, rgt = encStr[to].Pos;
//    return string(inStr.begin() + lft, inStr.begin() + rgt + 1);
//}
//
//int main( int argc,   char *argv[] ) {
//    freopen("test.in","r", stdin);
//    freopen("test.out","w", stdout);
//
//    //freopen("calfflac.in","r", stdin);
//    //freopen("calfflac.out","w", stdout);
//    string inStr = ReadFile();
//    vector<TEncodeChar> encStr = TEncodeChar::CvtStr(inStr);
//    int maxL = 0, maxI;
//    for(size_t i = 0; i < 2 * encStr.size(); i++) {
//        int curMax = GetPolyL(encStr, i);
//        if (curMax <= maxL)
//            continue;
//        maxL = curMax;
//        maxI = i;
//    }
//    cout << maxL << "\n" << GetPolyStr(inStr, encStr, maxL, maxI) << "\n";
//    return 0;
//}
//
















//
//int Permute(vector<int> &permutes, vector<int> &numV, int &num3, int &num21, int &num22) {
//    if (!permutes.size()) {
//        permutes.resize(5);
//    } else {
//        for(int i = 0; i < permutes.size(); i++) {
//            permutes[i]++;
//            if (permutes[i] < numV.size())
//                break;
//            permutes[i] = 0;
//        }
//        if (permutes.back() >= numV.size())
//            return 0;
//    }
//    num3 = 0;
//    for(int i = 0; i < 3; i++)
//        num3 += numV[permutes[i]] * pow(i, 10);
//    num21 = numV[permutes[3]];
//    num22 = numV[permutes[4]];
//}
//int PossibleNum(int num, map<int, int> &numM, int l) {
//    char nStr[256];
//    sprintf(nStr, "%i", num);
//    if (strlen(nStr) != l)
//        return 0;
//    for(int i = 0; i < l; i++)
//        if (numM.find(nStr[i]) == numM.end())
//            return 0;
//    return 1;
//}
//
//int main( int argc,   char *argv[] ) {
//    freopen("test.in","r", stdin);
//    freopen("test.out","w", stdout);
//
//    //freopen("crypt1.in","r", stdin);
//    //freopen("crypt1.out","w", stdout);
//    vector<int> numV;
//    ReadData(numV);
//    map<int, int> numM = Vector2Map(numV);
//    vector<int> permutes;
//    int r1, r2, num3, n21, n22, nVar = 0;
//    while(Permute(permutes, numV, num3, num21, num22) {
//        if (!PossibleNum(num3 * n21, numM, 3) || !PossibleNum(num3 * n22, numM, 3))
//            continue;
//        if (!PossibleNum(num3 * (n21 * 10 + n22), numM, 4))
//            continue;
//        nVar++;
//cout << num3 << "\n" << num21 * 10 + num22 << "\n\n" << r2 << "\n" << r1 * 10 << "\n\n" << r1 * 10 + r2 << "\n\n\n";
//    }
//    cout << nVar << "\n";
//    return 0;
//}
//








    //sort(gaps.begin(), gaps.end(), greater<int>());
    //int wasGaps = accumulate(gaps.begin(), gaps.end(), 0);
    //gaps.resize(nBoards - 1);
    //int leftGaps = accumulate(gaps.begin(), gaps.end(), 0);
    //cout << cows.back() - cows.front() << " " <<  wasGaps << " " << wasGaps + cows.size() << " " << leftGaps << "\n";
    //cout << cows.back() - cows.front() - leftGaps + 1<< "\n";































//int main( int argc,   char *argv[] ) {
//    freopen("input.txt","r", stdin);
//    freopen("output.txt","w", stdout);
//	int number, numOfNum;
//	cin >> number >> numOfNum;
//	set<int> vars;
//	vars.insert(number);
//	for(int i = 2; i <= numOfNum; i++) {
//		GetVars(number, vars);
//	}
//	int foundMin = 0;
//	set<int>::iterator it = vars.begin();
//	while(it != vars.end() && *it <= 0)
//		it++;
//	while(it != vars.end() && *it == foundMin+1) {
//		foundMin++;
//		it++;
//	}
//	cout << foundMin + 1 << "\n";
//    return 0;
//}


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
