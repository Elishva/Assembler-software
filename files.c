#include "database.h"
#include "prototype.h"


/*write_table2file" write table details to file
plbl-is poiter to struct label table contdtat & adtress (dymanic array)
last_index-is the last index in dymanic array
name- is string name file
suffix is the suffix of name file*/
void write_table2file(pLabel plbl,int last_index,char *name,char *suffix)
{
	FILE *fout;
	int i;
	char str[BASE8_CODE];
	char fname[MAX_NAME_FILE];

	/*make name fike with suffix*/
	strcpy(fname,name);
	strcat(fname,suffix);
	printf("make file %s\n",fname);
	/*open file to writing*/
	if(!(fout=fopen(fname,"w")))
	{
		fprintf(stderr,"canot been open write file %s\n",fname);
		return;
	}
	
	/*write to file*/
	for (i=0;i<last_index;i++)
	{
		get_special_code_address(plbl[i].address,str); /*convert to special base 8 code*/
		fprintf(fout,"%s  %s\n",plbl[i].name, str); /*write to file*/
	}


	fclose (fout); /*close file*/
	
}


/*write_mem2file: write assmbler code to file .ob
name: is name file to write
suffix: is the suffix of file -ob*/
void write_mem2file(char *name,char *suffix)
{
	FILE *fout;
	int i;
	char fname[MAX_NAME_FILE];
	char str[BASE8_CODE],str1[BASE8_CODE];
	
	/*make name fike with suffix*/
	strcpy(fname,name);
	strcat(fname,suffix);
	printf("make file %s\n",fname);

	/*open file to writing*/
	if(!(fout=fopen(fname,"w")))
	{
		fprintf(stderr,"canot been open write file %s\n",fname);
		return;
	}

	/*write IC & DC to file*/
	get_special_code_address(IC,str);
	get_special_code_address(DC,str1);
	fprintf (fout,"\t\t%s \t %s\n",str,str1);
	/*write all translation code to file address & code*/
	for (i=0;i<IC;i++)
	{	
		/*printf ("%d\t %x\n",i+START_ADDRESS,mem_word[i]);*/
		get_special_code_address(i+START_ADDRESS,str); /*translate address to 8 base special code*/
		get_special_code(mem_word[i],str1);/*translate code to 8 base special code*/
		fprintf (fout,"%s  %s\n",str,str1); /*write to file adrress & code*/
	}
	
	/*write Data at the end of program*/
	for (i=IC;i<IC+DC;i++)
	{
		get_special_code_address(i+START_ADDRESS,str); /*translate address to 8 base special code*/
		get_special_code(mem_word[i],str1);/*translate data to 8 base special code*/
		fprintf (fout,"%s  %s\n",str,str1);
	}
	
	fclose (fout);
}

/*get_special_code_address: translate number to 8 base special code -translation can be 1-5  digit
num-is the number havew to convert
str-is the result string contain the translate 8 base special code*/
void get_special_code_address(int num,char *str)
{
	unsigned int *p=(unsigned int *) (&BitFormat);
 	int flag=FALSE;
	int i=0;
	
	*p=num;
	/*if 3 msb are zero ingnore them else translate to 8 base special code*/
	if (BitFormat.ch5!=0 || flag)
	{
		str[i++]=SpecialCode[BitFormat.ch5];
		flag=TRUE;
	}
	/*if 3 msb are zero & also next 3 bits ingnore them, else translate to 8 base special code*/
	if (BitFormat.ch4!=0 || flag)
	{
		str[i++]=SpecialCode[BitFormat.ch4];
		flag=TRUE;
	}
	
	/*if 6 msb are zero & also next 3 bits ingnore them, else translate to 8 base special code*/
	if (BitFormat.ch3!=0 || flag)
	{
		str[i++]=SpecialCode[BitFormat.ch3];
		flag=TRUE;
	}

	/*if 9 msb are zero & also next 3 bits ingnore them, else translate to 8 base special code*/
	if (BitFormat.ch2!=0 || flag)
	{
		str[i++]=SpecialCode[BitFormat.ch2];
		flag=TRUE;
	}
	
	str[i++]=SpecialCode[BitFormat.ch1];
   	str[i]=CH_END_STRING; /*char '\0'*/


}


/*get_special_code:translate number to 8 base special code (5 digit always)
num-is the number havew to convert
str-is the result string contain the translate 8 base special code*/
void get_special_code(int num,char *str)
{
	unsigned int *p=(unsigned int *) (&BitFormat);
 	
	
	*p=num;
		
	str[0]=SpecialCode[BitFormat.ch5];
	str[1]=SpecialCode[BitFormat.ch4];
	str[2]=SpecialCode[BitFormat.ch3];	
	str[3]=SpecialCode[BitFormat.ch2];
	str[4]=SpecialCode[BitFormat.ch1];
   	str[5]=CH_END_STRING;


}


