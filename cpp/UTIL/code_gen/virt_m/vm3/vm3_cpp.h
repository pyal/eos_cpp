#ifndef __VM3_CPP_H
#define __VM3_CPP_H
#include <iostream>
#include <iomanip>

using namespace std;


extern "C"
#include "util\code_gen\virt_m\symtab.h"

    ostream &
    operator<<(ostream &out, symbol &sym) {
    return out << " Symbol: name" << setw(15) << sym.name << " type: " << sym.type
               << " val: " << sym.u.val << "\n";
};

struct symbol_io : symbol {
    //	friend ostream& operator<<(ostream &out,symbol_io &symIO){return out;};
};

//struct symbol
//{
//   char *name;          /* ��� ���������� ��� ������� */
//   int   type;          /* ���: VAR, BLTIN, UNDEF     */
//   union
//   {
//      double  val;       /* �������� ��� ����������( VAR )           */
//      double  (*ptr)();  /* ��������� �� ���������� �������( BLTIN ) */
//      int    (*defn)();  /* FUNCTION, PROCEDURE */
//      char   *str;       /* string */
//   } u;
//
//   struct symbol *next; /* ����� �� ��������� ������� */
//};


#endif