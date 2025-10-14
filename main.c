#include "headers.h"

int main()
{
    char command[4][80], str[80];
    char arr[1024] = {'\0'};
    char *str1 = NULL;
    int count = 0, ret = 0, fd = 0;

    InitialiseSuperB();
    CreateDILB();
    RestoreData();
    signal(SIGINT, sighandle);

    while (1)
    {
        printf("\nTerminal MayankUditAshish $ ");
        scanf(" %[^'\n']s", str);

        count = sscanf(str, "%s %s %s %s", command[0], command[1], command[2], command[3]);
        
        if (count == 1)
        {
            if (strcmp(command[0], "ls") == 0)
            {
                File_ls();
            }
            else if (strcmp(command[0], "clear") == 0)
            {
                system("clear");
                continue;
            }
            else if (strcmp(command[0], "exit") == 0)
            {
                BackupData();
                printf("\n**Terminate the Terminal Aniket**\n");
                break;
            }
            else if (strcmp(command[0], "deleteall") == 0)
            {
                Deleteall();
            }
            else if (strcmp(command[0], "help") == 0)
            {
                DisplayHelp();
                continue;
            }
            else
            {
                printf("ERROR :command not found\n");
                continue;
            }
        }
        else if (count == 2)
        {
            if (strcmp(command[0], "stat") == 0)
            {
                ret = stat_file(command[1]);
                if (ret == -1)
                {
                    printf("Invalid parameter");
                }
                else if (ret == -2)
                {
                    printf("File not found");
                }
            }
            else if (strcmp(command[0], "cat") == 0)
            {
                cat_file(command[1]);
            }
            else if (strcmp(command[0], "man") == 0)
            {
                DisplayManpage(command[1]);
            }
            else if (strcmp(command[0], "read") == 0)
            {
                fd = findfd(command[1]);
                if (fd == -2)
                {
                    printf("File not exist or not exist\n");
                    continue;
                }
                readWholeFile(fd);
            }
            else if (strcmp(command[0], "close") == 0)
            {
                close_file(command[1]);
                continue;
            }
            else if (strcmp(command[0], "fstat") == 0)
            {
                ret = atoi(command[1]);
                if (UArr[ret].ptrfiletable == NULL)
                {
                    printf("File not exist or not open\n");
                    continue;
                }
                fstat(atoi(command[1]));
            }
            else if (strcmp(command[0], "rm") == 0)
            {
                ret = remove_file(command[1]);
                if (ret == 0)
                {
                    printf("***Remove successfully***\n");
                }
                else if (ret == -1)
                {
                    printf("File not found");
                }
            }
            else if (strcmp(command[0], "write") == 0)
            {
                fd = findfd(command[1]);
                if (fd == -2)
                {
                    printf("File not exist or not exist\n");
                }
                printf("\nEnter the data :");
                scanf(" %[^\n]s", arr);
                if (strlen(arr) == 0)
                {
                    printf("\nInvalid");
                }
                ret = strlen(arr);
                ret = write_file(fd, arr, ret);
                if (ret > 0)
                {
                    printf("\n***Written Sucessfully***\n");
                }
                else if (ret == -1)
                {
                    printf("File size is full");
                }
                else if (ret == -2)
                {
                    printf("No permission for write");
                }
            }
            else
            {
                printf("ERROR:command not found\n");
                continue;
            }
        }
        else if (count == 3)
        {
            if (strcmp(command[0], "create") == 0)
            {
                ret = CreateFile(command[1], atoi(command[2]));
                if (ret >= 0)
                {
                    printf("***File is successfully created with file descriptor- %d***", ret);
                    continue;
                }
                else if (ret == -1)
                {
                    printf("File Already exist");
                    continue;
                }
                else if (ret == -2)
                {
                    printf("Incorrect Parameter");
                }
                else if (ret == -3)
                {
                    printf("Inode not available ");
                }
                continue;
            }
            else if (strcmp(command[0], "open") == 0)
            {
                ret = open_file(command[1], atoi(command[2]));
                printf("\nOpened file descriptor is %d\n", ret);
            }
            else if (strcmp(command[0], "truncate") == 0)
            {
                ret = truncate_file(command[1], atoi(command[2]));
                if (ret == -1)
                {
                    printf("\nInvalid parameter");
                    continue;
                }
                else
                {
                    printf("\n***Data deleted from file***");
                    continue;
                }
            }
            else if (strcmp(command[0], "read") == 0)
            {
                fd = findfd(command[1]);
                if (fd == -2)
                {
                    printf("File not exist or not exist\n");
                    continue;
                }
                ret = atoi(command[2]);
                str1 = (char *)malloc((sizeof(char) * ret) + 1);
                if (str1 == NULL)
                {
                    printf("\nMemory allocation fail\n");
                    continue;
                }
                for (int i = 0; i < (sizeof(char) * ret) + 1; i++)
                {
                    str1[i] = '\0';
                }
                ret = read_file(fd, str1, ret);
                printf("%s", str1);
                if (ret == 0)
                {
                    printf("\n***Read successfully***\n");
                }
                else if (ret == -1)
                {
                    printf("File have not permission to read");
                }
                free(str1); // Don't forget to free allocated memory
            }
            else
            {
                printf("ERROR:command not found\n");
                continue;
            }
        }
        else if (count == 4)
        {
            if (strcmp(command[0], "lseek") == 0)
            {
                ret = lseek_file(command[1], atoi(command[2]), atoi(command[3]));
                if (ret == 1)
                {
                    printf("\nlseek sucessfully done");
                }
            }
            continue;
        }
    }
    return 0;
}
