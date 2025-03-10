#include "common.h"
#include "types.h"

int update_flag=0;  /* global variable to check the update status*/

/*
 1.function to edit the mp3 file
 2.open the file
 3.copy the header from the src temp file
 4.update the information in the mp3 file
 */

Status mp3_Edit(Edit_mp3 *mp3)
{
    if(open_file(mp3)!=e_success)
    {
	printf("\nINFO:Failed to open file\n");
	return e_failure;
    }
    printf("\nINFO:File opened successfully. Information: Ready for access\n");
    if(copy_header(mp3)!=e_success)
    {
	printf("\nINFO:Failed to copy header\n");
	return e_failure;
    }
    printf("\nINFO:Header copied successfully\n");
    if(update_info(mp3)!=e_success)
    {
	printf("\nINFO:Failed to update details\n");
	return e_failure;
    }
    printf("\t%s:%s",data_arr[mp3->edit_arg_index],mp3->content);
    if(close_file(mp3)!=e_success)
    {
	printf("\nINFO:Failed to close file\n");
	return e_failure;
    }
    printf("\nINFO:Updated Successfully\n");
    return e_success;
}
/*
   1.function to open the src file
   
 */
Status open_file(Edit_mp3 *mp3)
{
    mp3->fptr_source=fopen(mp3->file_name,"r");
    if(mp3->fptr_source==NULL)
    {
	printf("Failed to open filr %s\n",mp3->file_name);
	return e_failure;
    }
    mp3->fptr_temp=fopen("temp.mp3","w");
    return e_success;
}

/*
   1.function to copy the header file
   2.copy the 10 bytes of data
   3.read the 1 byte data from src file to store it in buffer file
  
 */
Status copy_header(Edit_mp3 *mp3)
{
    if((fread(mp3->file_buffer,10,1,mp3->fptr_source)>0))
    {
	fwrite(mp3->file_buffer,10,1,mp3->fptr_temp);
	return e_success;
    }
    return e_failure;
}

/*
function to edit the key
 */
Status edit_key(char *key,Edit_mp3 *mp3)
{
    for(int i=0;i<6;i++)
    {
	if(!strcmp(key,edit_arg[i]))
	{
	    mp3->edit_arg_index=i;
	    return e_success;
 	}
    }
    return e_failure;
}

/*
 1.function to update the information in mp3 file
 2.write the tag to temporary file
 3.calculate the content size and update it
 4.read backup size from src file and update it
 5.write the update content size to temp file
 6.copy the flag and content to temp file
 
 */

Status update_info(Edit_mp3 *mp3)
{
    int b[4]={0},backup_size,content_size;
    char tag[15];
    while(fread(tag,4,1,mp3->fptr_source)>0)
    {
	tag[4] = '\0';
	if(!strcmp(tag,tags[mp3->edit_arg_index]))
	{
	    fwrite(tag,4,1,mp3->fptr_temp);
	    content_size=strlen(mp3->content)+1;
	    int Backup_extent=content_size;
	    char *content_size_ptr=(char *)&content_size;
	    convert_to_little_endian_edit(content_size_ptr,content_size_ptr+3);
	    if(!(fread(&backup_size,4,1,mp3->fptr_source)>0))
	    {
        	printf("Error in reading the size %s\n",tag);
		return e_failure;
	    }
	    char *size_of_ptr;
	    size_of_ptr =(char *)&backup_size;
	    convert_to_little_endian_edit(size_of_ptr,size_of_ptr+3);
	    
	    fwrite(content_size_ptr,4,1,mp3->fptr_temp);
	    if(!(fread(mp3->file_buffer,3,1,mp3->fptr_source)>0))
	    {
		printf("Error:Failed to read the flag for the %s tag\n",tag);
		return e_failure;
	    }
	    fwrite(mp3->file_buffer,3,1,mp3->fptr_temp);
	    if(!(fwrite(mp3->content,Backup_extent-1,1,mp3->fptr_temp)>0))
	    {
		printf("Error:Content not successfully copied\n");
		return e_failure;
	    }
	    fseek(mp3->fptr_source,backup_size-1,SEEK_CUR);
	    update_flag=1;
	}
	else
	{
	    if(update_flag==1)
	    {
		fwrite(tag,4,1,mp3->fptr_temp);
	    }
	    else
	    {
		fwrite(tag,4,1,mp3->fptr_temp);
		if(!(fread(&backup_size,4,1,mp3->fptr_source)>0))
		{
		    printf("Error in reading the size %s\n",tag);
		    return e_failure;
		}
		int size = backup_size;
		char *size_of_ptr=(char *)&backup_size;
		convert_to_little_endian_edit(size_of_ptr,size_of_ptr+3);

		fwrite(&size,4,1,mp3->fptr_temp);

		if(!(fread(mp3->file_buffer,2,1,mp3->fptr_source)>0))
		{
		    printf("Error:Failed to read the flag for the %s tag\n",tag);
		    return e_failure;
		}
		fwrite(mp3->file_buffer,2,1,mp3->fptr_temp);
		if(!(fread(mp3->file_buffer,backup_size,1,mp3->fptr_source)>0))
		{
		    printf("Failed to read the content of %s\n",tag);
		    return e_failure;
		}
		fwrite(mp3->file_buffer,backup_size,1,mp3->fptr_temp);
	    }
	}
    }
}

/*
 1.function to close the src file and temp file
 2.again reopen the files read and write mode
 3.copy the contents from temp file to src file
 4.close the files

 */
Status close_file(Edit_mp3 *mp3)
{
    fclose(mp3->fptr_source);
    fclose(mp3->fptr_temp);
    mp3->fptr_source=fopen(mp3->file_name,"w+");
    mp3->fptr_temp=fopen("temp.mp3","r+");
    char ch[8];
    while(fread(ch,1,1,mp3->fptr_temp)>0)
    {
	fwrite(ch,1,1,mp3->fptr_source);
    }
    fclose(mp3->fptr_source);
    fclose(mp3->fptr_temp);
    remove("temp.mp3");
}
/*
   Convertion of littile endian to big endian
 */
void convert_to_little_endian_edit(char *p,char *q)
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








