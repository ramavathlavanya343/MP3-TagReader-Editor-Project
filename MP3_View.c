#include "common.h"


/* global variables */
int flag=0;
int count=0;

/*
 1.function to view mp3 file
 2.open the file
 */
Status view_mp3(mp3_details *mp3)
{
    if(view_open_file(mp3)!=e_success)
    {
	printf("\nINFO: Failed to open file.\n");
	return e_failure;
    }
    printf("\nINFO:File opened sucessfully\n");

    if( display_mp3(mp3) != e_success)
    {
	printf("\nINFO:Failed to display mp3 information\n");
	return e_failure;
    }
    return e_success;
}

/*
 1.function to open the mp3 file 
 */

Status view_open_file(mp3_details *mp3 )
{
    if((mp3->fptr_source=fopen(mp3->file_name,"r"))!=NULL)
    {
	return e_success;
    }
    else
    {
	printf("\nINFO:Failed to open file\n");
	return e_failure;
    }
}

/*
   1.function to display the mp3 details
   2.read the tag and size
   3.convert size to little endian
 */

Status display_mp3(mp3_details *mp3)
{
    int size;
    char tag[4];
    rewind(mp3->fptr_source);
    fseek(mp3->fptr_source,10,SEEK_CUR);

    while((fread(tag,4,1,mp3->fptr_source))>0)
    {

	tag[4]='\0';


	if(!((fread(&size,4,1,mp3->fptr_source))>0))

	{
	    printf("Error: Failed to read size %s tags",tag);
	    return e_failure;
	}

	char *ptr;
	ptr=(char*)&size;
	convert_to_little_endian(ptr,ptr+3);


	char *content=malloc(size*sizeof(char));


	if(tag_presence(tag,mp3)!=e_failure)
	{
	    fseek(mp3->fptr_source,3,SEEK_CUR);

	    if(!(fread(content,size-1,1,mp3->fptr_source))>0)
	    {
		printf("Error: Failed to read the content of the '%s' tag",tag);
		return e_failure;
	    }

	    content[size-1]='\0';

	    
	    tag_info(tag,content);
	}
	else
	{
	    fseek(mp3->fptr_source,size+2,SEEK_CUR);
	}

    }

    return e_success;
}

/*
 function to display the tag information
 */
void tag_info(char *tag,char *content)
{
    for(int i=0;i<6;i++)
    {
	if(!(strcmp(tag,tags[i])))
	
	{
	    printf("\t\t%s:%s\n",data_arr[i],content);
	}
    }
}

/*
 function to convert the little endian to big endian
 */
void convert_to_little_endian(char *p,char *q)
{
    while(p<q)
    {
	char temp=*p;
	*p=*q;
	*q=temp;
	p++;
	q--;
    }
}
/*
 function to check presence of a tag
 */
Status tag_presence(char *key,mp3_details *mp3)
{
    for(int i=0;i<6;i++)
    {
	
	if ( !(strcmp(key,tags[i])))
	{
	    return e_success;
	}
	
    }
    return e_failure;
}







