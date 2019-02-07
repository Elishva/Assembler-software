#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include  <math.h>

#define NOT_USE 5
#define BASE8_CODE 6 /*number of character of word in special base 8 include '/0'*/
#define MAX_LINE 100
#define MAX_LABEL 31  /*include character ':', char '\0' not include in strlen() */
#define MAX_NAME_FILE 20
#define NUM_METHOD 4
#define START_ADDRESS 100
#define MEMORY_SIZE 1000

#define WHITE_SPACE " \n\t"
#define COMMA ","
#define NEW_LINE "\n"
#define BRACKETS "[]-"
#define CH_NEW_LINE '\n'
#define CH_OPEN_BRACKET '['
#define CH_CLOSE_BRACKET ']'
#define CH_DASH_SEPARATOR '-'
#define CH_COMMA ','
#define CH_DOUBLE_QUOTE '\"'
#define CH_END_STRING '\0'
#define PREFIX_NUM "#"
#define CH_PLUS '+'
#define CH_MINUS '-'
#define CH_COLON ':'
#define CH_PREFIX_NUM '#'
#define REMARK_LINE ';'

#define STRING_GUID ".string"
#define DATA_GUID ".data"
#define ENTRY_GUID ".entry"
#define EXTREN_GUID ".extern"

#define FIRST_BIT 0
#define LAST_BIT 14
#define DIF_BIT 13
#define MAX_NUM_OP 4095  /* 2^12-1 */
#define MIN_NUM_OP -4096 /*  2^12 */
#define NUM_OP_13BITS 8192  /*  2^13 */
#define MAX_NUM 16383  /* 2^14-1 */
#define MIN_NUM -16384 /*  2^14 */
#define NUM_15BITS 32768  /*  2^15 */

#define SUFFIX_AS ".as"
#define SUFFIX_EXT ".ext"
#define SUFFIX_ENT ".ent"
#define SUFFIX_OB ".ob"

enum GROUP {NO_OP=0,ONE_OP,TWO_OP};
enum ADDRESS_METHOD {NON_OPERAND=-99,IMMEDIATE=0,DIRECT,DYNAMIC,DIRECT_REG};
enum ERA {ABSOULT=0,EXTERNAL,RELOCATABLE};
enum ECD {EXT=0,CODE,DATA};
enum Boolean {FALSE=0,TRUE};

typedef struct SymbolTable *pSymbol;
typedef struct DataTable *pData;
typedef struct OpCode Op_Code;
typedef struct Reg Registers;
typedef enum ADDRESS_METHOD Address_Method;
typedef struct LabelTable *pLabel;

