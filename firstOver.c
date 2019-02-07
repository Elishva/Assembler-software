#include "database.h"
#include "prototype.h"


/*first over: over on program at first time and check there is no error & build a symbol table for label
f- is pointer to file program that have to translate */

void first_over(FILE *f)
{
	
 	char str[MAX_LINE];
	char *word;
	char label[MAX_LINE];
	int index;
	int flag_lbl;

	IC=0;  
	DC=0;
	line_number=0;
	while(!feof(f) && fgets(str,MAX_LINE,f)!=NULL)
	{	
		line_number++;
		flag_lbl=FALSE;
		word=strtok(str,WHITE_SPACE); 
		if (!word) continue;  /* line is empty*/
		if (word[0]==REMARK_LINE) continue;  /*ignore from remark line*/
	
		if (isLabel(word))  /*check if is label*/
		{	
			word[strlen(word)-1]='\0';  /*delete charagter ':' */
			if(isCorrectLabel(word) && !isLabelExists(word)) /*check for correct label and check is label not already declared*/
			{	
				flag_lbl=TRUE;			
				strcpy(label,word);
				word=strtok(NULL,WHITE_SPACE);
				if (word==NULL) /*check if line after label not empty*/
				{
					printf("Error at line %d: Illegal use of label %s.\n",line_number,label);
					err_count++;
					continue;
				}
			}
			else continue;
		}
	
		if(isGuidStmt(word)) /*check if is guidnace statment*/
		{ 		 
			if (flag_lbl)  /*check if guidnace is after label*/
			{/*if guidnace is after label*/
				if  (isDataGuid(word)) /*check for .data or .string guidnace statment*/
				{
					add2list(&head,label,DC,DATA); /*add label to node */
					add_data(word);  /* add data or string to dimaanic array*/
				}
				else /*for .entery & .extern after label make warning*/
				printf("Warning at line %d: Illegal use of label %s.\n",line_number,str); 
			}
			else /*if guidnace is not after label*/
				add_guid_stmt(word);	/*add the details of guidnace statment in table*/
				
		}
		else if(isCommand(word,&index)) /*check if is a command - after label */
		{
			if (flag_lbl)
				add2list(&head,label,IC,CODE); /*add label to node */
			cal_num_word(index); /* calulate the number of word the command line take*/
		}
		else 
		{
			printf("Error at line %d: Illegal use of word %s, you perhaps forget sign : for label.\n",line_number,word);
			err_count++;
		}

	}/* end while*/
	
	/*check if program not exit from size of memory*/
	if (IC+DC>MEMORY_SIZE)
	{ 
		printf("Error: Exit from bounds of memory. maximum memory size is %d, memory program is %d\n",MEMORY_SIZE,IC+DC);
		err_count++;
	}
	
	update_address(head); /*update label adrress*/
	add_DC_table2mem();	/*add DC table to memory word*/
	
	
	/*print_list(head); *//*print sybol list - for debugs*/
	/*print_DC_table(); *//*print DC table - for debugs*/


}



/* isLabel: check if is label- label have to end by :(COLON)
str- is the word to be check if is label*/
int isLabel(char *str)
{
	if(str[strlen(str)-1]==CH_COLON) /*check if the last charagter is ':' */
		return TRUE;
	return FALSE;
}

/*isGuidStmt- check if is guidnace statment- there is 4 guidsts: data,string,entry,external.
str- is a string to be check is one of 4 guidnace statment*/
int isGuidStmt(char *str)
{
	/*DataGuid - check if is it a data or string stasment*/
	if((!isDataGuid(str)) && (!isEntryGuid(str)) && (!isExtrnGuid(str)))
		return FALSE;
	return TRUE;
}



/*isDataGuid - check if it is Data or String guidnace statment.
str- is a string to be check is Data or String guidnace statment*/
int isDataGuid(char *str)
{
	if((strcmp(str,DATA_GUID)==0) || (strcmp(str,STRING_GUID)==0))
		return TRUE;
	return FALSE;
}


/*isEntryGuid - check if it is Entry guidnace statment.
str- is a string to be check is Entry guidnace statment*/
int isEntryGuid(char *str)
{
	if(strcmp(str,ENTRY_GUID)==0 )
		return TRUE;
	return FALSE;
}


/*isExtrnGuid - check if it is External guidnace statment.
str- is a string to be check is External guidnace statment*/
int isExtrnGuid(char *str)
{
	if(strcmp(str,EXTREN_GUID)==0 )
		return TRUE;
	return FALSE;
}


/*isCommand - check if it is Command code, there is 16 command word.
str- is a string to be check is command
i- is a return integer indicate the place of command is table */
int isCommand(char *str,int *i)
{
	*i=0; 
	
	while(op_code[*i].func)
	{
		if(strcmp(str,op_code[*i].func)==0)
			return TRUE;
		(*i)++;
	}
	return FALSE;
}


/*isRegister - check if it is Register name, there is 8 Register.
str- is a string to be check if is Register.
index- is a return integer indicate the value code of Register */
int isRegister(char *str,int *value)
{
	int i=0; 
	
	while(registers[i].name)
	{
		if(strcmp(str,registers[i].name)==0)
		{
			*value=registers[i].value;
			return TRUE;
		}
		i++;
	}
	return FALSE;
	
}

/*isCorrectLabel: check if label write with all rules
str- is given label have to check if it's correct label*/
int isCorrectLabel(char *str)
{
	int i;

	if(strlen(str)>MAX_LABEL)  /*check if label is too long*/
	{	
		printf("Error at line %d: label too long, the maximun label is %d\n",line_number,MAX_LABEL-1);
		err_count++;
		return FALSE;
	}

	if(!isalpha(str[0]))		/*check if the first character is alpha betic*/
	{	
		printf("Error at line %d: Illegal label %s, label must begin with alpha betic character.\n",line_number,str);
		err_count++;
		return FALSE;
	}
	
	for(i=0;i<strlen(str);i++) /*check if label include only numric and alpha betic*/
		if(!isalnum(str[i]))
		{	
			printf("Error at line %d: Illegal label %s, label can only include numric and alpha betic character.\n",line_number,str);
			err_count++;
			return FALSE;
		}
	
	if (isCommand(str,&i))	/*check if label is not command name, i is not neccsery here*/
	{	
		printf("Error at line %d: Illegal label %s, label can't be a Command name.\n",line_number,str);
		err_count++;
		return FALSE;
	}
		
	if (isRegister(str,&i))/*check if label is not reister name, i is not neccsery here*/
	{	
		printf("Error at line %d: Illegal label %s, label can't be a Register name.\n",line_number,str);
		err_count++;
		return FALSE;
	}

	

	return TRUE;


}

/*isLabelExists- checking if label is exsist in table(label is already declare)
str- is the label to check*/
int isLabelExists(char *lbl)
{

	if (isExists(head,lbl))
	{
		printf("Error at line %d: label %s already declared.\n",line_number,lbl);
		err_count++;
		return TRUE;
	}
	return FALSE;
}

/*add_data: add array number or string to table
word- is the represent array number sepert by comma or string character to add to table*/
void add_data(char *word)
{
	char *str;
	char token[20];
	int i=0,n;
	int num;
	
	/*if is string guidnace statment goto add_string*/	
	if (strcmp(word,STRING_GUID)==0)
	{
		add_string();
		return;
	}

	/*if is number*/
	str=strtok(NULL,NEW_LINE);
	if(!str) /*.data without parameter*/
	{
		printf("Error at line %d: Data without numbers.\n",line_number);
		err_count++;
		return;
	}

	while(i<strlen(str))
	{ 
		n=0;
		if(str[0]==CH_MINUS || str[0]==CH_PLUS) n=1;			
		while(n<strlen(str) && isdigit(str[n++])); /*find where number end*/
		
		strncpy(token,str,n); /*save the number in token*/
		token[n]=CH_END_STRING;
	
		for(n--;n<strlen(str) && str[n]!=CH_COMMA && isspace(str[n]);n++); /*ignore from white space befor comma*/
		
		if(n+1<strlen(str) && str[n]!=CH_COMMA) /*check if is camma between numbers*/
		{
			printf("Error at line %d: Missing comma.\n",line_number);
			err_count++;
			return;
		}

		for(n++;n<strlen(str) && isspace(str[n]);n++); /*ignore from white space after comma */
		
		if(n<strlen(str) && str[n]==CH_COMMA) /*check if is to many camma between numbers*/
		{
			printf("Error at line %d: Too many comma.\n",line_number);
			err_count++;
			return;
		}

		str=str+n;
		i=0;
		/*convert to int & check if number in limits*/
		num=atoi(token);
		if(num<MIN_NUM || num>MAX_NUM)
		{
			printf("Error at line %d: number %d not in range(%d to %d)\n",line_number,num,MAX_NUM,MIN_NUM);
			err_count++;
			return;
		}
		
		add_DC_table(num);  /*add number to dynamic table*/
		
	}
	
}


/*add_string:add string to table of data*/
void add_string()
{
	int i=1;
	char *token;
	
	token=strtok(NULL,NEW_LINE);
	if(token==NULL)
	{
		printf("Error at line %d: Missing string \n",line_number);
		err_count++;
		return;
	}	

	/*check if string begging with "*/
	if(token[0]!= CH_DOUBLE_QUOTE)
	{
		printf("Error at line %d: Missing begging of quotation marks \n",line_number);
		err_count++;
		return;
	}
	
	while(token[i]!=CH_DOUBLE_QUOTE && i<strlen(token))
	{
		add_DC_table(token[i]);
		i++;
	}

	/*check if string ending with " */
	if(token[i]!=CH_DOUBLE_QUOTE)
	{
		printf("Error at line %d: Missing end of quotation marks \n",line_number);
		err_count++;
		return;
	}

	/*check if is no charcter after" */
	if(token[i]!=CH_DOUBLE_QUOTE)
	
	for(i++;i<strlen(token);i++)
		if (!isspace(token[i]))
		{
			printf("Error at line %d: There are character after quotation marks \n",line_number);
			err_count++;
			return;
		}
	
	add_DC_table(CH_END_STRING); /*char \0 end of sting*/
}

/*add_guid_stmt: add gundnace statmment to table
there is 4 statment :.data. entery .extern .string
word - is kind of statmnet*/ 
void add_guid_stmt(char *word)
{
	/*check is statment is .data or .string*/
	if(isDataGuid(word))
 		add_data(word);   /* add data or string to dimaanic array*/
	
	/*check is statment is .extern*/
	if(isExtrnGuid(word))
	{	
		word=strtok(NULL,WHITE_SPACE);
		if (isCorrectLabel(word) && !isLabelExists(word))
			add2list(&head,word,0,EXT); /*add external label to node */
		
	}
}


/*cal_num_word: clculate the number of words needed to special command
& check if it's paramentrs are correct & make the first word
i- is index of the command in table op_code*/
void cal_num_word(int i)
{
  	enum GROUP n=op_code[i].num_expect_op;/*get the number word for specific command. operand 2 register will check later*/
	char *str;
	char *st;
	Address_Method method,method1;
	unsigned int *pcmd=(unsigned int *) (&cmd_wrd);
	 
	/*making the first word*/	
	*pcmd=0;
	cmd_wrd.era=ABSOULT;
	cmd_wrd.opcode=op_code[i].code;
	cmd_wrd.opcode=op_code[i].code;
	cmd_wrd.group=op_code[i].num_expect_op;
	cmd_wrd.no_used=NOT_USE;
	
	/* command with no operand*/

	if(n==NO_OP)
	{ 
		cmd_wrd.dest_op=0; 
		cmd_wrd.source_op=0;
		mem_word[IC++]=*pcmd;
		str=strtok(NULL,WHITE_SPACE);
		if(str!=NULL)
		{
			printf("Error at line %d: %s not need any operand \n",line_number,op_code[i].func);
			err_count++;
		}
		return;
	}
	
	/*get all entire line & remove space white from the beggning*/
	str=strtok(NULL,WHITE_SPACE);
	while ((st=strtok(NULL,WHITE_SPACE))!=NULL)
		strcat(str,st);
	
	/* command with 1 operand*/
	if(n==ONE_OP)
	{
		if(!check_dest_op(str,i,&method))
		{
			printf("Error at line %d: Illegal Oparand %s.\n",line_number,str);
			err_count++;
			return;
		}
		
		cmd_wrd.source_op=0;
		cmd_wrd.dest_op=(unsigned int)method;
		mem_word[IC]=*pcmd;
		IC+=2;		
		return;
	}
	
	/* command with 2 operand*/
	if(n==TWO_OP)
	{ 
		char *token	;
	
		token=strtok(str,COMMA);
		if(token==NULL) return;
		str=strtok(NULL,WHITE_SPACE);	
		if(str==NULL)
		{
			printf("Error at line %d: Missing Oparand, Command %s need 2 operand.\n",line_number,op_code[i].func);
			err_count++;
			return;
		}
		/*checking to correct source operand*/	
		if(!check_source_op(token,i,&method))
		{
			printf("Error at line %d: Illegal Oparand %s.\n",line_number,str);
			err_count++;
			return;
		}
		/*checking to correct destination operand*/	
		if(!check_dest_op(str,i,&method1))
		{
			printf("Error at line %d: Illegal Oparand %s.\n",line_number,str);
			err_count++;
			return;
		}
		/*make first word of command*/
		cmd_wrd.source_op=(unsigned int)method;
		cmd_wrd.dest_op=(unsigned int)method1;
		mem_word[IC]=*pcmd;
		if (method==DIRECT_REG && method1==DIRECT_REG)
			IC+=2; /*if source & dest operand are register there is only 2 word */
		else
			IC+=3;
		

	}
	/*printf("pcmd=%x \n",*pcmd);*/
}



/*check_source_op:cheking for correct source operand, & return true if correct & return the method of it
word- is source operand
metod is return value indicate the type of address method
return true is is correct use of source operand*/
int check_source_op(char *word,int index,Address_Method *method)
{
	char str[MAX_LINE];
	int i;
	

	for(i=0;i<NUM_METHOD && op_code[index].source_method[i]!=NON_OPERAND;i++)
	{	
		strcpy(str,word);
		if(check_method(op_code[index].source_method[i],str))
		{
			*method=op_code[index].source_method[i]; /*get the type of addressing metod*/
			return TRUE;
		}
	}
	return FALSE;
}



/*check_source_op:cheking for correct dest operand, & return true if correct & return the method of it
word- is dest operand
metod is return value indicate the type of address method
return true is is correct use of dest operand*/
int check_dest_op(char *word,int index,Address_Method *method)
{
	int i;
	char str[MAX_LINE];
	
	
	for(i=0;i<NUM_METHOD && op_code[index].dest_method[i]!=NON_OPERAND;i++)
	{
		strcpy(str,word);
		if(check_method(op_code[index].dest_method[i],str))
		{
			*method=op_code[index].dest_method[i];
			return TRUE;
		}
	
	}
	return FALSE;
}



/*check_method: checking the method of source or destatin operand
method is number indicate the type addressing metod
str is operand to be check 
return true if check success*/
int check_method(Address_Method method,char * str)
{
	char *token;
	int not_use=0;

	
	switch(method)
	{
		case IMMEDIATE:/*check if is correct immediate addressing metod*/
						if(isCorrectNumber(str))
							return TRUE;	
						break;
		
		case DIRECT: /*check if is correct direct addressing metod*/
					token=strtok(str,WHITE_SPACE);
					if(isCorrectLabel(token))/*check if is correct label */
						return TRUE;	
					token=strtok(NULL,WHITE_SPACE);
					if(token!=NULL)
					{
						printf("Error at line %d: too many operands.\n",line_number);
						err_count++;
						return FALSE;
					}
					break;
		
		case DYNAMIC: /*check if is correct dymanic addressing metod*/
					if(isCorrectDynamicPrm(str))
						return TRUE;	
					break;
		
		case DIRECT_REG: /*check if is correct register addressing metod*/
					if(isRegister(str,&not_use))
						return TRUE;	
					break;

		case NON_OPERAND: break;
	}

	return FALSE;

}



/*isCorrectNumner: check if number is legal integer number in Direct Operand & is in limit.
str- is a string need to check if is legal number in Direct Operand & convert it to integer
return true if is correct number*/
int isCorrectNumber(char *str)
{
	int i;
	int n;
	char *token;
	

	token=strtok(str,WHITE_SPACE);
	/*check if number begin is #*/
	if(token[0]!=CH_PREFIX_NUM)
		return FALSE;
	
	for(i=1;i<strlen(token);i++) /*check if label include only numric and alpha betic*/
		if(!isdigit(token[i]) && token[1]!=CH_MINUS && token[1]!=CH_PLUS)
		{
			printf("Error at line %d:ilegal number %s, number need to be integer.\n",line_number,str);
			err_count++;
			return FALSE;
		}
		
	/*delete begin charater (#) from string*/
	strcpy(str,str+1);
	n=atoi(token);
	if(n<MIN_NUM_OP || n>MAX_NUM_OP) /*check if number is in limit*/
	{
		printf("Error at line %d: %d number is not in range \n",line_number,n);
		err_count++;
		return FALSE;
	}
	
	return TRUE;


}


/*isCorrectDynamicPrm:check is is correct dynamic operand
str- is operand to be check if is correct dymanic method
return true if is correct operand*/
int isCorrectDynamicPrm(char *str)
{
	char token[MAX_LABEL];
	char  *word;

	int i=0;
	int n,x;

	for(i=0;i<strlen(str) && str[i]!=CH_OPEN_BRACKET;i++);
	if(str[i]!=CH_OPEN_BRACKET)
		return FALSE;
	strncpy(token,str,i);
	token[i]=CH_END_STRING;

	if(!isCorrectLabel(token))
		return FALSE;
	
	for(i++;i<strlen(str) && str[i]!=CH_DASH_SEPARATOR && str[i]!=CH_CLOSE_BRACKET ;i++)
		if(!isdigit(str[i]) && !isspace(str[i]))
		{
			printf("Error at line %d:ilegal number %s, number need to be integer.\n",line_number,str);
			err_count++;
			return FALSE;
		}
	
	/*check for DASHSEPARATOR*/
	if(str[i]!=CH_DASH_SEPARATOR)
	{
		printf("Error at line %d:missing dash separator.\n",line_number);
		err_count++;
		return FALSE;
	}	

	strcpy(token,str); /* save str becuase using strtok*/
	word=strtok(token,BRACKETS);
	word=strtok(NULL,BRACKETS);
	n=atoi(word);
	/*check firest number if is in range 0-14*/
	if(n<FIRST_BIT || n>LAST_BIT)
	{
		printf("Error at line %d:ilegal bit number %d, number need to be between %d - %d.\n",line_number,n,FIRST_BIT,LAST_BIT);
		err_count++;
		return FALSE;
	}

	
	for(i++;i<strlen(str) && str[i]!=CH_CLOSE_BRACKET;i++)
		if(!isdigit(token[i]) && !isspace(token[i]))
		{
			printf("Error at line %d:ilegal number %s, number need to be integer.\n",line_number,str);
			err_count++;
			return FALSE;
		}
	
	/*check for end bracket*/
	if(str[i]!=CH_CLOSE_BRACKET)
	{
		printf("Error at line %d:missing close bracket.\n",line_number);
		err_count++;
		return FALSE;
	}	

		
	word=strtok(NULL,BRACKETS); 
	x=atoi(word);
	/*check second number if is beggier than first number & is less then LAST_BIT*/
	if(x<n || n>LAST_BIT)
	{
		printf("Error at line %d:ilegal bit number %d, number need to be big from %d & less then %d\n",line_number,x,n,LAST_BIT+1);
		err_count++;
		return FALSE;
	}

	/*check is difference bit is less-eqaul to 13*/
	if(x-n>DIF_BIT)
	{
		printf("Error at line %d:ilegal difference bit number, the maximum difference is %d\n",line_number,DIF_BIT);
		err_count++;
		return FALSE;
	}
	return TRUE;		
			
}

