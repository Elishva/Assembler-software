
int IC,DC;/*counters of Instructions & Data*/ 
int inx_ent,inx_ext; /*index for entry table & extern table*/
int err_count; /*counter for number error */
pData t_data;  /* dymanic array for Data table (DC_Table)*/
pSymbol head=NULL;  /*pointer to head of list of symbol table*/
int mem_word[MEMORY_SIZE]; /*array for translated assmbler code */
int line_number; /*index for current line file read*/
pLabel ext_table,ent_table; /*dynamic array for entery & extern table*/


/*const struct of all optional command & thier options */
struct OpCode op_code[]= {
			{"mov",0,TWO_OP,{IMMEDIATE,DYNAMIC,DIRECT_REG,DIRECT},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"cmp",1,TWO_OP,{IMMEDIATE,DYNAMIC,DIRECT_REG,DIRECT},{IMMEDIATE,DYNAMIC,DIRECT_REG,DIRECT}},
			{"add",2,TWO_OP,{IMMEDIATE,DYNAMIC,DIRECT_REG,DIRECT},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"sub",3,TWO_OP,{IMMEDIATE,DYNAMIC,DIRECT_REG,DIRECT},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"not",4,ONE_OP,{NON_OPERAND},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"clr",5,ONE_OP,{NON_OPERAND},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"lea",6,TWO_OP,{DIRECT,NON_OPERAND},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"inc",7,ONE_OP,{NON_OPERAND},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"dec",8,ONE_OP,{NON_OPERAND},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"jmp",9,ONE_OP,{NON_OPERAND},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"bne",10,ONE_OP,{NON_OPERAND},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"red",11,ONE_OP,{NON_OPERAND},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"prn",12,ONE_OP,{NON_OPERAND},{IMMEDIATE,DYNAMIC,DIRECT_REG,DIRECT}},
			{"jsr",13,ONE_OP,{NON_OPERAND},{DIRECT_REG,DIRECT,NON_OPERAND}},
			{"rts",14,NO_OP,{NON_OPERAND},{NON_OPERAND}},
			{"stop",15,NO_OP,{NON_OPERAND},{NON_OPERAND}},
			{NULL}

		
};

/*const struct of register name & threrv code */
struct Reg registers[]= {
			{"r0",0},
			{"r1",1},
			{"r2",2},
			{"r3",3},
			{"r4",4},
			{"r5",5},
			{"r6",6},
			{"r7",7},
			{NULL}
		
};

/*const array for special base 8 code*/
char SpecialCode[]={"!@#$%^&*"};

