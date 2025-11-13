#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

// Macro definitions for file system constants
#define MAXINODE 50
#define MAXFILESIZE 1024

#define READ 4
#define WRITE 2
#define READ_WRITE 6

#define REGULAR 1

#define START 0
#define CURRENT 1
#define END 2

// Memory management constants
#define TOTAL_MEMORY 1024 * 1024  // 1MB total memory
#define PAGE_SIZE 4096           // 4KB page size
#define MAX_PROCESSES 10
#define MAX_PAGES (TOTAL_MEMORY / PAGE_SIZE)

// Error codes
typedef enum {
    VFS_SUCCESS = 0,
    VFS_ERROR_INVALID_PARAM = -1,
    VFS_ERROR_FILE_NOT_FOUND = -2,
    VFS_ERROR_NO_SPACE = -3,
    VFS_ERROR_PERMISSION_DENIED = -4,
    VFS_ERROR_FILE_EXISTS = -5,
    VFS_ERROR_INVALID_FD = -6,
    VFS_ERROR_FILE_BUSY = -7
} VFS_RESULT;

// Memory allocation algorithms
typedef enum {
    FIRST_FIT = 0,
    BEST_FIT = 1,
    WORST_FIT = 2
} ALLOC_ALGO;

// Data structure for the Inode
typedef struct inode
{
    char Fname[50];
    int Ino;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    struct inode *next;
    int Permission;
    char *Buffer; 
    int LinkCount;
    char Birth[60];
    char LastAccessTime[60];
    char LastModifiedTime[60];
} INODE, *PINODE, **PPINODE;

// Data structure for the File Table
typedef struct FileT
{
    int ReadOffset;
    int WriteOffset;
    int mode;
    int count;
    PINODE ptrinode;
} FT, *PFT;

// Data structure for the User File Descriptor Table (UFDT)
typedef struct ufdt
{
    PFT ptrfiletable;
} UFDT, *PUFDT;

// Data structure for the Superblock
typedef struct SuperB
{
    int TInode;
    int FInode;
} SB, *PSB;

// Process Control Block
typedef struct Process {
    int pid;
    char name[50];
    int memory_required;
    int pages_allocated;
    int *page_table;
    int state; // 0: terminated, 1: running, 2: waiting
    struct Process *next;
} PROCESS, *PPROCESS;

// Memory Block
typedef struct MemoryBlock {
    int start_address;
    int size;
    int is_free;
    int pid; // owner process ID
    struct MemoryBlock *next;
} MEMBLOCK, *PMEMBLOCK;

// Memory Manager
typedef struct MemoryManager {
    int total_memory;
    int free_memory;
    int used_memory;
    ALLOC_ALGO current_algorithm;
    PMEMBLOCK memory_list;
    PPROCESS process_list;
    int page_faults;
    int next_pid;
} MEM_MANAGER, *PMEM_MANAGER;

// Global variables declaration
extern PINODE head;
extern UFDT UArr[50];
extern SB superblockobj;
extern PMEM_MANAGER mem_manager;

// Core file system functions
void CreateDILB();
void InitialiseSuperB();
void DisplayHelp();
void DisplayManpage(char *str);
void fstat(int fd);
PINODE Get_Inode(char *name);
int CreateFile(char *name, int permission);
int remove_file(char *name);
int findfd(char *name);
void File_ls();
int stat_file(char *name);
int write_file(int fd, char *brr, int size);
int read_file(int fd, char *brr, int size);
int readWholeFile(int fd);
int Deleteall();
int truncate_file(char *name, int size);
int open_file(char *name, int mode);
int lseek_file(char *name, int size, int from);
void close_file(char *name);
void close_file_by_fd(int fd);
void cat_file(char *name);
void BackupData();
void RestoreData();
void sighandle(int s);

// GUI specific functions
char* File_ls_gui();
char* stat_file_gui(char *name);
char* cat_file_gui(char *name);

// Utility functions
int validate_fd(int fd);
int validate_filename(const char *name);
int validate_permission(int permission);

// Memory Management Functions
void InitializeMemoryManager();
int CreateProcess(char *name, int memory_required);
int TerminateProcess(int pid);
int AllocateMemory(int pid, int size, ALLOC_ALGO algo);
int DeallocateMemory(int pid);
void DisplayMemoryMap();
void DisplayProcessList();
void SetAllocationAlgorithm(ALLOC_ALGO algo);
void MemoryStats();
void CompactMemory();

// CLI wrapper functions
int CreateProcessCLI(char *name, int memory);
int TerminateProcessCLI(int pid);
void DisplayMemoryMapCLI();
void DisplayProcessListCLI();
void SetAllocAlgorithmCLI(int algo);
void MemoryStatsCLI();
void CompactMemoryCLI();

// GUI memory functions
char* DisplayMemoryMapGUI();
char* DisplayProcessListGUI();
char* MemoryStatsGUI();

#endif
