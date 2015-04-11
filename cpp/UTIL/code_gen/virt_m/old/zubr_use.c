#include "zubr_use.h"

int zubr_lex( void ){return 0;}
//extern int *current_prog;
void init_decl_prog(){}; /* set current_prog to init_declaration_prog */
void free_symbol_table(){};

void execute_program( program *cur ){};//203
            /* DELETE TMP VARIABLE & CONSTANT */
void free_program( program *cur  ){}; /* FREE init_declaration_prog */
void goto_text_prog(){}; /* RETURN TO GENERATION TEXT PROGRAM */
z_symbol* make_pointer_declarator( z_symbol *sym2 ){return 0;};//234
z_symbol* build_array(z_symbol *sym1, z_symbol *sym2 ){return 0;};//250
z_symbol* build_indirect_ref( z_symbol *sym1, const char *str ){return 0;};
z_symbol* build_unary_op(enum tetrade_code sym1, z_symbol *sym2 ){return 0;};
z_symbol* build_binary_op(enum tetrade_code sym1, z_symbol *sym2, z_symbol *sym3 ){return 0;};
z_symbol* build_modify_expr(z_symbol *sym1,enum tetrade_code sym2, z_symbol *sym3 ){return 0;};
z_symbol* build_mth_builtin1_fn(z_symbol *sym1, z_symbol *sym2 ){return 0;};
z_symbol* build_mth_builtin2_fn(z_symbol *sym1, z_symbol *sym2, z_symbol *sym3 ){return 0;};
z_symbol* build_array_ref(z_symbol *sym1, z_symbol *sym2 ){return 0;};
z_symbol* build_chainon_string(z_symbol *sym1, z_symbol *sym2 ){return 0;};
z_symbol* init_identifier(z_symbol *sym1,int current_declspec_type,int current_declspec_flag){return 0;};
void init(){};
