#include "var.h"


/*structure of coammnd word*/
struct CommandWord
{

	unsigned int era:2;    /*first two bits to ERA code */
	unsigned int dest_op:2; /* bits 2-3 for code of dest addreessing method */
	unsigned int source_op:2; /* bits 4-5 for code of source addreessing method */
	unsigned opcode:4;	/* bits 6-9 for code of opeation code */
	unsigned group:2;	/* bits 10-11 for group  code */
	unsigned no_used:3;	/* bits 12-15 not use always be 5(101b) */
	
}cmd_wrd;


/*structure for 2'nd & 3'rd word*/
struct OtherWord
{

	unsigned int era:2;
	unsigned int dest_reg:6;
	unsigned int source_reg:6;
	unsigned int no_used:1;
	
}other_word;


/*structure for opration  code*/
struct OpCode{

	char *func;  /*name of function*/
	int code;	/*the op code of function */	
	int num_expect_op; /* the number of operands in this function */
	Address_Method source_method[NUM_METHOD]; /* the source addressing method of each function */
	Address_Method dest_method[NUM_METHOD];	/* the destanation addressing method of each function */

};


/*structure for registers*/
struct Reg{
	char *name; /*name of register*/
	int value; /*the vakue of each register */
};


/*structure for list symbols label*/
typedef struct SymbolTable{
	char symbol[MAX_LABEL];
	int address; /*the IC address*/
	enum ECD status; /*indicate if adrress is code, data or external*/
	pSymbol next;

}symbol;


/*structure for data */
typedef struct DataTable{
	
	int address; /*the address that data will be set into*/
	int data;  /*number is comlete 2,and asccii charager*/

}tData;


/*structure for external & entry table*/
typedef struct LabelTable{
	
	char name[MAX_LABEL]; /*name label*/
	int address;		/*the addrees of label*/
	
}tLabel;


/*structure to convert number to 8 base code*/
struct BitsFormat{
 
	unsigned int ch1:3;
	unsigned int ch2:3;
	unsigned int ch3:3;
	unsigned int ch4:3;
	unsigned int ch5:3;

}BitFormat;

