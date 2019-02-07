#include "database.h"
#include "prototype.h"


/*add2list: add data to node at the first list
phead- is a pointer to list
str is symbol (name label) to add
int address the IC address when label declare
ecd is enum type isdicate if address is external ,code or data
*/
void add2list(pSymbol * phead,char *str,int address,enum ECD ecd)
{
	pSymbol p;
	p=(pSymbol)malloc(sizeof(symbol));
	strcpy(p->symbol,str);

	if (ecd==EXT)
		p->address=address;  /*for erternal address, value address cooming with 0*/
	else
		p->address=address+START_ADDRESS; /*add offset to address*/
	
	p->status=ecd; 
	
	/*make a node*/
	if(phead==NULL)
	{	
		p->next=NULL;
		*phead=p;
	}
	else
	{
		p->next=*phead;
		*phead=p;
	}
	
}


/*isExists: check if label exsist in list(label already declared)
p pointer to list
name- string label as to be check
return true is label exsists
*/
int isExists(pSymbol p,char *name)
{
	
	while(p)
	{
		if(strcmp(p->symbol,name)==0)
			return TRUE;
		p=p->next;
	}
	return FALSE;
	
}



/*print_list: print all list - this function need for debugs*/
void print_list(pSymbol head)
{
	while(head)
	{
		printf("label:%s\taddress:%d\ts:%d\n",head->symbol,head->address,head->status);
		head=head->next;
	}	
}


/*update_address: update the address of label in status data to real location
head is poiter to head of list*/
void update_address(pSymbol head)
{
	while(head)
	{	/*update the address of type data to exact location*/
		if(head->status==DATA)
			head->address=head->address+IC;
		head=head->next;
	}	
}


/*get_address: get the addrres of label
head is pointer to head of list
str is a label her address wanted
return the address of label*/
unsigned int get_address(pSymbol head ,char *str)
{

	while(head && strcmp(head->symbol,str)!=0)
		head=head->next;

	return (unsigned int )head->address;

}


/*get_status: get status of str(dtat, code, external
head- is pointer to head of list
str is string label her status wanted
return the status of label*/
unsigned int get_status(pSymbol head ,char *str)
{

	while(head && strcmp(head->symbol,str)!=0)
		head=head->next;
	
	if(head->status==EXT) 
		return (unsigned int )EXTERNAL;
	else
		return (unsigned int )RELOCATABLE;

}


/*free_list: free memory 
head is poiter to head of list*/
void free_list(pSymbol * phead)
{
	pSymbol p;
	while(*phead)
	{	
		p=*phead;
		*phead=(*phead)->next;
		free(p);
	
	}
}




/*print DC_table -for debugs*/
void print_DC_table()
{
	int i;
	for(i=0;i<DC;i++)
		printf("data:%d\taddress:%d\n",t_data[i].data,t_data[i].address);
		
}


/*add_DC_table:add DC data to dymanic array
data is data to add*/
void add_DC_table(int data)
{
	t_data=(pData)realloc(t_data,(DC+1)*sizeof(tData));
	if(!t_data)
	{
		printf("\n cannot have build a list");
		exit(0);
	}
	/*number <0 make complete to 2*/
	if (data<0)
		t_data[DC].data=NUM_15BITS+data; /*convert to complete 2*/
	else
		t_data[DC].data=data;
	t_data[DC].address=DC;
	DC++;
	
}


/*add_DC_table2mem: add Data code of memory to dymanic array*/
void add_DC_table2mem()
{
	int i;
	
	for (i=0;i<DC;i++)
		mem_word[IC++]=t_data[i].data;

}




