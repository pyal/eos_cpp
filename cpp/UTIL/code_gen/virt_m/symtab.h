
/**************************************************************
  SYMTAB.H
 **************************************************************/

/*************************************
  ��������� ������ � ������� ��������
 *************************************/
typedef struct symbol SYMBOL;
//#include "..\vm3_cpp.h"
struct symbol {
    char *name; /* ��� ���������� ��� ������� */
    int type;   /* ���: VAR, BLTIN, UNDEF     */
    union {
        double val;      /* �������� ��� ����������( VAR )           */
        double (*ptr)(); /* ��������� �� ���������� �������( BLTIN ) */
        int (*defn)();   /* FUNCTION, PROCEDURE */
        char *str;       /* string */
    } u;

    struct symbol *next; /* ����� �� ��������� ������� */
};

/************************************************************
  ���������� ������� ���������� ������ � ������ ������������
 ************************************************************/
extern SYMBOL *install(char *s, int t, double d);
extern SYMBOL *lookup(char *s);

extern char *emalloc(unsigned n);


/******************  END OF FILE SYMTAB.H  ********************/
