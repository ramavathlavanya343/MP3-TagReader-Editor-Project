#include "common.h"
#include "MP3_View.c"
#include "MP3_Edit.c"
#include "MP3_help.c"

/*
Name:Lavanya
Date:14-06-2024
Description:MP3 Tag Reader & Editor
 */

/*
Check the commant arguments 
compare the string with arguments -v
copy the what ever the arguments in the src file

 */

int main( int argc,char *argv[])
{
    if(argc>2)
    {
	int comp=strcmp(argv[1],"-v");
	if(comp==0)
	{
	    mp3_details mp3;
	    if((strstr(argv[2],".mp3"))!=NULL)
	    {
		strcpy(mp3.file_name,argv[2]);
		view_mp3(&mp3);
	    }
	    else
	    {
		printf("ERROR: Please provide the path to an MP3 file as a command-line argument.\n");
	    }
	}
	else if(!strcmp(argv[1],"-e"))
	{
	    Edit_mp3 mp3;
	    if((edit_key(argv[2],&mp3))!=e_failure)
	    {
		strcpy(mp3.content,argv[3]);
		if((strstr(argv[4],".mp3"))!=NULL)
		{
		    strcpy(mp3.file_name,argv[4]);
		    mp3_Edit(&mp3);
		}
		else
		{
		    printf("ERROR: Please provide the path to an MP3 file as a command-line argument.\n");
		}
	    }
	    else
	    {
		printf("ERROR: Please enter a valid keyword.\n");
	    }
	}
    }
    else if(argc>1)
    {
	if(!(strcmp(argv[1],"--help")) || (!strcmp(argv[1],"-h")))
	{
	  
	    disply_menu();

  
	}
    }
    else
    {
	printf("\n╔═══════════════════════════════════════════════════════════════════════════════════════════════════╗\n\n");
	printf("\tError: Invalid arguments provided for ./a.out\n");
	printf("\tUSAGE :\n\tTo view please pass like : ./a.out -v mp3filename\n\tTo edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n\tTo get help pass like\t : ./a.out --help or ./a.out -h\n");
	printf("\n╚═══════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");

    }



    return 0;
}


