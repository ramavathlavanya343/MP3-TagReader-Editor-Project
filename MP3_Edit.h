#ifndef mp3_edit
#define mp3_edit

 
typedef struct edit_mp3_specifications
{
    char file_name[10];
    char file_buffer[100];
    char size_content;
    FILE * fptr_source;
    FILE * fptr_temp;
    char content[100];
    char *tag[4];
    int edit_arg_index;
}Edit_mp3;


/*function prototype*/

Status mp3_Edit(Edit_mp3 *);


Status open_file(Edit_mp3 *);


Status close_file(Edit_mp3 *);


Status update_info(Edit_mp3 *);


Status copy_header(Edit_mp3 *);


Status edit_key(char *key,Edit_mp3 *mp3);


void convert_to_little_endian_edit(char *p,char *q);


#endif



    
