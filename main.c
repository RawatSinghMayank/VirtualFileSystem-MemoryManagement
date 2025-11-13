#include "headers.h"

int main()
{
    char command[4][80], str[80];
    char arr[1024] = {'\0'};
    char *str1 = NULL;
    int count = 0, ret = 0, fd = 0;

    // Initialize file system
    InitialiseSuperB();
    CreateDILB();
    RestoreData();
    signal(SIGINT, sighandle);

    printf("\n====================================================\n");
    printf("       VIRTUAL FILE SYSTEM WITH MEMORY MANAGEMENT\n");
    printf("====================================================\n");
    printf("Type 'help' for available commands\n");
    printf("Type 'exit' to quit\n");
    printf("====================================================\n");

    while (1)
    {
        printf("\nVFS $ ");
        fflush(stdout);
        
        if (fgets(str, sizeof(str), stdin) == NULL) {
            break;
        }
        
        // Remove newline character
        str[strcspn(str, "\n")] = 0;
        
        if (strlen(str) == 0) {
            continue;
        }

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
            }
            else if (strcmp(command[0], "exit") == 0)
            {
                printf("Exiting Virtual File System...\n");
                BackupData();
                printf("Goodbye!\n");
                break;
            }
            else if (strcmp(command[0], "deleteall") == 0)
            {
                printf("Are you sure you want to delete ALL files? (y/n): ");
                fflush(stdout);
                
                char confirm[10];
                if (fgets(confirm, sizeof(confirm), stdin) != NULL) {
                    if (confirm[0] == 'y' || confirm[0] == 'Y') {
                        Deleteall();
                    } else {
                        printf("Operation cancelled.\n");
                    }
                }
            }
            else if (strcmp(command[0], "help") == 0)
            {
                DisplayHelp();
            }
            else if (strcmp(command[0], "meminit") == 0)
            {
                InitializeMemoryManager();
            }
            else if (strcmp(command[0], "memmap") == 0)
            {
                DisplayMemoryMapCLI();
            }
            else if (strcmp(command[0], "proclist") == 0)
            {
                DisplayProcessListCLI();
            }
            else if (strcmp(command[0], "memstats") == 0)
            {
                MemoryStatsCLI();
            }
            else if (strcmp(command[0], "compact") == 0)
            {
                CompactMemoryCLI();
            }
            else
            {
                printf("Error: Command '%s' not found. Type 'help' for available commands.\n", command[0]);
            }
        }
        else if (count == 2)
        {
            if (strcmp(command[0], "stat") == 0)
            {
                ret = stat_file(command[1]);
                if (ret == VFS_ERROR_FILE_NOT_FOUND)
                {
                    printf("Error: File '%s' not found\n", command[1]);
                }
                else if (ret == VFS_ERROR_INVALID_PARAM)
                {
                    printf("Error: Invalid filename\n");
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
                if (fd < 0)
                {
                    printf("Error: File '%s' not found or not open\n", command[1]);
                    continue;
                }
                ret = readWholeFile(fd);
                if (ret < 0)
                {
                    printf("Error: Cannot read file (permission denied)\n");
                }
            }
            else if (strcmp(command[0], "close") == 0)
            {
                close_file(command[1]);
            }
            else if (strcmp(command[0], "fstat") == 0)
            {
                fd = atoi(command[1]);
                if (!validate_fd(fd))
                {
                    printf("Error: Invalid file descriptor or file not open\n");
                    continue;
                }
                fstat(fd);
            }
            else if (strcmp(command[0], "rm") == 0)
            {
                printf("Are you sure you want to delete '%s'? (y/n): ", command[1]);
                fflush(stdout);
                
                char confirm[10];
                if (fgets(confirm, sizeof(confirm), stdin) != NULL) {
                    if (confirm[0] == 'y' || confirm[0] == 'Y') {
                        ret = remove_file(command[1]);
                        if (ret == VFS_SUCCESS)
                        {
                            printf("File '%s' deleted successfully\n", command[1]);
                        }
                        else if (ret == VFS_ERROR_FILE_NOT_FOUND)
                        {
                            printf("Error: File '%s' not found\n", command[1]);
                        }
                        else if (ret == VFS_ERROR_FILE_BUSY)
                        {
                            printf("Error: File '%s' is currently in use\n", command[1]);
                        }
                    } else {
                        printf("Operation cancelled.\n");
                    }
                }
            }
            else if (strcmp(command[0], "write") == 0)
            {
                fd = findfd(command[1]);
                if (fd < 0)
                {
                    printf("Error: File '%s' not found or not open\n", command[1]);
                    continue;
                }
                
                printf("Enter data to write (press Enter when done):\n");
                fflush(stdout);
                
                if (fgets(arr, sizeof(arr), stdin) == NULL) {
                    printf("Error reading input\n");
                    continue;
                }
                
                // Remove newline
                arr[strcspn(arr, "\n")] = 0;
                
                if (strlen(arr) == 0)
                {
                    printf("Error: No data entered\n");
                    continue;
                }
                
                ret = write_file(fd, arr, strlen(arr));
                if (ret > 0)
                {
                    printf("Success: %d bytes written to file\n", ret);
                }
                else if (ret == VFS_ERROR_NO_SPACE)
                {
                    printf("Error: File system is full\n");
                }
                else if (ret == VFS_ERROR_PERMISSION_DENIED)
                {
                    printf("Error: Write permission denied\n");
                }
                else
                {
                    printf("Error: Write operation failed\n");
                }
            }
            else if (strcmp(command[0], "killproc") == 0)
            {
                int pid = atoi(command[1]);
                if (pid > 0) {
                    ret = TerminateProcessCLI(pid);
                    if (ret < 0) {
                        printf("Error: Failed to terminate process %d\n", pid);
                    }
                } else {
                    printf("Error: Invalid process ID\n");
                }
            }
            else if (strcmp(command[0], "setalgo") == 0)
            {
                int algo = atoi(command[1]);
                SetAllocAlgorithmCLI(algo);
            }
            else
            {
                printf("Error: Command '%s' not found or invalid usage\n", command[0]);
                printf("Type 'man %s' for help on this command\n", command[0]);
            }
        }
        else if (count == 3)
        {
            if (strcmp(command[0], "create") == 0)
            {
                ret = CreateFile(command[1], atoi(command[2]));
                if (ret >= 0)
                {
                    printf("Success: File '%s' created with FD: %d\n", command[1], ret);
                }
                else if (ret == VFS_ERROR_FILE_EXISTS)
                {
                    printf("Error: File '%s' already exists\n", command[1]);
                }
                else if (ret == VFS_ERROR_NO_SPACE)
                {
                    printf("Error: No space available (max files: %d)\n", MAXINODE);
                }
                else if (ret == VFS_ERROR_INVALID_PARAM)
                {
                    printf("Error: Invalid filename or permission\n");
                    printf("Permissions: 4=READ, 2=WRITE, 6=READ/WRITE\n");
                }
            }
            else if (strcmp(command[0], "open") == 0)
            {
                ret = open_file(command[1], atoi(command[2]));
                if (ret >= 0)
                {
                    printf("Success: File '%s' opened with FD: %d\n", command[1], ret);
                }
                else if (ret == VFS_ERROR_FILE_NOT_FOUND)
                {
                    printf("Error: File '%s' not found\n", command[1]);
                }
                else if (ret == VFS_ERROR_NO_SPACE)
                {
                    printf("Error: No file descriptors available\n");
                }
                else if (ret == VFS_ERROR_INVALID_PARAM)
                {
                    printf("Error: Invalid filename or mode\n");
                    printf("Modes: 4=READ, 2=WRITE, 6=READ/WRITE\n");
                }
            }
            else if (strcmp(command[0], "truncate") == 0)
            {
                ret = truncate_file(command[1], atoi(command[2]));
                if (ret == VFS_SUCCESS)
                {
                    printf("Success: File '%s' truncated to %s bytes\n", 
                           command[1], command[2]);
                }
                else if (ret == VFS_ERROR_FILE_NOT_FOUND)
                {
                    printf("Error: File '%s' not found\n", command[1]);
                }
                else if (ret == VFS_ERROR_INVALID_PARAM)
                {
                    printf("Error: Invalid size (0-%d)\n", MAXFILESIZE);
                }
            }
            else if (strcmp(command[0], "read") == 0)
            {
                fd = findfd(command[1]);
                if (fd < 0)
                {
                    printf("Error: File '%s' not found or not open\n", command[1]);
                    continue;
                }
                
                int bytes_to_read = atoi(command[2]);
                if (bytes_to_read <= 0)
                {
                    printf("Error: Invalid number of bytes\n");
                    continue;
                }
                
                str1 = (char *)malloc(bytes_to_read + 1);
                if (str1 == NULL)
                {
                    printf("Error: Memory allocation failed\n");
                    continue;
                }
                
                memset(str1, 0, bytes_to_read + 1);
                ret = read_file(fd, str1, bytes_to_read);
                
                if (ret > 0)
                {
                    printf("Data read (%d bytes):\n%s\n", ret, str1);
                }
                else if (ret == 0)
                {
                    printf("End of file reached\n");
                }
                else if (ret == VFS_ERROR_PERMISSION_DENIED)
                {
                    printf("Error: Read permission denied\n");
                }
                else
                {
                    printf("Error: Read operation failed\n");
                }
                
                free(str1);
            }
            else if (strcmp(command[0], "createproc") == 0)
            {
                int memory = atoi(command[2]);
                if (memory > 0) {
                    int pid = CreateProcessCLI(command[1], memory);
                    if (pid > 0) {
                        printf("Process created with PID: %d\n", pid);
                    } else {
                        printf("Failed to create process\n");
                    }
                } else {
                    printf("Error: Invalid memory size\n");
                }
            }
            else
            {
                printf("Error: Command '%s' not found or invalid usage\n", command[0]);
                printf("Type 'man %s' for help on this command\n", command[0]);
            }
        }
        else if (count == 4)
        {
            if (strcmp(command[0], "lseek") == 0)
            {
                ret = lseek_file(command[1], atoi(command[2]), atoi(command[3]));
                if (ret == VFS_SUCCESS)
                {
                    printf("Success: File offset repositioned\n");
                }
                else if (ret == VFS_ERROR_FILE_NOT_FOUND)
                {
                    printf("Error: File '%s' not found\n", command[1]);
                }
                else if (ret == VFS_ERROR_INVALID_PARAM)
                {
                    printf("Error: Invalid offset or origin\n");
                    printf("Origins: 0=START, 1=CURRENT, 2=END\n");
                }
            }
            else
            {
                printf("Error: Command '%s' not found or invalid usage\n", command[0]);
                printf("Type 'man %s' for help on this command\n", command[0]);
            }
        }
        else
        {
            printf("Error: Too many arguments\n");
        }
    }

    // Cleanup
    Deleteall(); // This will free all allocated memory
    
    // Cleanup memory manager
    if (mem_manager != NULL) {
        PMEMBLOCK current = mem_manager->memory_list;
        while (current != NULL) {
            PMEMBLOCK next = current->next;
            free(current);
            current = next;
        }
        
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
    
    return 0;
}
