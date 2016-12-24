#include <lib/precompiled/ref.h>


#include "Stroka.h"
#define AssertRange(cond) assert(cond)  /* if(!cond) throw outofrange_error(#cond); */
#define AssertLength(cond) assert(cond) /* if(!cond) throw maxlength_error(#cond); */

//#define POS_RANGE(pos,len) AssertRange(pos<=len)
#define POS_RANGE(pos, len) pos = pos < len ? pos : len

//#define NUM_RANGE(num,len,pos) num!=npos?AssertRange(num<=len-pos):num=len-pos
#define NUM_RANGE(num, len, pos) num = num < len - pos ? num : len - pos

#define MAX_LENGTH(len, max) AssertLength(len < max)

static ClassDesc Stroka_ClassDesc(
    typeid(Stroka),
    "Stroka",
    1,
    "Std_category",
    create<Stroka>);


char *Stroka::allocate(size_t size, base *old) {
    if(size == 0)
        return null;
    base *ret =
        old == &nullbase
            ?   // hack for _mt_
            (base *)y_allocate(round_up(size + OVERHEAD))
            : (base *)y_reallocate(old, allocated(old), round_up(size + OVERHEAD));

    assert(ret);   //  __THROW_BAD_ALLOC ?

    ret->refs = 1;
    ret->nchars = size;
    ret->chars[size] = '\x00';
    return ret->chars;
}

void Stroka::deallocate(base *old) {
    y_deallocate(old, allocated(old));
}

Stroka::base Stroka::nullbase = {0, 0, {'\x00'}};

char *Stroka::null = Stroka::nullbase.chars;

Stroka::Stroka(const Stroka &s, size_t pos, size_t n) {
    size_t len = s.length();
    POS_RANGE(pos, len);
    NUM_RANGE(n, len, pos);
    p = allocate(n);
    memcpy(p, s.p + pos, n);
}

Stroka::Stroka(const char *pc, size_t pos, size_t n) {
    p = allocate(n);
    memcpy(p, pc + pos, n);
}

Stroka::Stroka(const char *pc) {
    size_t len = pc ? (size_t)strlen(pc) : 0;
    p = allocate(len);
    memcpy(p, pc, len);
}

Stroka::Stroka(char c) {
    p = allocate(1);
    p[0] = c;
}

Stroka::Stroka(size_t n, char c) {
    MAX_LENGTH(n, Stroka::max_size);
    p = allocate(n);
    memset(p, c, n);
}

Stroka::Stroka(int n) {
    MAX_LENGTH((size_t)n, Stroka::max_size);
    p = allocate(n);
}

//Stroka::Stroka(const Int &i){
//    char tmp[256];
//    p = null;
//    assign(Itoa(i.I, tmp, 10));
//}
Stroka Stroka::copy() const {
    Stroka temp(*this);   // Make referenced copy
    temp.clone();         // Make a distinct copy
    return temp;
}

Stroka Stroka::substr(size_t pos, size_t n) const {
    return Stroka(*this, pos, n);
}

Stroka &Stroka::resize(size_t n, char c) {
    size_t len = length();
    if(n < len)
        return remove(n);
    else if(n > len)
        return append(Stroka(n - len, c));
    return *this;
}

//void Stroka::to_lower(size_t pos, size_t n COMMA CODEPAGE_ARGUMENT) {
//    bool changed = false;
//    size_t len   = length(), i;
//    char c;
//    POS_RANGE(pos, len);
//    NUM_RANGE(n, len, pos);
//    for (i = pos; n; i++, n--) {
//        c = TO_LOWER(p[i]);
//        if (c != p[i]) {
//            if (!changed) {
//                cow();
//                changed = true;
//            }
//            p[i] = c;
//        }
//    }
//}
//
//void Stroka::to_upper(size_t pos, size_t n COMMA CODEPAGE_ARGUMENT) {
//    bool changed = false;
//    size_t len   = length(), i;
//    char c;
//    POS_RANGE(pos, len);
//    NUM_RANGE(n, len, pos);
//    for (i = pos; n; i++, n--) {
//        c = TO_UPPER(p[i]);
//        if (c != p[i]) {
//            if (!changed) {
//                cow();
//                changed = true;
//            }
//            p[i] = c;
//        }
//    }
//}
//
//void Stroka::to_title(CODEPAGE_ARGUMENT) {
//    bool changed = false;
//    size_t len   = length(), i;
//    char c;
//    if (len) {
//        c = TO_TITLE(p[0]);
//        if (c != p[0]) {
//            if (!changed) {
//                cow();
//                changed = true;
//            }
//            p[0] = c;
//        }
//    }
//    for (i = 1; i < len; i++) {
//        char c = TO_LOWER(p[i]);
//        if (c != p[i]) {
//            if (!changed) {
//                cow();
//                changed = true;
//            }
//            p[i] = c;
//        }
//    }
//}
//
/*
int stroka::compare(const stroka &s1, const stroka &s2, const CodePage& cp)
{
    return cp.Stricmp(s1.p, s2.p);
}

int stroka::compare(const char* p, const stroka &s2, const CodePage& cp)
{
    return cp.Stricmp(p, s2.p);
}

int stroka::compare(const stroka &s1, const char* p, const CodePage& cp)
{
    return cp.Stricmp(s1.p, p);
}

bool stroka::is_prefix(const stroka &s, const CodePage& cp) const
{
    size_t len = length();
    return len <= s.length() && cp.strnicmp(p, s.p, len) == 0;
}

*/
size_t Stroka::hashVal(const char *pc, size_t len) {
    static const ui32 mask[sizeof(ui32)] = {
#if defined(_16_)
        0x0000,
        0x00FF
#elif defined(_little_endian_)
        0x00000000,
        0x000000FF,
        0x0000FFFF,
        0x00FFFFFF
#else
        0x00000000,
        0xFF000000,
        0xFFFF0000,
        0xFFFFFF00
#endif
    };

    size_t i, h;
    const ui32 *c;

#ifdef _must_align4_
    assert((((ui32)pc) & 3) == 0);
#endif

    h = len;
    c = (const ui32 *)pc;
    i = len / sizeof(ui32);

    while(i--)
        h ^= *c++;

    i = len % sizeof(ui32);
    if(i)
        h ^= (*c & mask[i]);   // ??? GPF ???

    return h;
}

//size_t stroka::hashVal(const char *s, size_t len COMMA CODEPAGE_ARGUMENT)
//{
//    size_t h = len;
//    for (; /* (*s) && */ len--; ++s)
//       h = 5*h + TO_LOWER(*s);
//    return h;
//}

Stroka &Stroka::replace(
    size_t pos,
    size_t del,
    const char *pc,
    size_t pos1,
    size_t ins,
    size_t len1) {
    size_t len = length();
    len1 = pc ? (len1 == npos ? (size_t)strlen(pc) : len1) : 0;
    POS_RANGE(pos, len);
    POS_RANGE(pos1, len1);
    NUM_RANGE(del, len, pos);
    NUM_RANGE(ins, len1, pos1);
    MAX_LENGTH(len - del, Stroka::max_size - ins);   // len-del+ins -- overflow

    size_t total = len - del + ins;
    if(!total) {
        relink(null);
        return *this;
    }
    size_t rem = len - del - pos;
    if(pdata()->refs != 1 || (pc && (pc >= p && pc < p + len))) {
        // malloc
        // 1. alias
        // 2. overlapped
        char *temp = allocate(total);
        memcpy(temp, p, pos);
        memcpy(temp + pos, pc + pos1, ins);
        memcpy(temp + pos + ins, p + pos + del, rem);
        relink(temp);
    } else if(reserve() < total) {
        // realloc (increasing)
        // 3. not enough room
        p = allocate(total, pdata());
        memmove(p + pos + ins, p + pos + del, rem);
        memcpy(p + pos, pc + pos1, ins);
    } else if(total + MAX_WASTE < reserve()) {
        // realloc (decreasing)
        // 4. too much room
        memmove(p + pos + ins, p + pos + del, rem);
        p = allocate(total, pdata());
        memcpy(p + pos, pc + pos1, ins);
    } else {
        // 1. not alias
        // 2. not overlapped
        // 3. enough room
        // 4. not too much room
        memmove(p + pos + ins, p + pos + del, rem);
        memcpy(p + pos, pc + pos1, ins);
        pdata()->nchars = total;
        p[total] = '\x00';
    }
    return *this;
}

//implemented in Stroka.h with strpbrk
//size_t find_first_of(const Stroka &set, size_t pos = 0) {
//    size_t len   = length();
//    pos          = POS_RANGE(pos, len);
//    size_t ret   = pos + strcspn(p + pos, set.p);
//    return (ret >= len) ? npos : ret;
//}

size_t Stroka::find_first_not_of(const Stroka &set, size_t pos) {
    size_t len = length();
    POS_RANGE(pos, len);
    size_t ret = pos + strspn(p + pos, set.p);   // was error in Borland 4.5
    return (ret >= len) ? npos : ret;
}

const size_t Stroka::npos = (size_t)(-1);
const size_t Stroka::max_size = npos - OVERHEAD;


bool Stroka::read_token_part(istream &is, char *buf, std::streamsize bufsize, char) {
    is.width(bufsize);
    is >> buf;
    if(!is.good())
        return true;
    if(isspace(is.peek()))
        return true;
    return false;
}

bool Stroka::read_line_part(istream &is, char *buf, std::streamsize bufsize, char delim) {
    is.get(buf, bufsize, delim);
    if(is.rdstate() & std::ios::failbit && !is.eof())
        is.clear();
    if(!is.good())
        return true;
    if(is.peek() == delim) {
        is.get();
        return true;
    }
    return false;
}

// from Doug Schmidt
// modified by iseg
#define CHUNK_SIZE 512
istream &Stroka::build(
    istream &is,
    bool (*f)(istream &, char *, std::streamsize, char),
    size_t &total,
    char delim) {
    char buf[CHUNK_SIZE];
    int enough = (*f)(is, buf, CHUNK_SIZE, delim);
    size_t count = (size_t)strlen(buf);
    size_t chunk_start = total;
    total += count;
    if(!enough)
        build(is, f, total, delim);
    else
        relink(allocate(total));
    memcpy(p + chunk_start, buf, count);
    return is;
}
