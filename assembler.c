#include "database.h"
#include "prototype.h"
#include "global.h"


int main(int argc,char *argv[])
{
	
	FILE *fin;
	char  name[MAX_NAME_FILE];
	
	if(argc==1)
	{
		printf("\nNot found any file to translate \n"); 
		exit(0);
	}
	
	
	while(--argc)
	{
	
		/*make a name file string with suffix .as */
		strcpy(name,argv[argc]); 
		strcat(name,SUFFIX_AS);
		printf("\nChecking file %s\n",name);
		/*open file to readd assemply program to translate*/
		if(!(fin=fopen(name,"r")))
		{
			fprintf(stderr,"canot been open read file %s\n",name);
			continue;
		}

		t_data=(pData)malloc(sizeof(tData));
		if(!t_data)
		{
			printf("\n cannot have build a list");
			exit(0);
		}
		
		err_count=0;
		first_over(fin);  /*first over checking*/
		fseek( fin, 0, SEEK_SET ); /*goto begging of file*/
		ext_table=(pLabel)malloc(sizeof(tLabel)); /*build dynamic array with 1 row for external address*/
		ent_table=(pLabel)malloc(sizeof(tLabel));/*build dynamic array with 1 row for extry address*/
		inx_ext=0; /*init to 0 index of external table*/
		inx_ent=0;/*init index of extry table*/
		second_over(fin); /*second over checking*/
		
		/*check if is no error */
		if (err_count>0)
			printf("\n\nTotal Errors %d.\n",err_count);
		else
		{/*no error, write the translation to files*/
			write_table2file(ent_table,inx_ent,argv[argc],SUFFIX_ENT); /*write entry table to file suffix .ent*/
			write_table2file(ext_table,inx_ext,argv[argc],SUFFIX_EXT); /*write external table to file*/
			write_mem2file(argv[argc],SUFFIX_OB); /*write program to file .ob*/
		};
	
		fclose(fin);/*close file*/
		/*free memory*/
		free(t_data);
		free_list(&head);
		free(ext_table);
		free(ent_table);
		
	} /*end while */
	
	

	return 0;
}
