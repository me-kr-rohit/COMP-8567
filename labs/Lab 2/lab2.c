#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    checkTextFile();

    int txtFile = open("new.txt", O_RDWR);

    if (txtFile == -1)
    {
        printf("\n!!Error while opening the txtFile!!");
    }
    else
    {
        char *txt;
        txt = "Welcome to COMP 8567";
        write(txtFile, txt, strlen(txt));
        close(txtFile);

        // Append to the txtFile
        txtFile = open("new.txt", O_APPEND | O_WRONLY);
        txt = "Advanced Systems Programming";
        write(txtFile, txt, strlen(txt));
        close(txtFile);
 
        //Opening txtFile in Read Write mode
        txtFile = open("new.txt", O_RDWR);


        lseek(txtFile, 12, SEEK_SET);
        write(txtFile, "*****", 5);


        lseek(txtFile, 8, SEEK_CUR);
        write(txtFile, "*****", 5);


        lseek(txtFile, 0, SEEK_END);
        lseek(txtFile, 31, SEEK_CUR);
        write(txtFile, "*****", 5);



        char temp;
        lseek(txtFile, 0, SEEK_SET);
        while (read(txtFile, &temp, 1) > 0)
        {
            if (temp == '\0')
            {
                lseek(txtFile, -1, SEEK_CUR);
                write(txtFile, "#", 1);
            }
        }
        printf("\nReplaced all the null characters with #\n");
        close(txtFile);

        
        txtFile = open("new.txt", O_RDONLY);

        char str_length[14];
        int total_bytes = 0;
        int txtFile_size = 0;

        
        while ((total_bytes = read(txtFile, str_length, 13)) > 0)
        {
            str_length[total_bytes] = '\0';
            printf("%s\n", str_length);
            txtFile_size = txtFile_size + total_bytes;
        }

        printf("Size of txtFile is:%d bytes\n", txtFile_size);
        
    }
    close(txtFile);
    return 0;
}


void checkTextFile() 
{

    umask(0);
    if (access("new.txt", F_OK) == 0) 
    {
        printf("File already exists.\n");
        exit(1);
    }

    int txtFile = open("new.txt", O_CREAT , 0777);
    if (txtFile == -1) 
    {
        printf("Error creating the file\n");
        exit(1);
    }
    close(txtFile);
}