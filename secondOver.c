#include "database.h"
#include "prototype.h"



/*second_over: second over on program' making full code
f-poiter to assembly file*/
void second_over(FILE *f)
{
	char str[MAX_LINE];
	char *word;
	int index;
	
	line_number=0;
	IC=0;

	while(!feof(f) && fgets(str,MAX_LINE,f)!=NULL)
	{			
		line_number++;
		word=strtok(str,WHITE_SPACE); 
		if (!word) continue;  /* line is empty, ignore from white space line*/
		if (word[0]==REMARK_LINE) continue;  /*ignore from remark line*/
		/*check if is label, if yes take the next word in line*/
		if (isLabel(word))
		{
			word=strtok(NULL,WHITE_SPACE);
			if (word==NULL) continue; /*ignore(in 2'nd over) from label without any command or guidnace stament*/
		}

		/*check if it is guidnace statment- in 2'nd over relate only to .entry guid*/
		if(isGuidStmt(word))
		{ 		 
			if (isEntryGuid(word)) /*check for .Entry guidnace statment*/
			{
				word=strtok(NULL,WHITE_SPACE);
				/*check if is no address label for entry guidnace stms*/
				if(word==NULL)
				{	
					printf("Error at line %d: .entry has no label .\n",line_number);
					err_count++;
					continue;		
				}

				/*check if is label declared*/
				if (!isExists(head,word))
				{	
					printf("Error at line %d: label %s is not declared.\n",line_number,word);
					err_count++;
					continue;		
				}
			
				/*add new list to entry table*/
				ent_table=(pLabel)realloc(ent_table,(inx_ent+1)*sizeof(tLabel));
				if(!ent_table)
				{
					printf("\n cannot have build a list");
					exit(0);
				}
				strcpy(ent_table[inx_ent].name,word);
				ent_table[inx_ent].address=get_address(head,word);
				inx_ent++;
			 }					
		}
		else if(isCommand(word,&index))
		{
			build_word(index);
		 	
		}
	
	}
	
	
}

/*build_word: make the second & third word for command
index- the index to relative command in table op_code*/
void build_word(int index)
{
	char *str,*token;
	Address_Method method,method1;
	unsigned int *pcmd=(unsigned int *) (&cmd_wrd);
	unsigned int *pother=(unsigned int *) (&other_word);
	
	*pcmd=0;
	*pother=0;
	IC++;  /*word1 already decode in first over*/

	switch (op_code[index].num_expect_op)
	{
		/*command with no operand- the first word make in first over*/
		case NO_OP:
				break;
		
		/*command with 1 operand: have two word, the first word make in first over, & second make here*/
		case ONE_OP:
				/*get operand*/
				str=strtok(NULL,WHITE_SPACE);
				while ((token=strtok(NULL,WHITE_SPACE))!=NULL)
					strcat(str,token);
				
				check_dest_op(str,index,&method);/*find the metod of operand*/
				decode_second_word(str,index,method,TRUE);/*make the word*/
				mem_word[IC++]=*pother;/*save the code in table array*/
				break;

		/*command with 2 operand have 3 or 2 woed:first maake in frist over, the 2'nd 3'rd make here*/
		case TWO_OP:
				/*get all entire line*/
				str=strtok(NULL,WHITE_SPACE);
				token=strtok(NULL,NEW_LINE);
				if (token!=NULL) 
					strcat(str,token);
				token=strtok(str,COMMA); /*save in token the first operand*/
				if (token==NULL) return; /*check if operands seperate with comma, if not exit function*/
				str=strtok(NULL,WHITE_SPACE); /*save in str the s'nd operand*/	
				if (str==NULL) return;  /*check if there is only 1 operand, if not exit function*/
				
				check_source_op(token,index,&method); /*get the method of first operand -surce operand*/
				check_dest_op(str,index,&method1);  /*get the method of second operand -dest operand*/
				
				/*if both operand are register there is only 2 word (first make in first over)*/			
				if (method==DIRECT_REG && method1==DIRECT_REG)
				{ 
					decode_second_word(token,index,method,FALSE); /*make the code for surce register*/
					decode_second_word(str,index,method1,TRUE); /*make the code for dest register*/
					mem_word[IC++]=*pother; /*save the word in array*/
				}
				else
				{	/*make the 2'nd & 3'rd words*/
					decode_second_word(token,index,method,FALSE);
					mem_word[IC++]=*pother;
					*pother=0;
					decode_second_word(str,index,method1,TRUE);
					mem_word[IC++]=*pother;
				}
				
			break;

	}
	
}

/* decode_second_word:decode the second & third word
str is operand to decode 
index- is index of command in op_code table 
method -is the addressing method of operand
isdest- in bolleat indicate if operand is suorce or dest(TRUE=Dest) */
void decode_second_word(char *str,int index,Address_Method method,int isdest)
{	
	int num;
	unsigned int *pother=(unsigned int *) (&other_word);

	
	switch(method)
	{
		case IMMEDIATE:	/*Immediate addressing*/		
						strcpy(str,str+1); /*ingnore frpm # at the begging of string*/			
						num=atoi(str);
						if(num<0)
							*pother=(unsigned int)(((NUM_OP_13BITS+num)<<2)+ABSOULT); /*complete to 2*/
						else
							*pother=(unsigned int)((num<<2)+ABSOULT);
						break;
		
		case DIRECT: /*Direct addressing*/					
					if (!isExists(head,str)) /*check if label exists*/
					{	
						printf("Error at line %d: label %s is not declared.\n",line_number,str);
						err_count++;
						return;		
					}
					/*check if label is external*/
					num=get_status(head,str);
					*pother=(get_address(head,str)<<2)+num;
					if (num==EXTERNAL)
					{ /*if label extrnal add it to table external*/
						ext_table=(pLabel)realloc(ext_table,(inx_ext+1)*sizeof(tLabel));
						if(!ext_table)
						{
							printf("\n cannot have build a list");
							exit(0);
						}
						strcpy(ext_table[inx_ext].name,str);
						ext_table[inx_ext].address=IC+START_ADDRESS;
						inx_ext++;
				 	 }
					 break;
		
		case DYNAMIC:/*dynamic addressing*/
					str=strtok(str,BRACKETS);
					if (!isExists(head,str)) /*check if label declar*/
					{	
						printf("Error at line %d: label %s is not declared.\n",line_number,str);
						err_count++;
						return;		
					}
					num=atoi(strtok(NULL,BRACKETS));/*get the start bit*/
					*pother=get_dynamic_num(num,atoi(strtok(NULL,BRACKETS)),str);
					break;
		
		case DIRECT_REG:/*register addrsing*/										
					other_word.era=ABSOULT;
					isRegister(str,&num);  /*not nrrd to be check in second over, but use to get value*/
					/*for dest rgister number will be in 2-7 bit,for source register it will in 8-13*/
					if (isdest==TRUE)
						other_word.dest_reg=(unsigned int)num; 
					else
						other_word.source_reg=(unsigned int)num; 
					break;

		case NON_OPERAND: break;
	}


}


/*get_dynamic_num: find the number of dymanic addressing
num1-if the first bit to start
num2-is the last bit wanted
str-is the label to addressing
return unsigned int- the number found*/

unsigned int get_dynamic_num(int num1,int num2,char *str)
{
	unsigned int n;
	int bits=0x7fff; /*15 bits in 1*/
	int ad,sign;	
	int num_bits=num2-num1;

	ad=get_address(head,str)-START_ADDRESS; /*get address of label*/
	n=(bits<<num1) & ~(bits<<(num2+1)) & bits;/* make bits betwwen num1 to num2 in 1 others in 0*/
	n=n&mem_word[ad]; /*get the bits of data address */
	sign=n&(1<<num2); /*check if number is posstive or nagative by checking bit num2*/	
	n=n>>num1; /* rotate right num1 bits-to delete rigth's 0*/	
	if (sign!=0) 
	{	
		n=(1<<(num_bits+1))-n;  /*convert negative number to positive number */
		n=NUM_OP_13BITS-n; /*convert to negative number in 13 bits */
		n=(n&bits)<<2; /* delete addtional 1  & rotate 2 bits left*/
	}
	else 
		n=n<<2; /* rotate 2 bits left*/
	
	
	return n;
}
