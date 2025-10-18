#include "headers.h"

void DisplayHelp()
{
    printf("\n====================================================\n");
    printf("            VIRTUAL FILE SYSTEM HELP MENU\n");
    printf("====================================================\n");
    printf("create  : Create a new file with specified permissions\n");
    printf("open    : Open an existing file\n");
    printf("close   : Close an opened file\n");
    printf("read    : Read data from a file\n");
    printf("write   : Write data to a file\n");
    printf("ls      : List all files in the system\n");
    printf("stat    : Display file information\n");
    printf("fstat   : Display information of opened file\n");
    printf("truncate: Shrink or extend file size\n");
    printf("lseek   : Reposition read/write file offset\n");
    printf("cat     : Display file content without opening\n");
    printf("rm      : Remove a file\n");
    printf("deleteall: Delete all files\n");
    printf("clear   : Clear the terminal screen\n");
    printf("help    : Display this help menu\n");
    printf("man     : Display manual for a command\n");
    printf("exit    : Exit the application\n");
    printf("====================================================\n");
    printf("Permissions: 4=READ, 2=WRITE, 6=READ/WRITE\n");
    printf("====================================================\n");
}

void DisplayManpage(char *str)
{
    if (str == NULL)
    {
        printf("Error: No command specified\n");
        return;
    }
    
    if (strcasecmp(str, "open") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\topen - open a file\n"
               "\nSYNOPSIS\n"
               "\topen FILE_NAME MODE\n"
               "\nDESCRIPTION\n"
               "\tOpen an existing file with specified mode.\n"
               "\nMODES\n"
               "\t4 or READ    - Open in read-only mode\n"
               "\t2 or WRITE   - Open in write-only mode\n"
               "\t6 or READWRITE - Open in read-write mode\n"
               "\nEXAMPLES\n"
               "\topen myfile 4\n"
               "\topen data.txt 6\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "close") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tclose - close a file\n"
               "\nSYNOPSIS\n"
               "\tclose FILE_NAME\n"
               "\nDESCRIPTION\n"
               "\tClose an opened file and free associated resources.\n"
               "\nEXAMPLES\n"
               "\tclose myfile\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "read") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tread - read from a file\n"
               "\nSYNOPSIS\n"
               "\tread FILE_NAME [BYTES]\n"
               "\nDESCRIPTION\n"
               "\tRead data from a file. If BYTES is not specified,\n"
               "\treads the entire file content.\n"
               "\nEXAMPLES\n"
               "\tread myfile        # Read entire file\n"
               "\tread data.txt 100  # Read 100 bytes\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "write") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\twrite - write to a file\n"
               "\nSYNOPSIS\n"
               "\twrite FILE_NAME\n"
               "\nDESCRIPTION\n"
               "\tWrite data to a file. You will be prompted for input.\n"
               "\nEXAMPLES\n"
               "\twrite myfile\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "ls") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tls - list files\n"
               "\nSYNOPSIS\n"
               "\tls\n"
               "\nDESCRIPTION\n"
               "\tList all files in the virtual file system with their sizes.\n"
               "\nEXAMPLES\n"
               "\tls\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "stat") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tstat - display file statistics\n"
               "\nSYNOPSIS\n"
               "\tstat FILE_NAME\n"
               "\nDESCRIPTION\n"
               "\tDisplay detailed information about a file including\n"
               "\tsize, permissions, timestamps, and metadata.\n"
               "\nEXAMPLES\n"
               "\tstat myfile\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "fstat") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tfstat - display file descriptor statistics\n"
               "\nSYNOPSIS\n"
               "\tfstat FILE_DESCRIPTOR\n"
               "\nDESCRIPTION\n"
               "\tDisplay information about an opened file using its\n"
               "\tfile descriptor.\n"
               "\nEXAMPLES\n"
               "\tfstat 3\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "truncate") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\ttruncate - change file size\n"
               "\nSYNOPSIS\n"
               "\ttruncate FILE_NAME SIZE\n"
               "\nDESCRIPTION\n"
               "\tShrink or extend a file to the specified size.\n"
               "\tIf extending, new space is filled with spaces.\n"
               "\nEXAMPLES\n"
               "\ttruncate myfile 500   # Set file size to 500 bytes\n"
               "\ttruncate data.txt 0   # Empty the file\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "create") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tcreate - create a new file\n"
               "\nSYNOPSIS\n"
               "\tcreate FILE_NAME PERMISSION\n"
               "\nDESCRIPTION\n"
               "\tCreate a new file with specified permissions.\n"
               "\nPERMISSIONS\n"
               "\t4 or READ    - Read-only access\n"
               "\t2 or WRITE   - Write-only access\n"
               "\t6 or READWRITE - Read-write access\n"
               "\nEXAMPLES\n"
               "\tcreate myfile 6\n"
               "\tcreate config.txt 4\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "lseek") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tlseek - reposition file offset\n"
               "\nSYNOPSIS\n"
               "\tlseek FILE_NAME OFFSET ORIGIN\n"
               "\nDESCRIPTION\n"
               "\tReposition the read/write file offset.\n"
               "\nORIGINS\n"
               "\t0 or START   - Offset from beginning\n"
               "\t1 or CURRENT - Offset from current position\n"
               "\t2 or END     - Offset from end\n"
               "\nEXAMPLES\n"
               "\tlseek myfile 100 0   # Seek to byte 100 from start\n"
               "\tlseek data.txt -50 1 # Seek back 50 bytes from current\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "rm") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\trm - remove a file\n"
               "\nSYNOPSIS\n"
               "\trm FILE_NAME\n"
               "\nDESCRIPTION\n"
               "\tPermanently delete a file from the system.\n"
               "\nEXAMPLES\n"
               "\trm myfile\n"
               "\trm old_data.txt\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "exit") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\texit - terminate the application\n"
               "\nSYNOPSIS\n"
               "\texit\n"
               "\nDESCRIPTION\n"
               "\tExit the virtual file system application.\n"
               "\tAll data is automatically backed up.\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "cat") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tcat - display file content\n"
               "\nSYNOPSIS\n"
               "\tcat FILE_NAME\n"
               "\nDESCRIPTION\n"
               "\tDisplay the content of a file without opening it.\n"
               "\nEXAMPLES\n"
               "\tcat myfile\n"
               "\tcat config.txt\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "man") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tman - display command manual\n"
               "\nSYNOPSIS\n"
               "\tman COMMAND\n"
               "\nDESCRIPTION\n"
               "\tDisplay detailed manual page for a command.\n"
               "\nEXAMPLES\n"
               "\tman create\n"
               "\tman open\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "deleteall") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tdeleteall - delete all files\n"
               "\nSYNOPSIS\n"
               "\tdeleteall\n"
               "\nDESCRIPTION\n"
               "\tDelete all files in the virtual file system.\n"
               "\tThis operation cannot be undone.\n"
               "\nWARNING\n"
               "\tThis will permanently remove all files!\n");
        printf("====================================================\n");
    }
    else
    {
        printf("Error: No manual entry for '%s'\n", str);
        printf("Use 'help' to see available commands.\n");
    }
}

void fstat(int fd)
{
    if (!validate_fd(fd))
    {
        printf("Error: Invalid file descriptor\n");
        return;
    }
    
    PINODE inode = UArr[fd].ptrfiletable->ptrinode;
    
    printf("\nFile Descriptor Statistics (FD: %d)\n", fd);
    printf("====================================\n");
    printf("File name: %s\n", inode->Fname);
    printf("Inode number: %d\n", inode->Ino);
    printf("File size: %d/%d bytes\n", inode->ActualFileSize, inode->FileSize);
    printf("File type: %s\n", (inode->FileType == REGULAR) ? "Regular" : "Unknown");
    printf("Actual File size: %d bytes\n", inode->ActualFileSize);
    
    if (inode->Permission == READ)
    {
        printf("Permission: READ ONLY\n");
    }
    else if (inode->Permission == WRITE)
    {
        printf("Permission: WRITE ONLY\n");
    }
    else if (inode->Permission == READ_WRITE)
    {
        printf("Permission: READ and WRITE\n");
    }
    else
    {
        printf("Permission: UNKNOWN\n");
    }
    
    printf("Link count: %d\n", inode->LinkCount);
    printf("Reference count: %d\n", inode->ReferenceCount);
    printf("Birth: %s", inode->Birth);
    printf("Last Access Time: %s", inode->LastAccessTime);
    printf("Last Modified Time: %s", inode->LastModifiedTime);
    
    // File table information
    printf("\nFile Table Information:\n");
    printf("Read Offset: %d\n", UArr[fd].ptrfiletable->ReadOffset);
    printf("Write Offset: %d\n", UArr[fd].ptrfiletable->WriteOffset);
    printf("Open mode: %d\n", UArr[fd].ptrfiletable->mode);
    printf("Reference count: %d\n", UArr[fd].ptrfiletable->count);
}

void BackupData()
{
    int fd = 0;
    PINODE temp = head;

    printf("Backing up data...\n");
    
    fd = creat("vfs_backup.dat", 0777);
    if (fd == -1)
    {
        printf("Error: Could not create backup file\n");
        return;
    }
    close(fd);
    
    fd = open("vfs_backup.dat", O_WRONLY | O_TRUNC);
    if (fd == -1)
    {
        printf("Error: Could not open backup file for writing\n");
        return;
    }

    // Write superblock
    write(fd, &superblockobj, sizeof(SB));

    // Write inodes
    temp = head;
    while (temp != NULL)
    {
        write(fd, temp, sizeof(INODE));
        if (temp->FileType != 0 && temp->Buffer != NULL)
        {
            write(fd, temp->Buffer, MAXFILESIZE);
        }
        temp = temp->next;
    }
    
    close(fd);
    printf("Data backed up successfully to 'vfs_backup.dat'\n");
}

void RestoreData()
{
    int fd = 0, ret = 0;
    INODE iobj;
    PINODE temp = head;

    fd = open("vfs_backup.dat", O_RDONLY);
    if (fd == -1)
    {
        printf("No backup file found. Starting with fresh file system.\n");
        return;
    }

    // Read superblock
    ret = read(fd, &superblockobj, sizeof(SB));
    if (ret != sizeof(SB))
    {
        printf("Warning: Backup file may be corrupted. Starting fresh.\n");
        close(fd);
        return;
    }

    // Read inodes
    temp = head;
    while (temp != NULL && read(fd, &iobj, sizeof(INODE)) == sizeof(INODE))
    {
        // Copy basic inode data
        strcpy(temp->Fname, iobj.Fname);
        temp->Ino = iobj.Ino;
        temp->FileSize = iobj.FileSize;
        temp->ActualFileSize = iobj.ActualFileSize;
        temp->FileType = iobj.FileType;
        temp->ReferenceCount = iobj.ReferenceCount;
        temp->Permission = iobj.Permission;
        temp->LinkCount = iobj.LinkCount;
        strcpy(temp->Birth, iobj.Birth);
        strcpy(temp->LastAccessTime, iobj.LastAccessTime);
        strcpy(temp->LastModifiedTime, iobj.LastModifiedTime);

        // Allocate and read buffer if file exists
        if (temp->FileType != 0)
        {
            temp->Buffer = (char *)malloc(MAXFILESIZE);
            if (temp->Buffer != NULL)
            {
                read(fd, temp->Buffer, MAXFILESIZE);
            }
        }
        else
        {
            temp->Buffer = NULL;
        }
        
        temp = temp->next;
    }
    
    close(fd);
    printf("Data restored successfully from backup.\n");
}

void sighandle(int s)
{
    printf("\n\nReceived interrupt signal. Backing up data...\n");
    BackupData();
    printf("Virtual File System terminated safely.\n");
    exit(0);
}