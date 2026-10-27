#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edit.h"

void Mp3_edit(char *filename, char *option, char *new_value)
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

    char frame_id[5];

    if(strcmp(option, "-t") == 0)
    {
        strcpy(frame_id, "TIT2");
    }
    else if(strcmp(option, "-a") == 0)
    {
        strcpy(frame_id, "TPE1");
    }
    else if(strcmp(option, "-A") == 0)
    {
        strcpy(frame_id, "TALB");
    }
    else if(strcmp(option, "-y") == 0)
    {
        strcpy(frame_id, "TYER");
    }
    else if(strcmp(option, "-m") == 0)
    {
        strcpy(frame_id, "TCON");
    }
    else if(strcmp(option, "-c") == 0)
    {
        strcpy(frame_id, "COMM");
    }

    FILE *fptr_dup = fopen("duplicate.mp3", "wb");

    if(fptr_dup == NULL)
    {
        printf("ERROR: Unable to create duplicate file\n");
        fclose(fptr);
        return;
    }

    fseek(fptr, 0, SEEK_SET);
    char header[10];
    fread(header, 10, 1, fptr);
    fwrite(header, 10, 1, fptr_dup);

    for(int i = 0; i < 6; i++)
    {
        char tag[5];
        fread(tag, 4, 1, fptr);
        tag[4] = '\0';
        unsigned char size_buffer[4];

        fread(size_buffer, 4, 1, fptr);

        int backup_size = (size_buffer[0] << 24) | (size_buffer[1] << 16) | (size_buffer[2] << 8) | size_buffer[3];
       
        unsigned char flag[2];
        fread(flag, 2, 1, fptr);

        if(strcmp(tag, frame_id) == 0 &&
           strcmp(option, "-c") == 0)
        {
            unsigned char *old_content = malloc(backup_size);

            if(old_content == NULL)
            {
                printf("ERROR: Memory allocation failed\n");
                fclose(fptr);
                fclose(fptr_dup);
                return;
            }

            fread(old_content, backup_size, 1, fptr);
            int comment_start = 4;

            while(comment_start < backup_size &&
                  old_content[comment_start] != '\0')
            {
                comment_start++;
            }

            if(comment_start < backup_size)
            {
                comment_start++;
            }

            int prefix_size = comment_start;
            int newsize = prefix_size + strlen(new_value);
            unsigned char new_size_buffer[4];

            new_size_buffer[0] = (newsize >> 24) & 0xFF;
            new_size_buffer[1] = (newsize >> 16) & 0xFF;
            new_size_buffer[2] = (newsize >> 8) & 0xFF;
            new_size_buffer[3] = newsize & 0xFF;

            fwrite(tag, 4, 1, fptr_dup);
            fwrite(new_size_buffer, 4, 1, fptr_dup);
            fwrite(flag, 2, 1, fptr_dup);
            fwrite(old_content, prefix_size, 1, fptr_dup);
            fwrite(new_value, strlen(new_value), 1, fptr_dup);

            free(old_content);
        }

        else if(strcmp(tag, frame_id) == 0)
        {
            fwrite(tag, 4, 1, fptr_dup);
            int newsize = strlen(new_value) + 1;
            unsigned char new_size_buffer[4];

            new_size_buffer[0] = (newsize >> 24) & 0xFF;
            new_size_buffer[1] = (newsize >> 16) & 0xFF;
            new_size_buffer[2] = (newsize >> 8) & 0xFF;
            new_size_buffer[3] = newsize & 0xFF;

            fwrite(new_size_buffer, 4, 1, fptr_dup);
            fwrite(flag, 2, 1, fptr_dup);
            unsigned char encoding = 0;

            fwrite(&encoding, 1, 1, fptr_dup);
            fwrite(new_value, strlen(new_value), 1, fptr_dup);
            fseek(fptr, backup_size, SEEK_CUR);
        }
        else
        {
            fwrite(tag, 4, 1, fptr_dup);
            fwrite(size_buffer, 4, 1, fptr_dup);
            fwrite(flag, 2, 1, fptr_dup);
            char *content = malloc(backup_size);

            if(content == NULL)
            {
                printf("ERROR: Memory allocation failed\n");
                fclose(fptr);
                fclose(fptr_dup);
                return;
            }

            fread(content, backup_size, 1, fptr);
            fwrite(content, backup_size, 1, fptr_dup);

            free(content);
        }
    }

    char ch;
    while(fread(&ch, 1, 1, fptr) == 1)
    {
        fwrite(&ch, 1, 1, fptr_dup);
    }

    fclose(fptr);
    fclose(fptr_dup);

    remove(filename);
    rename("duplicate.mp3", filename);

    printf("MP3 file edited successfully\n");
}