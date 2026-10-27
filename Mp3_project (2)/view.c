#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "view.h"

void Mp3_view(char *filename)
{
    FILE *fptr = fopen(filename, "rb");

    if(fptr == NULL)
    {
        printf("ERROR: Unable to open file\n");
        return;
    }

    char buffer[4];
    fread(buffer, 3, 1, fptr);
    buffer[3] = '\0';

    if(strcmp(buffer, "ID3") != 0)
    {
        printf("ERROR: Not a valid MP3 file\n");
        fclose(fptr);
        return;
    }

    fseek(fptr, 7, SEEK_CUR);

    printf("--------------------VIEW SELECTED--------------------\n");

    for(int i = 0; i < 6; i++)
    {
        char tag[5];
        fread(tag, 4, 1, fptr);
        tag[4] = '\0';
        unsigned char size_buffer[4];
        fread(size_buffer, 4, 1, fptr);

        int size = (size_buffer[0] << 24) | (size_buffer[1] << 16) | (size_buffer[2] << 8) | size_buffer[3];
        fseek(fptr, 2, SEEK_CUR);

        if(strcmp(tag, "COMM") == 0)
        {
            fseek(fptr, 1, SEEK_CUR);
            char language[4];
            fread(language, 3, 1, fptr);
            language[3] = '\0';
            printf("%d.%s ---->%d %s\n",i + 1, tag, size, language);

            fseek(fptr, size - 4, SEEK_CUR);
        }
        else
        {
            unsigned char encoding;
            fread(&encoding, 1, 1, fptr);
            char *content = malloc(size);
            fread(content, size - 1, 1, fptr);

            content[size - 1] = '\0';
            printf("%d.%s ---->%d %s\n",i + 1, tag, size, content);

            free(content);
        }
    }

    fclose(fptr);
}