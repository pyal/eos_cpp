#ifndef __stroka_h
#define __stroka_h


#include "lib/std/util.h"
#include "class.h"
#include "assert.h"
// Stroka is case-sensitive
// stroka is case-insensitive


class Stroka : public DescribedClass {
protected:
#define ui32 unsigned long int
#define ui16 unsigned int
#define COMMA
#define CODEPAGE_ARGUMENT
#define CODEPAGE_ARGUMENT_DEFAULT

    static inline void y_deallocate(void *p, size_t) {
        free(p);
    }
    static inline void *y_reallocate(void *p, size_t, size_t new_sz) {
        return realloc(p, new_sz);
    }
    static inline void *y_allocate(size_t n) {
        return malloc(n);
    }
    inline bool atomic_increment(volatile long &p, long v) {
        return (p += v) != 0;
    }
    inline bool atomic_increment(volatile unsigned long &p) {
        return ++p != 0;
    }
    inline bool atomic_decrement(volatile unsigned long &p) {
        return --p != 0;
    }
    inline long atomic_add(volatile long &p, long v) {
        return p += v;
    }

    struct base {
        unsigned long refs;   // reference counter
        size_t nchars;        // == strlen()
        char chars[1];
    };

    enum {
        RESIZE_INCREMENT = 16,   // 64
        MAX_WASTE = 15,          // 63
        OVERHEAD = offsetof(base, chars) + 1
    };

    static size_t round_up(size_t sz) {
        return (size_t)(sz + (RESIZE_INCREMENT - 1)) & ~(RESIZE_INCREMENT - 1);
    }

    static size_t allocated(const base *b) {
        return round_up(b->nchars + OVERHEAD);   // may use member of base {}
    }

    static char *allocate(size_t size, base *old = &nullbase);
    static void deallocate(base *old);

    // ~~~ Data member ~~~
    char *p;


    // ~~~ Core functions ~~~
    base *pdata() const {
        base *b = (base *)(p - offsetof(base, chars));
        // assert(b == &nullbase || b->refs <= 0x10000);
        return b;
    }
    void relink(char *tmp) {
        if(p != null && !atomic_decrement(pdata()->refs))
            deallocate(pdata());
        p = tmp;
    }
    void cow() {
        if(pdata()->refs != 1)
            clone();
    }
    void clone()   // make a distinct copy of self
    {
        size_t len = length();
        relink((char *)memcpy(allocate(len), p, len));
    }
    size_t off(char *ret) const {
        return ret ? (size_t)(ret - p) : npos;
    }
    static bool read_token_part(std::istream &, char *, std::streamsize, char);
    static bool read_line_part(std::istream &, char *, std::streamsize, char);

    std::istream &build(
        std::istream &is,
        bool (*f)(std::istream &, char *, std::streamsize, char),
        size_t &total,
        char delim = '\x00');


    // ~~~ Static ~~~
    static base nullbase;

public:
    //struct Int {
    //    int I;
    //    Int(int i) : I(i) {}
    //};
    static Stroka Int2Str(int i) {
        char buf[65];
        if(sprintf(buf, "%i", i))
            return Stroka(buf);
        return Stroka("");
    }

    static char *null;
    static const size_t npos;                            // (size_t)(-1)
    static const size_t max_size;                        // npos - OVERHEAD
    static size_t hashVal(const char *pc, size_t len);   // make it public

public:
    // ~~~ Miscellaneous ~~~
    const char *c_str() const   // data(),chars(),operator~()
    {
        return p;
    }
    const char *operator~() const {
        return p;
    }
    // ~~~ STL compatible method to obtain data pointer ~~~
    char *begin() {
        cow();
        return p;
    }

    size_t length() const   // size(), operator+()
    {
        return pdata()->nchars;
    }
    size_t size() const   // size(), operator+()
    {
        return pdata()->nchars;
    }
    size_t operator+() const {
        return length();
    }

    size_t reserve() const   // capacity()
    {
        return p == null ? 0 : allocated(pdata()) - OVERHEAD;
    }
    size_t hash() const {
        return hashVal(p, length());
    }

    bool is_null() const   // operator!()
    {
        return *p == '\x00';
    }
    bool empty() const {
        return is_null();
    }
    bool operator!() const {
        return is_null();
    }

    // ~~~ Size and capacitiy ~~~
    Stroka &resize(size_t m, char c = ' ');   // remove or append

    // ~~~ Constructor ~~~ : FAMILY0(,Stroka)

    Stroka() {
        p = null;
    }
    Stroka(const Stroka &s) {
        if(s.p != null)
            atomic_increment(s.pdata()->refs);
        p = s.p;
    }
    Stroka(double f) {
        p = null;
        char buf[265];
        if(sprintf(buf, "%g", f))
            assign(buf);
    }
    Stroka(double f, int prec) {
        p = null;
        char buf[265];
        char format[256] = "%.";
        Itoa(prec, &format[2], 10);
        int l = strlen(format);
        format[l] = 'g';
        format[l + 1] = 0;
        if(sprintf(buf, &format[0], f))
            assign(buf);
    }
    Stroka(const Stroka &s, size_t pos, size_t n);
    /*explicit*/ Stroka(const char *pc);
    Stroka(const char *pc, size_t pos, size_t n);
    explicit Stroka(char c);
    Stroka(size_t n, char c = ' ');
    explicit Stroka(int n);
    //explicit Stroka (const Int &n);

    // ~~~ Destructor ~~~
    ~Stroka() {
        relink(null);
    }

    // ~~~ Assignment ~~~ : FAMILY0(Stroka&, assign);
    Stroka &assign(const Stroka &s) {
        if(s.p != null)
            atomic_increment(
                s.pdata()->refs);   // instead of check of assignment to itself
        relink(s.p);                // increment/decrement in right order
        return *this;
    }
    Stroka &assign(const Stroka &s, size_t pos, size_t n) {
        return assign(Stroka(s, pos, n));   // ? replace(0, npos, s.p, pos, n)
    }
    Stroka &assign(const char *pc) {
        return replace(0, npos, pc);   // ? assign(Stroka(pc))
    }
    Stroka &operator=(const Stroka &s) {
        return assign(s);
    }
    Stroka &operator=(const char *pc) {
        return replace(0, npos, pc);   // ? assign(Stroka(pc))
    }

    //static Stroka MakeFromNum(int Num){
    //    char tmp[256];
    //    return Stroka(Itoa(Num,tmp,10));
    //  }
    Stroka &Trim(char ch = ' ') {
        size_t pos = 0;
        while(pos < length() && p[pos] == ch)
            pos++;
        size_t end = length() - 1;
        while(end >= pos && p[end] == ch)
            end--;
        end++;
        return assign(*this, pos, end - pos);
    }
    // ~~~ Appending ~~~ : FAMILY0(Stroka&, append);
    Stroka &append(const Stroka &s) {
        return replace(length(), 0, s.p, 0, npos, s.length());
    }
    Stroka &append(const Stroka &s, size_t pos, size_t n) {
        return replace(length(), 0, s.p, pos, n, s.length());
    }
    Stroka &append(const char *pc) {
        return replace(length(), 0, pc);
    }
    Stroka &append(const char *pc, size_t pos, size_t n, size_t pc_len = npos) {
        return replace(length(), 0, pc, pos, n, pc_len);
    }
    Stroka &append(char c) {
        return replace(length(), 0, &c, 0, 1, 1);
    }
    Stroka &operator+=(const Stroka &s) {
        return replace(length(), 0, s.p, 0, npos, s.length());
    }
    Stroka &operator+=(const char *pc) {
        return replace(length(), 0, pc);
    }
    Stroka &operator+=(char c) {
        return replace(length(), 0, &c, 0, 1, 1);
    }
    friend Stroka operator+(const Stroka &s1, const Stroka &s2) {
        return Stroka(s1) += s2;
    }
    friend Stroka operator+(const Stroka &s1, const char *s2) {
        return Stroka(s1) += s2;
    }
    friend Stroka operator+(const Stroka &s1, char s2) {
        return Stroka(s1) += s2;
    }

    // ~~~ Prepending ~~~ : FAMILY0(Stroka&, prepend);
    Stroka &prepend(const Stroka &s) {
        return replace(0, 0, s.p, 0, npos, s.length());
    }
    Stroka &prepend(const Stroka &s, size_t pos, size_t n) {
        return replace(0, 0, s.p, pos, n, s.length());
    }
    Stroka &prepend(const char *pc) {
        return replace(0, 0, pc);
    }
    Stroka &prepend(char c) {
        return replace(0, 0, &c, 0, 1, 1);
    }

    // ~~~ Insertion ~~~ : FAMILY1(Stroka&, insert, size_t pos);
    Stroka &insert(size_t pos, const Stroka &s) {
        return replace(pos, 0, s.p, 0, npos, s.length());
    }
    Stroka &insert(size_t pos, const Stroka &s, size_t pos1, size_t n1) {
        return replace(pos, 0, s.p, pos1, n1, s.length());
    }
    Stroka &insert(size_t pos, const char *pc) {
        return replace(pos, 0, pc);
    }

    // ~~~ Removing ~~~
    Stroka &remove(size_t pos = 0, size_t n = npos) {
        return replace(pos, n, "");
    }
    Stroka &erase(size_t pos = 0, size_t n = npos) {
        return remove(pos, n);
    }

    // ~~~ replacement ~~~ : FAMILY2(Stroka&, replace, size_t pos, size_t n);
    Stroka &replace(size_t pos, size_t n, const Stroka &s) {
        return replace(pos, n, s.p, 0, npos, s.length());
    }
    Stroka &replace(size_t pos, size_t n, const Stroka &s, size_t pos1, size_t n1) {
        return replace(pos, n, s.p, pos1, n1, s.length());
    }
    Stroka &replace(size_t pos, size_t n, const char *pc) {
        size_t len1 = pc ? strlen(pc) : 0;
        return replace(pos, n, pc, 0, len1, len1);
    }

    // ~~~ main driver: should be protected (in the future)
    Stroka &replace(
        size_t pos,
        size_t n,
        const char *pc,
        size_t pos1,
        size_t n1,
        size_t len1);

    // ~~~ Comparison ~~~ : FAMILY0(int, compare)
    static int compare(const Stroka &s1, const Stroka &s2) {
        return strcmp(s1.p, s2.p);
    }
    static int compare(const char *p, const Stroka &s2) {
        return strcmp(p, s2.p);
    }
    static int compare(const Stroka &s1, const char *p) {
        return strcmp(s1.p, p);
    }
    bool is_prefix(const Stroka &s) const {
        size_t len = length();
        return len <= s.length() && strncmp(p, s.p, len) == 0;
    }

    friend bool operator==(const Stroka &s1, const Stroka &s2) {
        return compare(s1, s2) == 0;
    }
    friend bool operator==(const Stroka &s, const char *pc) {
        return compare(s, pc) == 0;
    }
    friend bool operator==(const char *pc, const Stroka &s) {
        return compare(pc, s) == 0;
    }

    friend bool operator!=(const Stroka &s1, const Stroka &s2) {
        return compare(s1, s2) != 0;
    }
    friend bool operator!=(const Stroka &s, const char *pc) {
        return compare(s, pc) != 0;
    }
    friend bool operator!=(const char *pc, const Stroka &s) {
        return compare(pc, s) != 0;
    }

    friend bool operator<(const Stroka &s1, const Stroka &s2) {
        return compare(s1, s2) < 0;
    }
    friend bool operator<(const Stroka &s, const char *pc) {
        return compare(s, pc) < 0;
    }
    friend bool operator<(const char *pc, const Stroka &s) {
        return compare(pc, s) < 0;
    }

    friend bool operator<=(const Stroka &s1, const Stroka &s2) {
        return compare(s1, s2) <= 0;
    }
    friend bool operator<=(const Stroka &s, const char *pc) {
        return compare(s, pc) <= 0;
    }
    friend bool operator<=(const char *pc, const Stroka &s) {
        return compare(pc, s) <= 0;
    }

    friend bool operator>(const Stroka &s1, const Stroka &s2) {
        return compare(s1, s2) > 0;
    }
    friend bool operator>(const Stroka &s, const char *pc) {
        return compare(s, pc) > 0;
    }
    friend bool operator>(const char *pc, const Stroka &s) {
        return compare(pc, s) > 0;
    }

    friend bool operator>=(const Stroka &s1, const Stroka &s2) {
        return compare(s1, s2) >= 0;
    }
    friend bool operator>=(const Stroka &s, const char *pc) {
        return compare(s, pc) >= 0;
    }
    friend bool operator>=(const char *pc, const Stroka &s) {
        return compare(pc, s) >= 0;
    }

    // ~~ Read access ~~
    char at(size_t pos) const {
        if(pos < length())
            return p[pos];
        return '\x00';
    }
    char get_at(size_t pos) const {
        if(pos < length())
            return p[pos];
        return '\x00';
    }
    char operator[](size_t pos) const {
        if(pos < length())
            return p[pos];
        return '\x00';
    }

#if 0
// ~~ Write access ~~
    char &at(size_t pos);
    void put_at(size_t pos, char c)
      { at(pos) = c;
      }
    char &operator[](size_t pos)
      { return at(pos);
      }
#endif

    // ~~~ Change Case ~~~
    void to_lower(size_t pos = 0, size_t n = npos COMMA CODEPAGE_ARGUMENT_DEFAULT);
    void to_upper(size_t pos = 0, size_t n = npos COMMA CODEPAGE_ARGUMENT_DEFAULT);
    void to_title(CODEPAGE_ARGUMENT_DEFAULT);

    friend Stroka to_lower(const Stroka &s COMMA CODEPAGE_ARGUMENT_DEFAULT) {
        Stroka ret(s);
        ret.to_lower(0, Stroka::npos);   //, cp);
        return ret;
    }
    friend Stroka to_upper(const Stroka &s COMMA CODEPAGE_ARGUMENT_DEFAULT) {
        Stroka ret(s);
        ret.to_upper(0, Stroka::npos);   //, cp);
        return ret;
    }
    friend Stroka to_title(const Stroka &s COMMA CODEPAGE_ARGUMENT_DEFAULT) {
        Stroka ret(s);
        ret.to_title();   //cp);
        return ret;
    }

    //~~~~Search~~~~
    size_t find(const Stroka &s, size_t pos = 0) const {
        return find(s.p, pos);
    }

    size_t find(const char *s, size_t pos = 0) const {
        if(pos >= length())
            return npos;
        return off(strstr(p + pos, s));
    }

    // not implemented
    // size_t rfind(const Stroka &s, size_t pos = 0);

    size_t find(char c) const {
        return off(strchr(p, c));
    }
    size_t rfind(char c) const {
        return off(strrchr(p, c));
    }

    //~~~~Character Set Search~~~
    size_t find_first_of(const Stroka &set, size_t pos = 0) {
        return find_first_of(set.p, pos);
    }
    size_t find_first_of(const char *set, size_t pos = 0) {
        if(pos >= length())
            return npos;
        return off(strpbrk(p + pos, set));
    }
    size_t find_first_not_of(const Stroka &set, size_t pos = 0);

    // not implemented
    // size_t find_last_of(const Stroka &, size_t = 0);
    // size_t find_last_not_of(const Stroka &, size_t = 0);

    // ~~ Initial Capacity ~~
    static size_t initial_capacity(size_t = 0) {
        return 0;
    }
    static size_t get_initial_capacity() {
        return 0;
    }

    // ~~ ReSize Increment ~~
    static size_t resize_increment(size_t = 32) {
        return 32;
    }
    static size_t get_resize_increment() {
        return 32;
    }

    //~~~~Input~~~~
    // ~~ until Delimiter ~~
    std::istream &read_to_delim(std::istream &is, char delim = '\n') {
        size_t n = 0;
        return build(is, read_line_part, n, delim);
    }

    friend std::istream &getline(std::istream &is, Stroka &s, char c = '\n') {
        return s.read_to_delim(is, c);
    }

    // ~~ until WhiteSpace
    std::istream &read_token(std::istream &is) {
        size_t n = 0;
        return build(is, read_token_part, n);
    }

    friend std::istream &operator>>(std::istream &is, Stroka &s) {
        return s.read_token(is);
    }

    // ~~ until EOF ~~
    std::istream &read_file(std::istream &is) {
        return read_to_delim(is, '\x00');
    }
    // ~~ until EOS ~~
    std::istream &read_string(std::istream &is) {
        return read_to_delim(is, '\x00');
    }

    //~~~~Output~~~~
    friend std::ostream &operator<<(std::ostream &os, const Stroka &s) {
        return os << s.p;
    }

    // ~~~ Full copy ~~~
    Stroka copy() const;
    void copy(char *pc, size_t n) const {
        MemoryMove(p, pc, n);
    }

    // ~~~ Partial copy ~~~~
    Stroka substr(size_t pos, size_t n = npos) const;

    // ~~~ Reversion ~~~~
    //    void reverse()
    //      { cow();
    //        strrev(p);
    //      }
};


class stroka : public Stroka {
#define STROKA_INLINE
public:
    stroka() : Stroka() {}
    stroka(const Stroka &s) : Stroka(s) {}
    stroka(const Stroka &s, size_t pos, size_t n) : Stroka(s, pos, n) {}
    stroka(const char *pc) : Stroka(pc) {}
    stroka(const char *pc, size_t pos, size_t n) : Stroka(pc, pos, n){};
    explicit stroka(char c) : Stroka(c) {}
    explicit stroka(size_t n, char c = ' ') : Stroka(n, c) {}

    // ~~~ Comparison ~~~ : FAMILY0(int, compare)
    static STROKA_INLINE int compare(
        const stroka &s1,
        const stroka &s2 COMMA CODEPAGE_ARGUMENT_DEFAULT);
    static STROKA_INLINE int compare(
        const char *p,
        const stroka &s2 COMMA CODEPAGE_ARGUMENT_DEFAULT);
    static STROKA_INLINE int compare(
        const stroka &s1,
        const char *p COMMA CODEPAGE_ARGUMENT_DEFAULT);
    bool is_prefix(const stroka &s COMMA CODEPAGE_ARGUMENT_DEFAULT) const;

    friend bool operator==(const stroka &s1, const stroka &s2) {
        return stroka::compare(s1, s2) == 0;
    }
    friend bool operator==(const stroka &s, const char *pc) {
        return stroka::compare(s, pc) == 0;
    }
    friend bool operator==(const char *pc, const stroka &s) {
        return stroka::compare(pc, s) == 0;
    }

    friend bool operator!=(const stroka &s1, const stroka &s2) {
        return stroka::compare(s1, s2) != 0;
    }
    friend bool operator!=(const stroka &s, const char *pc) {
        return stroka::compare(s, pc) != 0;
    }
    friend bool operator!=(const char *pc, const stroka &s) {
        return stroka::compare(pc, s) != 0;
    }

    friend bool operator<(const stroka &s1, const stroka &s2) {
        return stroka::compare(s1, s2) < 0;
    }
    friend bool operator<(const stroka &s, const char *pc) {
        return stroka::compare(s, pc) < 0;
    }
    friend bool operator<(const char *pc, const stroka &s) {
        return stroka::compare(pc, s) < 0;
    }

    friend bool operator<=(const stroka &s1, const stroka &s2) {
        return stroka::compare(s1, s2) <= 0;
    }
    friend bool operator<=(const stroka &s, const char *pc) {
        return stroka::compare(s, pc) <= 0;
    }
    friend bool operator<=(const char *pc, const stroka &s) {
        return stroka::compare(pc, s) <= 0;
    }

    friend bool operator>(const stroka &s1, const stroka &s2) {
        return stroka::compare(s1, s2) > 0;
    }
    friend bool operator>(const stroka &s, const char *pc) {
        return stroka::compare(s, pc) > 0;
    }
    friend bool operator>(const char *pc, const stroka &s) {
        return stroka::compare(pc, s) > 0;
    }

    friend bool operator>=(const stroka &s1, const stroka &s2) {
        return stroka::compare(s1, s2) >= 0;
    }
    friend bool operator>=(const stroka &s, const char *pc) {
        return stroka::compare(s, pc) >= 0;
    }
    friend bool operator>=(const char *pc, const stroka &s) {
        return stroka::compare(pc, s) >= 0;
    }
    static size_t hashVal(const char *pc, size_t len COMMA CODEPAGE_ARGUMENT_DEFAULT);
    size_t hash() const {
        return stroka::hashVal(p, length());
    }
};

inline int stroka::compare(const stroka &s1, const stroka &s2) {
    return Stricmp(s1.p, s2.p);
}
inline int stroka::compare(const char *p, const stroka &s2) {
    return Stricmp(p, s2.p);
}
inline int stroka::compare(const stroka &s1, const char *p) {
    return Stricmp(s1.p, p);
}
inline bool stroka::is_prefix(const stroka &s) const {
    size_t len = length();
    return len <= s.length() && Strnicmp(p, s.p, len) == 0;
}


#endif   // __stroka_h
