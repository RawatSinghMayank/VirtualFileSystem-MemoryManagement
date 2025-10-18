#include "headers.h"

PINODE head = NULL;
UFDT UArr[50];
SB superblockobj;

void CreateDILB()
{
    PINODE temp = NULL, newn = NULL;
    
    while (head != NULL) {
        temp = head;
        head = head->next;
        if (temp->Buffer != NULL) {
            free(temp->Buffer);
        }
        free(temp);
    }
    
    head = NULL;
    
    for (int i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));
        if (newn == NULL) {
            printf("Memory allocation failed for inode %d\n", i);
            continue;
        }
        
        memset(newn, 0, sizeof(INODE));
        
        if (head == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = newn;
        }
        
        newn->Ino = i;
        newn->FileSize = MAXFILESIZE;
        newn->Buffer = NULL;
        newn->next = NULL;
    }
}

void InitialiseSuperB()
{
    for (int i = 0; i < 50; i++)
    {
        UArr[i].ptrfiletable = NULL;
    }
    superblockobj.TInode = MAXINODE;
    superblockobj.FInode = MAXINODE;
}

PINODE Get_Inode(char *name)
{
    PINODE temp = head;
    
    if (!validate_filename(name)) {
        return NULL;
    }
    
    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            if (strcmp(temp->Fname, name) == 0)
            {
                return temp;
            }
        }
        temp = temp->next;
    }
    return NULL;
}

int CreateFile(char *name, int permission)
{
    PINODE temp = head;
    time_t t;
    int i = 0;

    if (!validate_filename(name) || !validate_permission(permission))
    {
        return VFS_ERROR_INVALID_PARAM;
    }
    
    if (superblockobj.FInode == 0)
    {
        return VFS_ERROR_NO_SPACE;
    }
    
    if (Get_Inode(name) != NULL)
    {
        return VFS_ERROR_FILE_EXISTS;
    }

    
    while (temp != NULL && temp->FileType != 0)
    {
        temp = temp->next;
    }
    
    if (temp == NULL) {
        return VFS_ERROR_NO_SPACE;
    }

 
    for (i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable == NULL)
        {
            break;
        }
    }
    
    if (i == 50) {
        return VFS_ERROR_NO_SPACE;
    }

   
    PFT fileobj = (PFT)malloc(sizeof(FT));
    if (fileobj == NULL) {
        return VFS_ERROR_NO_SPACE;
    }
    
    temp->Buffer = (char *)malloc(MAXFILESIZE);
    if (temp->Buffer == NULL) {
        free(fileobj);
        return VFS_ERROR_NO_SPACE;
    }

    // Initialize file table
    UArr[i].ptrfiletable = fileobj;
    UArr[i].ptrfiletable->ReadOffset = 0;
    UArr[i].ptrfiletable->WriteOffset = 0;
    UArr[i].ptrfiletable->count = 1;
    UArr[i].ptrfiletable->mode = permission;
    UArr[i].ptrfiletable->ptrinode = temp;

    // Initialize inode
    temp->Permission = permission;
    temp->ActualFileSize = 0;
    temp->FileSize = MAXFILESIZE;
    temp->FileType = REGULAR;
    temp->ReferenceCount = 1;
    strncpy(temp->Fname, name, sizeof(temp->Fname) - 1);
    temp->Fname[sizeof(temp->Fname) - 1] = '\0';
    
    // Initialize buffer
    memset(temp->Buffer, 0, MAXFILESIZE);
    
    // Set timestamps
    time(&t);
    strncpy(temp->Birth, ctime(&t), sizeof(temp->Birth) - 1);
    strncpy(temp->LastAccessTime, ctime(&t), sizeof(temp->LastAccessTime) - 1);
    strncpy(temp->LastModifiedTime, "-", sizeof(temp->LastModifiedTime) - 1);

    (superblockobj.FInode)--;
    return i;
}

int remove_file(char *name)
{
    PINODE temp = head;
    
    if (!validate_filename(name)) {
        return VFS_ERROR_INVALID_PARAM;
    }

    // First close any open file descriptors for this file
    for (int i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable != NULL && 
            UArr[i].ptrfiletable->ptrinode != NULL &&
            strcmp(UArr[i].ptrfiletable->ptrinode->Fname, name) == 0)
        {
            close_file_by_fd(i);
        }
    }

    // Find and remove the inode
    while (temp != NULL)
    {
        if (temp->FileType != 0 && strcmp(temp->Fname, name) == 0)
        {
            if (temp->ReferenceCount > 0) {
                return VFS_ERROR_FILE_BUSY;
            }
            
            if (temp->Buffer != NULL) {
                free(temp->Buffer);
                temp->Buffer = NULL;
            }
            
            temp->FileType = 0;
            memset(temp->Fname, 0, sizeof(temp->Fname));
            temp->FileSize = MAXFILESIZE;
            temp->ActualFileSize = 0;
            temp->ReferenceCount = 0;
            temp->Permission = 0;
            temp->LinkCount = 0;
            memset(temp->Birth, 0, sizeof(temp->Birth));
            memset(temp->LastAccessTime, 0, sizeof(temp->LastAccessTime));
            memset(temp->LastModifiedTime, 0, sizeof(temp->LastModifiedTime));
            
            (superblockobj.FInode)++;
            return VFS_SUCCESS;
        }
        temp = temp->next;
    }
    
    return VFS_ERROR_FILE_NOT_FOUND;
}

int findfd(char *name)
{
    int i = 0;
    
    if (!validate_filename(name)) {
        return VFS_ERROR_INVALID_PARAM;
    }
    
    for (i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable != NULL && 
            UArr[i].ptrfiletable->ptrinode != NULL)
        {
            if (strcmp(UArr[i].ptrfiletable->ptrinode->Fname, name) == 0)
            {
                return i;
            }
        }
    }
    
    return VFS_ERROR_FILE_NOT_FOUND;
}

void File_ls()
{
    PINODE temp = head;
    int count = 0;
    
    printf("\nFiles in Virtual File System:\n");
    printf("-----------------------------\n");
    
    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            printf("%s (Size: %d bytes)\n", temp->Fname, temp->ActualFileSize);
            count++;
        }
        temp = temp->next;
    }
    
    if (count == 0) {
        printf("No files found.\n");
    }
    printf("Total files: %d\n", count);
}

int stat_file(char *name)
{
    PINODE temp = head;
    
    if (!validate_filename(name)) {
        return VFS_ERROR_INVALID_PARAM;
    }
    
    while (temp != NULL)
    {
        if (temp->FileType != 0 && strcmp(temp->Fname, name) == 0)
        {
            printf("\nFile Statistics for: %s\n", temp->Fname);
            printf("-----------------------------\n");
            printf("Inode number: %d\n", temp->Ino);
            printf("File size: %d/%d bytes\n", temp->ActualFileSize, temp->FileSize);
            printf("File type: %s\n", (temp->FileType == REGULAR) ? "Regular" : "Unknown");
            printf("Birth: %s", temp->Birth);
            printf("Last Access Time: %s", temp->LastAccessTime);
            printf("Last Modified Time: %s", temp->LastModifiedTime);
            
            if (temp->Permission == READ)
            {
                printf("Permission: READ ONLY\n");
            }
            else if (temp->Permission == WRITE)
            {
                printf("Permission: WRITE ONLY\n");
            }
            else if (temp->Permission == READ_WRITE)
            {
                printf("Permission: READ and WRITE\n");
            }
            else
            {
                printf("Permission: UNKNOWN\n");
            }
            
            printf("Link count: %d\n", temp->LinkCount);
            printf("Reference count: %d\n", temp->ReferenceCount);
            return VFS_SUCCESS;
        }
        temp = temp->next;
    }
    
    return VFS_ERROR_FILE_NOT_FOUND;
}

int write_file(int fd, char *brr, int size)
{
    time_t t;
    
    if (!validate_fd(fd) || brr == NULL || size < 0) {
        return VFS_ERROR_INVALID_PARAM;
    }
    
    if (UArr[fd].ptrfiletable->ptrinode->ActualFileSize >= MAXFILESIZE)
    {
        return VFS_ERROR_NO_SPACE;
    }
    
    if ((UArr[fd].ptrfiletable->mode == READ) || 
        (UArr[fd].ptrfiletable->ptrinode->Permission == READ))
    {
        return VFS_ERROR_PERMISSION_DENIED;
    }
    
    // Calculate available space
    int available = MAXFILESIZE - UArr[fd].ptrfiletable->WriteOffset;
    if (size > available) {
        size = available;
    }
    
    // Write data
    memcpy(UArr[fd].ptrfiletable->ptrinode->Buffer + 
           UArr[fd].ptrfiletable->WriteOffset, brr, size);
    
    UArr[fd].ptrfiletable->WriteOffset += size;
    
    // Update actual file size if we wrote beyond current end
    if (UArr[fd].ptrfiletable->WriteOffset > UArr[fd].ptrfiletable->ptrinode->ActualFileSize) {
        UArr[fd].ptrfiletable->ptrinode->ActualFileSize = UArr[fd].ptrfiletable->WriteOffset;
    }
    
    // Ensure null termination
    if (UArr[fd].ptrfiletable->ptrinode->ActualFileSize < MAXFILESIZE) {
        UArr[fd].ptrfiletable->ptrinode->Buffer[UArr[fd].ptrfiletable->ptrinode->ActualFileSize] = '\0';
    }
    
    // Update timestamp
    time(&t);
    strncpy(UArr[fd].ptrfiletable->ptrinode->LastModifiedTime, ctime(&t), 
            sizeof(UArr[fd].ptrfiletable->ptrinode->LastModifiedTime) - 1);
    
    return size;
}

int read_file(int fd, char *brr, int size)
{
    int bytes_to_read = 0;
    time_t t;
    
    if (!validate_fd(fd) || brr == NULL || size < 0) {
        return VFS_ERROR_INVALID_PARAM;
    }
    
    if (UArr[fd].ptrfiletable->mode == WRITE)
    {
        return VFS_ERROR_PERMISSION_DENIED;
    }
    
    if (UArr[fd].ptrfiletable->ReadOffset >= UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
    {
        return 0; // EOF
    }
    
    // Calculate how many bytes we can read
    bytes_to_read = UArr[fd].ptrfiletable->ptrinode->ActualFileSize - UArr[fd].ptrfiletable->ReadOffset;
    if (bytes_to_read > size) {
        bytes_to_read = size;
    }
    
    // Read data
    memcpy(brr, UArr[fd].ptrfiletable->ptrinode->Buffer + UArr[fd].ptrfiletable->ReadOffset, bytes_to_read);
    brr[bytes_to_read] = '\0'; // Null terminate
    
    UArr[fd].ptrfiletable->ReadOffset += bytes_to_read;
    
    // Update timestamp
    time(&t);
    strncpy(UArr[fd].ptrfiletable->ptrinode->LastAccessTime, ctime(&t), 
            sizeof(UArr[fd].ptrfiletable->ptrinode->LastAccessTime) - 1);
    
    return bytes_to_read;
}

int readWholeFile(int fd)
{
    int size = 0;
    time_t t;
    
    if (!validate_fd(fd)) {
        return VFS_ERROR_INVALID_PARAM;
    }
    
    if (UArr[fd].ptrfiletable->mode == WRITE)
    {
        return VFS_ERROR_PERMISSION_DENIED;
    }
    
    if (UArr[fd].ptrfiletable->ptrinode->ActualFileSize > 0) {
        printf("%s", UArr[fd].ptrfiletable->ptrinode->Buffer);
        size = UArr[fd].ptrfiletable->ptrinode->ActualFileSize;
    }
    
    // Update read offset to end
    UArr[fd].ptrfiletable->ReadOffset = UArr[fd].ptrfiletable->ptrinode->ActualFileSize;
    
    // Update timestamp
    time(&t);
    strncpy(UArr[fd].ptrfiletable->ptrinode->LastAccessTime, ctime(&t), 
            sizeof(UArr[fd].ptrfiletable->ptrinode->LastAccessTime) - 1);
    
    return size;
}

int Deleteall()
{
    PINODE temp = head;
    
    // Close all file descriptors
    for (int i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable != NULL)
        {
            close_file_by_fd(i);
        }
    }
    
    // Free all inodes
    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            if (temp->Buffer != NULL) {
                free(temp->Buffer);
                temp->Buffer = NULL;
            }
            temp->FileType = 0;
            memset(temp->Fname, 0, sizeof(temp->Fname));
            temp->FileSize = MAXFILESIZE;
            temp->ActualFileSize = 0;
            temp->ReferenceCount = 0;
            temp->Permission = 0;
            temp->LinkCount = 0;
            memset(temp->Birth, 0, sizeof(temp->Birth));
            memset(temp->LastAccessTime, 0, sizeof(temp->LastAccessTime));
            memset(temp->LastModifiedTime, 0, sizeof(temp->LastModifiedTime));
        }
        temp = temp->next;
    }
    
    superblockobj.FInode = MAXINODE;
    printf("All files deleted successfully.\n");
    return VFS_SUCCESS;
}

int truncate_file(char *name, int size)
{
    int fd;
    
    if (!validate_filename(name) || size < 0 || size > MAXFILESIZE) {
        return VFS_ERROR_INVALID_PARAM;
    }
    
    fd = findfd(name);
    if (fd < 0) {
        return VFS_ERROR_FILE_NOT_FOUND;
    }
    
    if (size <= UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
    {
        // Truncate to smaller size
        UArr[fd].ptrfiletable->ptrinode->Buffer[size] = '\0';
        UArr[fd].ptrfiletable->ptrinode->ActualFileSize = size;
        
        // Adjust offsets if necessary
        if (UArr[fd].ptrfiletable->ReadOffset > size) {
            UArr[fd].ptrfiletable->ReadOffset = size;
        }
        if (UArr[fd].ptrfiletable->WriteOffset > size) {
            UArr[fd].ptrfiletable->WriteOffset = size;
        }
    }
    else
    {
        // Extend file with spaces
        int current_size = UArr[fd].ptrfiletable->ptrinode->ActualFileSize;
        for (int i = current_size; i < size; i++)
        {
            UArr[fd].ptrfiletable->ptrinode->Buffer[i] = ' ';
        }
        UArr[fd].ptrfiletable->ptrinode->Buffer[size] = '\0';
        UArr[fd].ptrfiletable->ptrinode->ActualFileSize = size;
    }
    
    return VFS_SUCCESS;
}

int open_file(char *name, int mode)
{
    int i = 0;
    time_t t;
    PINODE temp;
    
    if (!validate_filename(name) || !validate_permission(mode)) {
        return VFS_ERROR_INVALID_PARAM;
    }
    
    temp = Get_Inode(name);
    if (temp == NULL)
    {
        return VFS_ERROR_FILE_NOT_FOUND;
    }
    
    // Check if file is already open with same mode
    for (i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable != NULL && 
            UArr[i].ptrfiletable->ptrinode == temp &&
            UArr[i].ptrfiletable->mode == mode)
        {
            UArr[i].ptrfiletable->count++;
            return i;
        }
    }
    
    // Find free UFDT entry
    for (i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable == NULL)
        {
            break;
        }
    }
    
    if (i == 50)
    {
        return VFS_ERROR_NO_SPACE;
    }
    
    // Allocate file table
    UArr[i].ptrfiletable = (PFT)malloc(sizeof(FT));
    if (UArr[i].ptrfiletable == NULL) {
        return VFS_ERROR_NO_SPACE;
    }
    
    UArr[i].ptrfiletable->count = 1;
    UArr[i].ptrfiletable->mode = mode;
    UArr[i].ptrfiletable->ReadOffset = 0;
    UArr[i].ptrfiletable->WriteOffset = 0;
    UArr[i].ptrfiletable->ptrinode = temp;
    
    (temp->ReferenceCount)++;
    
    time(&t);
    strncpy(temp->LastAccessTime, ctime(&t), sizeof(temp->LastAccessTime) - 1);
    
    return i;
}

int lseek_file(char *name, int size, int from)
{
    int fd = findfd(name);
    
    if (fd < 0) {
        return VFS_ERROR_FILE_NOT_FOUND;
    }
    
    if (!validate_fd(fd)) {
        return VFS_ERROR_INVALID_FD;
    }
    
    if ((UArr[fd].ptrfiletable->mode == READ) || (UArr[fd].ptrfiletable->mode == READ_WRITE))
    {
        if (from == START)
        {
            if (size < 0 || size > UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
            {
                return VFS_ERROR_INVALID_PARAM;
            }
            UArr[fd].ptrfiletable->ReadOffset = size;
        }
        else if (from == CURRENT)
        {
            int new_offset = UArr[fd].ptrfiletable->ReadOffset + size;
            if (new_offset < 0 || new_offset > UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
            {
                return VFS_ERROR_INVALID_PARAM;
            }
            UArr[fd].ptrfiletable->ReadOffset = new_offset;
        }
        else if (from == END)
        {
            int new_offset = UArr[fd].ptrfiletable->ptrinode->ActualFileSize - size;
            if (new_offset < 0 || new_offset > UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
            {
                return VFS_ERROR_INVALID_PARAM;
            }
            UArr[fd].ptrfiletable->ReadOffset = new_offset;
        }
    }
    
    if (UArr[fd].ptrfiletable->mode == WRITE || UArr[fd].ptrfiletable->mode == READ_WRITE)
    {
        if (from == START)
        {
            if (size < 0 || size > MAXFILESIZE)
            {
                return VFS_ERROR_INVALID_PARAM;
            }
            UArr[fd].ptrfiletable->WriteOffset = size;
        }
        else if (from == CURRENT)
        {
            int new_offset = UArr[fd].ptrfiletable->WriteOffset + size;
            if (new_offset < 0 || new_offset > MAXFILESIZE)
            {
                return VFS_ERROR_INVALID_PARAM;
            }
            UArr[fd].ptrfiletable->WriteOffset = new_offset;
        }
        else if (from == END)
        {
            int new_offset = MAXFILESIZE - size;
            if (new_offset < 0 || new_offset > MAXFILESIZE)
            {
                return VFS_ERROR_INVALID_PARAM;
            }
            UArr[fd].ptrfiletable->WriteOffset = new_offset;
        }
    }
    
    return VFS_SUCCESS;
}

void close_file(char *name)
{
    int found = 0;
    
    if (!validate_filename(name)) {
        printf("Error: Invalid filename\n");
        return;
    }
    
    for (int i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable != NULL && 
            UArr[i].ptrfiletable->ptrinode != NULL &&
            strcmp(UArr[i].ptrfiletable->ptrinode->Fname, name) == 0)
        {
            close_file_by_fd(i);
            found = 1;
        }
    }
    
    if (!found) {
        printf("File '%s' not found or not open\n", name);
    } else {
        printf("File '%s' closed successfully\n", name);
    }
}

void close_file_by_fd(int fd)
{
    if (!validate_fd(fd)) {
        return;
    }
    
    if (UArr[fd].ptrfiletable->ptrinode->ReferenceCount > 0) {
        (UArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
    }
    
    free(UArr[fd].ptrfiletable);
    UArr[fd].ptrfiletable = NULL;
}

void cat_file(char *name)
{
    PINODE temp = head;
    
    if (!validate_filename(name)) {
        printf("Error: Invalid filename\n");
        return;
    }
    
    while (temp != NULL)
    {
        if (temp->FileType != 0 && strcmp(temp->Fname, name) == 0)
        {
            if (temp->Buffer != NULL && temp->ActualFileSize > 0)
            {
                printf("%s\n", temp->Buffer);
            } else {
                printf("(File is empty)\n");
            }
            return;
        }
        temp = temp->next;
    }
    
    printf("Error: File '%s' not found\n", name);
}

// GUI specific functions
char* File_ls_gui() {
    static char buffer[4096] = "";
    int written = 0;
    PINODE temp = head;
    
    memset(buffer, 0, sizeof(buffer));
    
    strncpy(buffer, "Files in Virtual File System:\n", sizeof(buffer) - 1);
    written = strlen(buffer);
    
    while (temp != NULL && written < sizeof(buffer) - 100) {
        if (temp->FileType != 0) {
            int len = snprintf(buffer + written, sizeof(buffer) - written, 
                             "• %s (%d bytes)\n", temp->Fname, temp->ActualFileSize);
            if (len < 0 || len >= (int)(sizeof(buffer) - written)) break;
            written += len;
        }
        temp = temp->next;
    }
    
    if (written == strlen("Files in Virtual File System:\n")) {
        strncat(buffer, "No files found.\n", sizeof(buffer) - strlen(buffer) - 1);
    }
    
    return buffer;
}

char* stat_file_gui(char *name) {
    static char buffer[2048] = "";
    PINODE temp = head;

    memset(buffer, 0, sizeof(buffer));

    if (!validate_filename(name)) {
        strncpy(buffer, "Error: Invalid filename\n", sizeof(buffer) - 1);
        return buffer;
    }

    while (temp != NULL) {
        if (temp->FileType != 0 && strcmp(temp->Fname, name) == 0) {
            snprintf(buffer, sizeof(buffer),
                    "File Statistics: %s\n"
                    "-----------------------------\n"
                    "Inode number: %d\n"
                    "File size: %d/%d bytes\n"
                    "File type: %s\n"
                    "Birth: %s"
                    "Last Access: %s"
                    "Last Modified: %s"
                    "Permission: %s\n"
                    "Link count: %d\n"
                    "Reference count: %d",
                    temp->Fname,
                    temp->Ino,
                    temp->ActualFileSize, temp->FileSize,
                    (temp->FileType == REGULAR) ? "Regular" : "Unknown",
                    temp->Birth,
                    temp->LastAccessTime,
                    temp->LastModifiedTime,
                    (temp->Permission == READ) ? "READ ONLY" : 
                    (temp->Permission == WRITE) ? "WRITE ONLY" : 
                    (temp->Permission == READ_WRITE) ? "READ/WRITE" : "UNKNOWN",
                    temp->LinkCount,
                    temp->ReferenceCount);
            return buffer;
        }
        temp = temp->next;
    }

    strncpy(buffer, "Error: File not found\n", sizeof(buffer) - 1);
    return buffer;
}

char* cat_file_gui(char *name) {
    static char buffer[MAXFILESIZE + 100] = "";
    PINODE temp = head;
    
    memset(buffer, 0, sizeof(buffer));

    if (!validate_filename(name)) {
        strncpy(buffer, "Error: Invalid filename\n", sizeof(buffer) - 1);
        return buffer;
    }

    while (temp != NULL) {
        if (temp->FileType != 0 && strcmp(temp->Fname, name) == 0) {
            if (temp->Buffer != NULL && temp->ActualFileSize > 0) {
                strncpy(buffer, temp->Buffer, sizeof(buffer) - 1);
            } else {
                strncpy(buffer, "(File is empty)", sizeof(buffer) - 1);
            }
            return buffer;
        }
        temp = temp->next;
    }

    strncpy(buffer, "Error: File not found\n", sizeof(buffer) - 1);
    return buffer;
}

// Utility functions
int validate_fd(int fd) {
    return (fd >= 0 && fd < 50 && UArr[fd].ptrfiletable != NULL && UArr[fd].ptrfiletable->ptrinode != NULL);
}

int validate_filename(const char *name) {
    return (name != NULL && strlen(name) > 0 && strlen(name) < 50);
}

int validate_permission(int permission) {
    return (permission == READ || permission == WRITE || permission == READ_WRITE);
}