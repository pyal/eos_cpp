#ifndef _ANZUBR_USE_H
#define _ANZUBR_USE_H

#define modif_def int
#define type_def int
typedef struct z_symbol z_symbol;

struct z_symbol {
    modif_def *modif;
    type_def *type;
    //int *modif;
    //int *type;
    void *addr;
};
enum tetrade_code {
    PLUS_EXPR,
    MINUS_EXPR,
    MULT_EXPR,
    TRUNC_DIV_EXPR,
    TRUNC_MOD_EXPR,
    BIT_XOR_EXPR,
    BIT_AND_EXPR,
    BIT_IOR_EXPR,
    ANDIF_EXPR,
    ORIF_EXPR,
    LSHIFT_EXPR,
    RSHIFT_EXPR,
    LT_EXPR,
    GT_EXPR,
    LE_EXPR,
    GE_EXPR,
    NE_EXPR,
    EQ_EXPR,

    ADDR_EXPR,          /* unary  & */
    NEGATE_EXPR,        /* unary  - */
    CONVERT_EXPR,       /* unary  + */
    PREINCREMENT_EXPR,  /* unary ++ */
    PREDECREMENT_EXPR,  /* unary -- */
    POSTINCREMENT_EXPR, /* unary ++ */
    POSTDECREMENT_EXPR, /* unary -- */
    BIT_NOT_EXPR,       /* unary  ~ */
    TRUTH_NOT_EXPR,     /* unary  ! */

    NOP_EXPR,

    UNUSED_EXPR
};

#define program int


extern int zubr_lex(void);
extern void init_decl_prog();
extern void free_symbol_table();

extern void execute_program(program *cur);
extern void free_program(program *cur);
extern void goto_text_prog();
extern z_symbol *make_pointer_declarator(z_symbol *sym2);
extern z_symbol *build_array(z_symbol *sym1, z_symbol *sym2);   //250
extern z_symbol *build_indirect_ref(z_symbol *sym1, const char *str);
extern z_symbol *build_unary_op(enum tetrade_code sym1, z_symbol *sym2);
extern z_symbol *build_binary_op(enum tetrade_code sym1, z_symbol *sym2, z_symbol *sym3);
extern z_symbol *build_modify_expr(
    z_symbol *sym1,
    enum tetrade_code sym2,
    z_symbol *sym3);
extern z_symbol *build_mth_builtin1_fn(z_symbol *sym1, z_symbol *sym2);
extern z_symbol *build_mth_builtin2_fn(z_symbol *sym1, z_symbol *sym2, z_symbol *sym3);
extern z_symbol *build_array_ref(z_symbol *sym1, z_symbol *sym2);
extern z_symbol *build_chainon_string(z_symbol *sym1, z_symbol *sym2);
extern z_symbol *init_identifier(
    z_symbol *sym1,
    int current_declspec_type,
    int current_declspec_flag);
extern void init();


#endif


//           int type;

//           $1->type  = ( is_constant( current_declspec_flags ) ) ? CONSTANT : VARIABLE;

//           type = get_symtype( current_declspec_flags );
//           if( type == T_REAL )
//           {
//              /* real */
//              $1->u.v.t = T_REAL;
//              $1->u.v.data.v_real = 0.0;
//           }
//           else if( type == T_INT || type == T_UINT )
//           {
//              /* int */
//              $1->u.v.t = type;
//              $1->u.v.data.v_uint = 0;
//           }
//           else if( type == T_CHAR )
//           {
//              /* init as T_CHAR */
//              $1->u.v.t = type;
//              $1->u.v.data.v_char = 0;
//           }
//           else
//           {
//              fprintf( stderr,
//              "%s: %s:%d: ERROR: incompatible type of initialiser constant\n",
//                       progname, input_file_name, lineno );
//              exit( 1 );
//           }
////           current_declspec_flags = 0;

//           $$ = $1;


//
//
//
///**************************************************************
//  SYMTAB.H
//              Copyright (C) 1995 by Andrew Kosteltsev
//              All Rights Reserved
// **************************************************************/
//
//#ifndef __SYMTAB_H__
//#define __SYMTAB_H__   1
//
//
//#define T_CHAR           0x10 /*      uint8 */
//#define T_CHAR_PTR       0x11 /*  (uint8 *) */
//
//#define T_INT            0x20 /*      int32 */
//#define T_INT_PTR        0x21 /*  (int32 *) */
//
//#define T_UINT           0x40 /*     uint32 */
//#define T_UINT_PTR       0x41 /* (uint32 *) */
//
//#define T_REAL          0x400 /*     real64 */
//#define T_REAL_PTR      0x401 /* (real64 *) */
//
//
//
//typedef struct variant variant;
//struct variant
//{
//   unsigned long int   t; /* type of data */
//
//   union
//   {
//      unsigned           char  v_char;    /* for  uint8 */
//
//        signed long      int   v_int;     /* for  int32 */
//      unsigned long      int   v_uint;    /* for uint32 */
//
//               double          v_real;    /* for real64 */
//
//                         void *v_ptr;     /* for array or string */
//
//   } data;
//};
//
///* the structure of a symbol table entry */
//typedef struct bucket bucket;
//struct bucket
//{
//   struct bucket *uplink;
//   struct bucket *link;
//
//   struct bucket *prev;
//   struct bucket *next;
//
//   char *id;
//
//   int   type;              /* ASM_CODE, VARIABLE, STRING, ... */
//   int   tmp_ref;           /* for tmp reference to ARRAY */
//   int   tmp_flag;          /* for tmp VARIABLE and CONSTANT */
//
//   union
//   {
//      struct variant v;     /* for data of VARIABLE */
//
//      void * (*ptr)();      /************************************
//                              for ASM_CODE: NOTE:
//                              ARITHMETIC operations return( 0 ),
//                              GOTO operation return( tetrade * )
//                              for set the program->ptr_tetrade
//                             ************************************/
//
//      void *label;          /* ptr to tetrade for GOTO */
//
//   } u;
//};
//
//extern void *xmalloc( unsigned n );
//extern void *xrealloc( void *b, unsigned n );
//
//
//extern bucket *lookup( char *id );
//extern bucket *install( char *id, int t, bucket *b );
//extern int delete_symbol( char *id );
//
//extern void create_symbol_table( void );
//extern void free_symbol_table( void );
//
//
//extern void init_labels( void );
//
//
//
//extern void printf_symbol_table( void );
//
//
//
//#endif /* __SYMTAB_H__ */
//
///**********************  END OF SYMTAB.H  *********************/


//
//%{
//#include "zubr_use.h"
//
//
//
//char *progname;
//
//FILE *input_file;
//char *input_file_name;
//
//int lineno;
//
//program *current_prog;
//
//static unsigned long current_declspec_flags;
//static unsigned long declspec_stack;
//
//%}
//
//%union
//{
//   unsigned long      itype;
//   z_symbol          *sym;
//   enum tetrade_code  code;
//}
//
//%right <code> ASSIGN 907 '='
//%right <code> '?' ':'
//%left  <code> OROR 901
//%left  <code> ANDAND 902
//%left  <code> '|'
//%left  <code> '^'
//%left  <code> '&'
//%left  <code> EQCOMPARE 906
//%left  <code> ARITHCOMPARE 905
//%left  <code> LSHIFT 910  RSHIFT 911
//%left  <code> '+' '-'
//%left  <code> '*' '/' '%'
//%right <code> UNARY 908  PLUSPLUS 903  MINUSMINUS 904
//%left  HYPERUNARY 909
//%left  <code> POINTSAT 912 '.' '(' '['
//
//
//%type <code> unop
//
//
//%token <itype> TYPESPEC 800   SCSPEC 801  TYPE_QUAL 802
//
//
//%token <sym> IDENTIFIER 500 CONSTANT 502 VARIABLE 503
//%token <sym> STRING 505
//%token <sym> MTH_BUILTIN 600
//
//
//%type <itype> setspecs
//%type <itype> declmods typespec typed_declspecs reserved_declspecs
//%type <itype> typespequal_reserved type_quals
//
//%type <sym> expr nonull_exprlist expr_no_commas cast_expr primary unary_expr
//%type <sym> string
//%type <sym> init initdcl
//%type <sym> declarator notype_declarator
//
//%start program
//%%
//program:          { ; /* finish file */ }
//       | extdefs
//       ;
//
//extdefs:
//         extdef
//       | extdefs extdef
//       ;
//
//extdef:
//         datadef
//       ;
//
//datadef: setspecs notype_initdecls ';'
//         {
//            current_declspec_flags = 1; /* RID_INT flags */
//            fprintf( stderr,
//            "%s: %s:%d: WARNING: data definition with no type or storage class\n",
//                     progname, input_file_name, lineno );
//         }
//       | declmods setspecs notype_initdecls ';'
//         {
//            current_declspec_flags = declspec_stack;
//            declspec_stack = 0;
//         }
//       | typed_declspecs setspecs initdecls ';'
//         {
//            current_declspec_flags = declspec_stack;
//            declspec_stack = 0;
//         }
//       | declmods ';'
//         {
//            current_declspec_flags = 0;
//            declspec_stack = 0;
//            fprintf( stderr,
//            "%s: %s:%d: WARNING: empty declaration\n",
//                     progname, input_file_name, lineno );
//         }
//       | error ';' { current_declspec_flags = 0; zubr_errok; }
//       | ';'
//         {
//            current_declspec_flags = 0;
//            declspec_stack = 0;
//            fprintf( stderr,
//            "%s: %s:%d: WARNING: not allow extra ';' outside of a function\n",
//                     progname, input_file_name, lineno );
//         }
//       ;
//
//declmods:
//         TYPE_QUAL
//         { $$ = (declspec_stack |= $1); }
//       | SCSPEC
//         { $$ = (declspec_stack |= $1); }
//       | declmods TYPE_QUAL
//         { $$ = (declspec_stack |= $2); }
//       | declmods SCSPEC
//         {
//            fprintf( stderr,
//            "%s: %s:%d: WARNING: storage class specification is not at beginning of declaration\n",
//                     progname, input_file_name, lineno );
//            $$ = (declspec_stack |= $2);
//         }
//       ;
//
//typespec:
//         TYPESPEC
//         { $$ = (declspec_stack |= $1); }
//       ;
//
//typed_declspecs:
//         typespec reserved_declspecs
//         {
//            $$ = (declspec_stack |= $2);
//            current_declspec_flags = declspec_stack;
//            declspec_stack = 0;
//         }
//       | declmods typespec reserved_declspecs
//         {
//            $$ = (declspec_stack |= $3);
//            current_declspec_flags = declspec_stack;
//            declspec_stack = 0;
//         }
//       ;
//
//reserved_declspecs: /* empty */
//         { $$ = (declspec_stack |=  0); }
//       | reserved_declspecs typespequal_reserved
//         { $$ = (declspec_stack |= $2); }
//       | reserved_declspecs SCSPEC
//         { $$ = (declspec_stack |= $2); }
//       ;
//
//typespequal_reserved:
//         TYPESPEC
//         { $$ = (declspec_stack |= $1); }
//       | TYPE_QUAL
//         { $$ = (declspec_stack |= $1); }
//       ;
//
//type_quals:
//         /* empty */
//         { $$ = (declspec_stack |=  0); }
//       | type_quals TYPE_QUAL
//         { $$ = (declspec_stack |= $2); }
//       ;
//
//initdecls:
//         initdcl
//       | initdecls ',' initdcl
//       ;
//
//notype_initdecls:
//         notype_initdcl
//       | notype_initdecls ',' initdcl
//       ;
//
//initdcl:
//         declarator '='
//         {
//            init_decl_prog(); /* set current_prog to init_declaration_prog */
//         }
//         init
//         {
//            $$ = build_modify_expr( $1, NOP_EXPR, $4 );
//            execute_program( current_prog ); /* exec init_declaration_prog */
//            /* DELETE TMP VARIABLE & CONSTANT */
//            free_program( current_prog ); /* FREE init_declaration_prog */
//            goto_text_prog(); /* RETURN TO GENERATION TEXT PROGRAM */
//         }
//       | declarator
//       ;
//
//notype_initdcl:
//         notype_declarator '=' init
//       | notype_declarator
//       ;
//
//declarator:
//         notype_declarator
//       ;
//
//notype_declarator:
//         '*' type_quals notype_declarator %prec UNARY
//         {
//            $$ = make_pointer_declarator( $2, $3 );
//         }
//       | notype_declarator '[' expr ']' %prec '.'
//         {
//            $$ = build_array( $1, $3 );
//         }
//       | notype_declarator '[' ']' %prec '.'
//         {
//            $$ = build_array( $1, (z_symbol *)0 );
//         }
//       | IDENTIFIER
//         {
//			$$ = init_identifier(current_declspec_flags);
//			current_declspec_flags=0;
//         }
//       ;
//
//setspecs: /* empty */
//         { $$ = $<itype>0; }
//       ;
//
//init:
//         expr_no_commas
//         { $$ = $1;}
//       ;
//
//
//
//
//unop:
//         '&'        { $$ = ADDR_EXPR;         }
//       | '-'        { $$ = NEGATE_EXPR;       }
//       | '+'        { $$ = CONVERT_EXPR;      }
//       | PLUSPLUS   { $$ = PREINCREMENT_EXPR; }
//       | MINUSMINUS { $$ = PREDECREMENT_EXPR; }
//       | '~'        { $$ = BIT_NOT_EXPR;      }
//       | '!'        { $$ = TRUTH_NOT_EXPR;    }
//       ;
//
//expr:
//         nonull_exprlist
//       ;
//
//nonull_exprlist:
//         expr_no_commas
//       | nonull_exprlist ',' expr_no_commas
//                             { $$ = $3; }
//       ;
//
//unary_expr:
//         primary
//       | '*' cast_expr %prec UNARY
//         { $$ = build_indirect_ref( $2, "unary *" ); }
//       | unop cast_expr %prec UNARY
//         { $$ = build_unary_op( $1, $2 ); }
//       ;
//
//cast_expr:
//         unary_expr
//       ;
//
//expr_no_commas:
//         cast_expr
//       | expr_no_commas '+' expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas '-' expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas '/' expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas '*' expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas '%' expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas '&' expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas '|' expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas '^' expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas LSHIFT expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas RSHIFT expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas ARITHCOMPARE expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas EQCOMPARE expr_no_commas
//                { $$ = build_binary_op( $2, $1, $3 ); }
//       | expr_no_commas ANDAND expr_no_commas
//                { $$ = build_binary_op( ANDIF_EXPR, $1, $3 ); }
//       | expr_no_commas OROR expr_no_commas
//                { $$ = build_binary_op( ORIF_EXPR, $1, $3 ); }
//       | expr_no_commas '=' expr_no_commas
//                { $$ = build_modify_expr( $1, NOP_EXPR, $3 ); }
//       | expr_no_commas ASSIGN expr_no_commas
//                { $$ = build_modify_expr( $1, $2, $3 ); }
//       | MTH_BUILTIN '(' expr_no_commas ')'
//         {
//            $$ = build_mth_builtin1_fn( $1, $3 );
//         }
//       | MTH_BUILTIN '(' expr_no_commas ',' expr_no_commas ')'
//         {
//            $$ = build_mth_builtin2_fn( $1, $3, $5 );
//         }
//       ;
//
//primary: VARIABLE
//       | CONSTANT
//       | string
//       | '(' expr ')'
//         { $$ = $2; }
//       | primary '[' expr ']' %prec '.'
//         {
//            $$ = build_array_ref( $1, $3 );
//         }
//       | primary PLUSPLUS
//         { $$ = build_unary_op( POSTINCREMENT_EXPR, $1 ); }
//       | primary MINUSMINUS
//         { $$ = build_unary_op( POSTDECREMENT_EXPR, $1 ); }
//       ;
//
//string:
//         STRING
//       | string STRING
//         { $$ = build_chainon_string( $1, $2 ); }
//       ;
//
//
//
//
//
//%%
///*********************  END OF GRAMMAR  ***********************/
//
//
//void zubr_error( char *s )
//{
//   printf( "%s\n" );
//}
//
//int main( int argc, char *argv[] )
//{
//   int c;
////   bucket b, *fn, *rc, *op1, *op2;
//
//
//   progname = argv[0];
//   if (argc < 2)
//   {
//      fprintf( stderr, "usage: %s file.name\n", progname );
//      exit( 1 );
//   }
//
//   input_file_name = argv[1];
//
//   if( (input_file = fopen( input_file_name, "rb" )) == NULL )
//   {
//      printf( "Cannot open file: %s\n", input_file_name );
//      exit( 1 );
//   }
//
//   init();
//
//   zubr_parse();
//
//   if( input_file ) fclose( input_file );
//
//
//   execute_program( current_prog );
//
//
//   free_program( current_prog );
//   free_symbol_table();
//
//
//   return( 0 );
//}
//
///**********************  END OF MAIN.Y  ***********************/
//
