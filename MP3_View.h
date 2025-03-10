#ifndef mp3_view
#define mp3_view 

typedef struct mp3_specifications
{
    char file_name[10];
    char file_buffer[100];
    int size_content;
    FILE *fptr_source;
    FILE *fptr_buffer;
 
    char content[100];
}mp3_details;

/*
   function prototypes
 */
Status view_mp3(mp3_details *);

Status view_open_file(mp3_details *);

Status display_mp3(mp3_details *);

Status tag_presence(char *key,mp3_details *mp3);

void convert_to_little_endian(char *p,char *q);

void tag_info(char *,char *);

#endif








