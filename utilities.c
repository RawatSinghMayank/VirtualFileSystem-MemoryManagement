#include "headers.h"

void DisplayHelp()
{
    printf("\n----------------------------------------------------\n");
    printf("open : It is used to open the file\n");
    printf("close : It is used to close the file\n");
    printf("closeall : It is used to close all opened files\n");
    printf("read : It is used to read the file\n");
    printf("write : It is used to write the file\n");
    printf("delete : It is used to delete the file\n");
    printf("stat : It is used to display information the file\n");
    printf("fstat : It is used to display information of opened the file\n");
    printf("ls : To list out all files present in that directory\n");
    printf("lseek : Read the data where starting point of reading data is depends on user.\n");
    printf("clear : To clear console\n");
    printf("truncate : To shrink or extend the file\n");
    printf("cat : To display the data of file without openeing it\n");
    printf("exit : To exit the application\n");
    printf("-----------------------------------------------------------\n");
    printf("to know more about command type : 'man command_name'\n");
    printf("-----------------------------------------------------------\n");
}

void DisplayManpage(char *str)
{
    if (str == NULL)
    {
        return;
    }
    if (strcasecmp(str, "open") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\topen - open the file"
               "\n\n"
               "SYNOPSIS\n"
               "\t\topen file_name mode"
               "\n\n"
               "DESCRIPTION\n"
               "\t\topen the existing file which is specified by user.\n"
               "\t\tmode - \t"
               "1.\tr or 4 - open file in read mode.\n"
               "\t\t\t2.\tw or 2 - open file in write mode\n"
               "\t\t\t3.\trw or 6 - open file in read & write both mode"
               "\n\n"
               "RETURN VALUE\n"
               "\t\ton success returned file descriptor, on failure display error\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "close") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tclose- close the file"
               "\n\n"
               "SYNOPSIS\n"
               "\t\tclose file_name"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tcloses the all file_descriptor pointing to file which is specified by user.so it may reused."
               "\n\n"
               "RETURN VALUE\n"
               "\t\ton failure display error\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "read") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tread - read from a file name"
               "\n\n"
               "SYNOPSIS\n"
               "\t\t1.\tread file_name\n"
               "\t\t2.\tread file_name no_of_bytes"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tread attempts to read up to count bytes from file descriptor fd into the buffer starting at buf."
               "\n\t\tif u did not specify the no of bytes you want to read then it will read the whole file from current offset."
               "\n\n"
               "RETURN VALUE\n"
               "\t\tOn success, the number of bytes read is returned, on failure error name gets displayed appropriately\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "write") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\twrite - write from a file_name"
               "\n\n"
               "SYNOPSIS\n"
               "\t\twrite file_name"
               "\n\n"
               "DESCRIPTION\n"
               "\t\twrite attempts to write up to count bytes from file name in the file."
               "\n\n"
               "RETURN VALUE\n"
               "\t\tOn success, the number of bytes written is returned, and the file position is advanced by this number and on failure errorname gets display.\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "ls") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tls - displays list"
               "\n\n"
               "SYNOPSIS\n"
               "\t\t1.\tls\n"
               "\t\t2.\tls -i\n"
               "\t\t3.\tls fd"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tls displays the list of files present in directory\n"
               "\t\tls -i displays the list of file with their inode number\n"
               "\t\tls fd display the list of files opened with their file descriptor\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "stat") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tstat - information from a file_name"
               "\n\n"
               "SYNOPSIS\n"
               "\t\tstat file_name"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tstat command display the all information related to that file."
               "\n\n"
               "RETURN VALUE\n"
               "\t\tOn success, display information, on failure display error\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "fstat") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tfstat - information from a file descriptor"
               "\n\n"
               "SYNOPSIS\n"
               "\t\tfstat file_descriptor"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tfstat command display the all information related to that file descriptor."
               "\n\n"
               "RETURN VALUE\n"
               "\t\tOn success, display information, on failure display error\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "truncate") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\ttruncate - shrink or extend the size of a file to the specified size"
               "\n\n"
               "SYNOPSIS\n"
               "\t\ttruncate file_name size"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tIf  a  Actual File Size is larger than the specified size, the extra data is lost.\n"
               "\t\tIf a Actual File Size is shorter, it is extended and the extended part (hole) reads as zero bytes."
               "\n\n"
               "RETURN VALUE\n"
               "\t\tOn success display success, on failure display error\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "create") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tcreate - creates the file"
               "\n\n"
               "SYNOPSIS\n"
               "\t\tcreate file_name permission"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tcreate the new file.\n"
               "\t\tpermssion - \t"
               "1.\tr or 4 - gives file only read permission to the file.\n"
               "\t\t\t\t2.\tw or 2 - gives only write permission to the file.\n"
               "\t\t\t\t3.\trw or 6 - gives read and write both permission to the file."
               "\n\n"
               "RETURN VALUE\n"
               "\t\ton success returned file descriptor, on failure display error\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "closeall") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tcloseall - closes all file_descriptor"
               "\n\n"
               "SYNOPSIS\n"
               "\t\tcloseall"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tcloseall closes the all the file_descriptor pointing to any file, so it may be reused."
               "\n\n"
               "RETURN VALUE\n"
               "\t\tOn success display success, on failure display error\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "lseek") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tlseek - reposition read/write file offset"
               "\n\n"
               "SYNOPSIS\n"
               "\t\tlseek file_name offset flag"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tlseek repositions the file offset of the open file description associated with the file name\n"
               "\t\tto the argument offset according to the directive flag as follows:\n"
               "\t\tflag \t"
               "1.\tstart - The file offset is set to offset bytes.\n"
               "\t\t\t2.\tcurrent - The file offset is set to its current location plus offset bytes.\n"
               "\t\t\t3.\tend - The file offset is set to the size of the file plus offset bytes.\n"
               "\t\t\tlseek() allows the file offset to be set beyond the end of the file and creates hole and file size changes."
               "\n\n"
               "RETURN VALUE\n"
               "\t\ton failure display error\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "delete") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tdelete - delete the file"
               "\n\n"
               "SYNOPSIS\n"
               "\t\tdelete file_name"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tdelete command the delete the file specified by user."
               "\n\n"
               "RETURN VALUE\n"
               "\t\ton failure display error\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "exit") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\texit - terminate the application"
               "\n\n"
               "SYNOPSIS\n"
               "\t\texit"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tclose the application\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "cat") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tcat - display the file data"
               "\n\n"
               "SYNOPSIS\n"
               "\t\tcat File_name"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tdisplay the data of file without openeing the file\n");
        printf("\n----------------------------------------------------\n");
    }
    else if (strcasecmp(str, "man") == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("NAME\n"
               "\t\tman - an interface to the application reference manuals"
               "\n\n"
               "SYNOPSIS\n"
               "\t\tman command_name"
               "\n\n"
               "DESCRIPTION\n"
               "\t\tman is the application's manual pager.  Each page argument given to man is the name of a command.\n"
               "\t\tThe manual page associated with each of these arguments is then found and displayed. \n");
        printf("\n----------------------------------------------------\n");
    }
    else
    {
        printf("error : man page not found for entry %s\n", str);
    }
}

void fstat(int fd)
{
    printf("File name- %s\n", UArr[fd].ptrfiletable->ptrinode->Fname);
    printf("Inode number- %d\n", UArr[fd].ptrfiletable->ptrinode->Ino);
    printf("File size- %d\n", UArr[fd].ptrfiletable->ptrinode->FileSize);
    printf("File type- %d\n", UArr[fd].ptrfiletable->ptrinode->FileType);
    printf("Actual File size- %d\n", UArr[fd].ptrfiletable->ptrinode->ActualFileSize);
    printf("Permission- %d\n", UArr[fd].ptrfiletable->ptrinode->Permission);
    printf("Link count- %d\n", UArr[fd].ptrfiletable->ptrinode->LinkCount);
    printf("Reference count- %d\n", UArr[fd].ptrfiletable->ptrinode->ReferenceCount);
    printf("Birth- %s", UArr[fd].ptrfiletable->ptrinode->Birth);
    printf("Last Access Time- %s", UArr[fd].ptrfiletable->ptrinode->LastAccessTime);
    printf("Last Modified Time- %s", UArr[fd].ptrfiletable->ptrinode->LastModifiedTime);
}

void BackupData()
{
    int fd = 0;
    PINODE temp = head;

    fd = creat("backup.txt", 0777);
    if (fd != -1)
    {
        close(fd);
    }
    fd = open("backup.txt", O_WRONLY);

    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            write(fd, temp, sizeof(INODE));
            write(fd, temp->Buffer, MAXFILESIZE);
        }
        temp = temp->next;
    }
    close(fd);
}

void RestoreData()
{
    int fd = 0, ret = 0;
    INODE iobj;
    PINODE temp = head;

    fd = open("backup.txt", O_RDONLY);
    if (fd == -1)
    {
        return;
    }

    int size = lseek(fd, 0, SEEK_END);
    if (size == 0)
    {
        return;
    }

    lseek(fd, 0, SEEK_SET);

    while ((ret = read(fd, &iobj, sizeof(INODE))) > 0)
    {
        temp->Ino = iobj.Ino;
        strcpy(temp->Fname, iobj.Fname);
        temp->FileSize = iobj.FileSize;
        temp->FileType = iobj.FileType;
        temp->ActualFileSize = iobj.ActualFileSize;
        temp->LinkCount = iobj.LinkCount;
        temp->ReferenceCount = iobj.ReferenceCount;
        temp->Permission = iobj.Permission;
        strcpy(temp->Birth, iobj.Birth);
        strcpy(temp->LastAccessTime, iobj.LastAccessTime);
        strcpy(temp->LastModifiedTime, iobj.LastModifiedTime);
        temp->Buffer = (char *)malloc(MAXFILESIZE);
        read(fd, temp->Buffer, MAXFILESIZE);
        temp = temp->next;
    }
    close(fd);
    printf("Data Restored\n");
}

void sighandle(int s)
{
    BackupData();
    printf("\n**Terminate the Terminal Aniket**\n");
    exit(0);
}
