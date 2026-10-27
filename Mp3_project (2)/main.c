#include <stdio.h>
#include <string.h>
#include "view.h"
#include "edit.h"

void Mp3_view(char *filename);

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("INVALID ARGUMENTS\n");
        printf("USAGE :\n");
        printf("To view please pass like: ./a.out -v Mp3filename\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");
        printf("To get help pass like: ./a.out --help\n");

        return -1;
    }
    else
    {
        if(strcmp(argv[1], "--help") == 0)
        {
            printf("1. -v -> to view mp3 file contents\n");
            printf("2. -e -> to edit mp3 file contents\n");
            printf("    2.1. -t -> to edit song title\n");
            printf("    2.2. -a -> to edit artist name\n");
            printf("    2.3. -A -> to edit album name\n");
            printf("    2.4. -y -> to edit year\n");
            printf("    2.5. -m -> to edit content\n");
            printf("    2.6. -c -> to edit comment\n");
        }

        else if(strcmp(argv[1], "-v") == 0)
        {
            if(argc != 3)
            {
                printf("ERROR: Please pass mp3 filename\n");
                return -1;
            }

            if(strstr(argv[2], ".mp3") == NULL)
            {
                printf("ERROR: Please pass .mp3 file\n");
                return -1;
            }

            Mp3_view(argv[2]);
        }

        else if(strcmp(argv[1], "-e") == 0)
        {
            if(argc != 5)
            {
                printf("INVALID ARGUMENTS\n");
                printf("USAGE :\n");
                printf("To edit please pass like:\n");
                printf("./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");

                return -1;
            }
            if(strcmp(argv[2], "-t") != 0 &&
            strcmp(argv[2], "-a") != 0 &&
            strcmp(argv[2], "-A") != 0 &&
            strcmp(argv[2], "-y") != 0 &&
            strcmp(argv[2], "-m") != 0 &&
            strcmp(argv[2], "-c") != 0)
            {
                printf("INVALID EDIT OPTION\n");
                return -1;
            }
            if(strstr(argv[4], ".mp3") == NULL)
            {
                printf("ERROR: Please pass .mp3 file\n");
                return -1;
            }
            Mp3_edit(argv[4], argv[2], argv[3]);
        }

        else
        {
            printf("INVALID ARGUMENTS\n");
            printf("USAGE :\n");
            printf("To view please pass like: ./a.out -v Mp3filename\n");
            printf("To edit please pass like: ./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");
            printf("To get help pass like: ./a.out --help\n");
        }
    }

    return 0;
}