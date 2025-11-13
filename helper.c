#include "headers.h"

void DisplayHelp()
{
    printf("\n====================================================\n");
    printf("            VIRTUAL FILE SYSTEM HELP MENU\n");
    printf("====================================================\n");
    printf("File System Commands:\n");
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
    
    printf("\nMemory Management Commands:\n");
    printf("meminit   : Initialize memory manager\n");
    printf("createproc: Create a new process <name> <memory>\n");
    printf("killproc  : Terminate process <pid>\n");
    printf("memmap    : Display memory map\n");
    printf("proclist  : Display process list\n");
    printf("setalgo   : Set allocation algorithm <0=FirstFit,1=BestFit,2=WorstFit>\n");
    printf("memstats  : Display memory statistics\n");
    printf("compact   : Compact memory\n");
    
    printf("\nUtility Commands:\n");
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
    else if (strcasecmp(str, "meminit") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tmeminit - initialize memory manager\n"
               "\nSYNOPSIS\n"
               "\tmeminit\n"
               "\nDESCRIPTION\n"
               "\tInitialize the memory management system with 1MB total memory.\n"
               "\nEXAMPLES\n"
               "\tmeminit\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "createproc") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tcreateproc - create a new process\n"
               "\nSYNOPSIS\n"
               "\tcreateproc PROCESS_NAME MEMORY_SIZE\n"
               "\nDESCRIPTION\n"
               "\tCreate a new process with specified memory requirements.\n"
               "\nEXAMPLES\n"
               "\tcreateproc browser 65536\n"
               "\tcreateproc editor 32768\n");
        printf("====================================================\n");
    }
    else if (strcasecmp(str, "memmap") == 0)
    {
        printf("\n====================================================\n");
        printf("NAME\n"
               "\tmemmap - display memory map\n"
               "\nSYNOPSIS\n"
               "\tmemmap\n"
               "\nDESCRIPTION\n"
               "\tDisplay current memory allocation map showing used and free blocks.\n"
               "\nEXAMPLES\n"
               "\tmemmap\n");
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

void sighandle(int s __attribute__((unused)))
{
    printf("\n\nReceived interrupt signal. Backing up data...\n");
    BackupData();
    
    // Cleanup memory manager
    if (mem_manager != NULL) {
        // Free all memory blocks
        PMEMBLOCK current = mem_manager->memory_list;
        while (current != NULL) {
            PMEMBLOCK next = current->next;
            free(current);
            current = next;
        }
        
        // Free all processes
        PPROCESS proc_current = mem_manager->process_list;
        while (proc_current != NULL) {
            PPROCESS proc_next = proc_current->next;
            if (proc_current->page_table != NULL) {
                free(proc_current->page_table);
            }
            free(proc_current);
            proc_current = proc_next;
        }
        
        free(mem_manager);
        mem_manager = NULL;
    }
    
    printf("Virtual File System terminated safely.\n");
    exit(0);
}
