#include "headers.h"

// Global variables definition
PINODE head = NULL;
UFDT UArr[50];
SB superblockobj;

void CreateDILB()
{
    PINODE temp = NULL, newn = NULL;
    for (int i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));
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
        newn->ReferenceCount = newn->ActualFileSize = newn->FileType = newn->LinkCount = 0;
        newn->Buffer = NULL;
        newn->Ino = i;
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
    if (name == NULL)
    {
        return NULL;
    }
    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            if (strcmp(temp->Fname, name) == 0)
            {
                break;
            }
        }
        temp = temp->next;
    }
    return temp;
}

int CreateFile(char *name, int permission)
{
    PINODE temp = head;
    time_t t;
    int i = 0;

    if ((name == NULL) || ((permission != READ) && (permission != WRITE) && (permission != (READ + WRITE))))
    {
        return -2;
    }
    if (superblockobj.FInode == 0)
    {
        return -3;
    }
    if (Get_Inode(name) != NULL)
    {
        return -1;
    }

    while (temp->FileType != 0)
    {
        temp = temp->next;
    }

    for (i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable == NULL)
        {
            break;
        }
    }

    PFT fileobj = (PFT)malloc(sizeof(FT));
    UArr[i].ptrfiletable = fileobj;
    UArr[i].ptrfiletable->ReadOffset = 0;
    UArr[i].ptrfiletable->WriteOffset = 0;
    UArr[i].ptrfiletable->count = 1;
    UArr[i].ptrfiletable->mode = permission;
    UArr[i].ptrfiletable->ptrinode = temp;
    UArr[i].ptrfiletable->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);
    UArr[i].ptrfiletable->ptrinode->Permission = permission;
    UArr[i].ptrfiletable->ptrinode->ActualFileSize = 0;
    UArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
    UArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
    UArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
    strcpy(UArr[i].ptrfiletable->ptrinode->Fname, name);
    
    time(&t);
    strcpy(UArr[i].ptrfiletable->ptrinode->Birth, ctime(&t));
    strcpy(UArr[i].ptrfiletable->ptrinode->LastAccessTime, ctime(&t));
    strcpy(UArr[i].ptrfiletable->ptrinode->LastModifiedTime, "-");

    (superblockobj.FInode)--;
    return i;
}

int remove_file(char *name)
{
    int i = 0;
    PINODE temp = head;

    for (i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable != NULL)
        {
            if (strcmp(UArr[i].ptrfiletable->ptrinode->Fname, name) == 0)
            {
                free(UArr[i].ptrfiletable->ptrinode->Buffer);
                UArr[i].ptrfiletable->ptrinode->FileType = 0;
                memset(UArr[i].ptrfiletable->ptrinode->Fname, '\0', 50);
                UArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
                UArr[i].ptrfiletable->ptrinode->ActualFileSize = 0;
                UArr[i].ptrfiletable->ptrinode->ReferenceCount = 0;
                UArr[i].ptrfiletable->ptrinode->Permission = 0;
                UArr[i].ptrfiletable->ptrinode->LinkCount = 0;
                UArr[i].ptrfiletable->ptrinode = NULL;
                free(UArr[i].ptrfiletable);
                UArr[i].ptrfiletable = NULL;
                break;
            }
        }
    }
    
    if (i == 50)
    {
        while (temp != NULL)
        {
            if (temp->FileType != 0)
            {
                if (strcmp(temp->Fname, name) == 0)
                {
                    free(temp->Buffer);
                    temp->FileType = 0;
                    memset(temp->Fname, '\0', 50);
                    temp->FileSize = MAXFILESIZE;
                    temp->ActualFileSize = 0;
                    temp->ReferenceCount = 0;
                    temp->Permission = 0;
                    temp->LinkCount = 0;
                    break;
                }
            }
            temp = temp->next;
        }
        if (temp == NULL)
        {
            return -1;
        }
    }
    (superblockobj.FInode)++;
    return 0;
}

int findfd(char *name)
{
    int i = 0;
    if (name == NULL)
    {
        return -1;
    }
    while (i < 50)
    {
        if (UArr[i].ptrfiletable != NULL)
        {
            if (strcmp(UArr[i].ptrfiletable->ptrinode->Fname, name) == 0)
            {
                break;
            }
        }
        i++;
    }
    if (i == 50)
    {
        return -2;
    }
    return i;
}

void File_ls()
{
    PINODE temp = head;
    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            printf("%s\n", temp->Fname);
        }
        temp = temp->next;
    }
}

int stat_file(char *name)
{
    PINODE temp = head;
    if (name == NULL)
    {
        return -1;
    }
    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            if (strcmp(temp->Fname, name) == 0)
            {
                break;
            }
        }
        temp = temp->next;
    }
    if (temp == NULL)
    {
        return -1;
    }
    printf("File name- %s\n", temp->Fname);
    printf("Inode number- %d\n", temp->Ino);
    printf("File size- %d\n", temp->FileSize);
    printf("File type- %d\n", temp->FileType);
    printf("Actual File size- %d\n", temp->ActualFileSize);
    printf("Birth- %s\n", temp->Birth);
    printf("Last Access Time- %s\n", temp->LastAccessTime);
    printf("Last Modified Time- %s\n", temp->LastModifiedTime);
    if (temp->Permission == READ)
    {
        printf("Permission- READ ONLY\n");
    }
    else if (temp->Permission == WRITE)
    {
        printf("Permission- WRITE ONLY\n");
    }
    else
    {
        printf("Permission- READ and WRITE\n");
    }
    printf("Link count- %d\n", temp->LinkCount);
    printf("Reference count- %d\n", temp->ReferenceCount);
    return 0;
}

int write_file(int fd, char *brr, int size)
{
    time_t t;
    if (UArr[fd].ptrfiletable->ptrinode->ActualFileSize == MAXFILESIZE)
    {
        return -1;
    }
    if ((UArr[fd].ptrfiletable->mode == READ) || (UArr[fd].ptrfiletable->ptrinode->Permission == READ))
    {
        return -2;
    }
    if (UArr[fd].ptrfiletable->WriteOffset + size >= MAXFILESIZE)
    {
        size = MAXFILESIZE - UArr[fd].ptrfiletable->WriteOffset;
    }
    strncpy((UArr[fd].ptrfiletable->ptrinode->Buffer) + (UArr[fd].ptrfiletable->WriteOffset), brr, size);
    UArr[fd].ptrfiletable->WriteOffset = (UArr[fd].ptrfiletable->WriteOffset) + size;
    UArr[fd].ptrfiletable->ptrinode->ActualFileSize += size;
    UArr[fd].ptrfiletable->ptrinode->Buffer[UArr[fd].ptrfiletable->ptrinode->ActualFileSize] = '\0';
    time(&t);
    strcpy(UArr[fd].ptrfiletable->ptrinode->LastModifiedTime, ctime(&t));
    return size;
}

int read_file(int fd, char *brr, int size)
{
    int size_t = 0;
    time_t t;
    if (UArr[fd].ptrfiletable->mode == WRITE)
    {
        return -1;
    }
    if (UArr[fd].ptrfiletable->ReadOffset == UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
    {
        return -2;
    }
    
    size_t = (UArr[fd].ptrfiletable->ptrinode->ActualFileSize) - (UArr[fd].ptrfiletable->ReadOffset);
    if (size_t < size)
    {
        strncpy(brr, (UArr[fd].ptrfiletable->ptrinode->Buffer) + (UArr[fd].ptrfiletable->ReadOffset), size_t);
        UArr[fd].ptrfiletable->ReadOffset += size_t;
        size = size_t;
    }
    else
    {
        strncpy(brr, (UArr[fd].ptrfiletable->ptrinode->Buffer) + (UArr[fd].ptrfiletable->ReadOffset), size);
        UArr[fd].ptrfiletable->ReadOffset += size;
    }
    time(&t);
    strcpy(UArr[fd].ptrfiletable->ptrinode->LastAccessTime, ctime(&t));
    return size;
}

int readWholeFile(int fd)
{
    int size = 0;
    time_t t;
    if (UArr[fd].ptrfiletable->mode == WRITE)
    {
        return -1;
    }
    printf("%s", UArr[fd].ptrfiletable->ptrinode->Buffer);
    size = strlen(UArr[fd].ptrfiletable->ptrinode->Buffer);
    time(&t);
    strcpy(UArr[fd].ptrfiletable->ptrinode->LastAccessTime, ctime(&t));
    return size;
}

int Deleteall()
{
    PINODE temp = head;
    int i = 0;
    for (i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable != NULL)
        {
            UArr[i].ptrfiletable->ptrinode = NULL;
            free(UArr[i].ptrfiletable);
            UArr[i].ptrfiletable = NULL;
        }
    }
    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            free(temp->Buffer);
            temp->Buffer = NULL;
            temp->FileType = 0;
            memset(temp->Fname, '\0', 50);
            temp->FileSize = MAXFILESIZE;
            temp->ActualFileSize = 0;
            temp->ReferenceCount = 0;
            temp->Permission = 0;
            temp->LinkCount = 0;
        }
        temp = temp->next;
    }
    printf("All Files Delete Successfully\n");
    return 0;
}

int truncate_file(char *name, int size)
{
    int start = 0;
    if (name == NULL)
    {
        return -1;
    }
    int fd = findfd(name);
    if (fd == -2)
    {
        printf("File not exist or not open\n");
        return -1;
    }
    if (size < 0)
    {
        printf("Invalid Parameter\n");
        return -1;
    }
    else if (size <= UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
    {
        UArr[fd].ptrfiletable->ptrinode->Buffer[size] = '\0';
        UArr[fd].ptrfiletable->ptrinode->ActualFileSize = size;
    }
    else if ((size > UArr[fd].ptrfiletable->ptrinode->ActualFileSize) && (size <= MAXFILESIZE))
    {
        for (start = UArr[fd].ptrfiletable->ptrinode->ActualFileSize; start < size; start++)
        {
            UArr[fd].ptrfiletable->ptrinode->Buffer[start] = ' ';
        }
        UArr[fd].ptrfiletable->ptrinode->Buffer[start-1] = '\0';
        UArr[fd].ptrfiletable->ptrinode->ActualFileSize = size;
    }
    else
    {
        printf("Invalid Parameter\n");
        return -1;
    }
    return 0;
}

int open_file(char *name, int mode)
{
    int i = 0;
    time_t t;
    if ((name == NULL) || ((mode != READ) && (mode != WRITE) && (mode != (READ + WRITE))))
    {
        return -1;
    }
    PINODE temp = Get_Inode(name);
    if (temp == NULL)
    {
        printf("File %s not exist\n", name);
        return -1;
    }
    while (i < 50)
    {
        if (UArr[i].ptrfiletable == NULL)
        {
            break;
        }
        i++;
    }
    if (i == 50)
    {
        printf("UFDT reaches its limit\n");
        return -1;
    }
    time(&t);
    UArr[i].ptrfiletable = (PFT)malloc(sizeof(FT));
    UArr[i].ptrfiletable->count = 1;
    UArr[i].ptrfiletable->mode = mode;
    UArr[i].ptrfiletable->ReadOffset = 0;
    UArr[i].ptrfiletable->WriteOffset = 0;
    UArr[i].ptrfiletable->ptrinode = temp;
    (UArr[i].ptrfiletable->ptrinode->ReferenceCount)++;
    strcpy(UArr[i].ptrfiletable->ptrinode->LastAccessTime, ctime(&t));
    return i;
}

int lseek_file(char *name, int size, int from)
{
    int fd = findfd(name);
    int size_t;
    if (fd == -1)
    {
        printf("Invalid Parameter");
    }
    if (fd == -2)
    {
        printf("File not found");
    }
    if ((UArr[fd].ptrfiletable->mode == READ) || (UArr[fd].ptrfiletable->mode == (READ + WRITE)))
    {
        if (from == START)
        {
            if (size > UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
            {
                return -1;
            }
            UArr[fd].ptrfiletable->ReadOffset = size;
        }
        else if (from == CURRENT)
        {
            if ((UArr[fd].ptrfiletable->ReadOffset + size) > (UArr[fd].ptrfiletable->ptrinode->ActualFileSize))
            {
                return -1;
            }
            if ((UArr[fd].ptrfiletable->ReadOffset + size) < 0)
            {
                return -1;
            }
            UArr[fd].ptrfiletable->ReadOffset = UArr[fd].ptrfiletable->ReadOffset + size;
        }
        else if (from == END)
        {
            if (size > UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
            {
                return -1;
            }
            UArr[fd].ptrfiletable->ReadOffset = UArr[fd].ptrfiletable->ptrinode->ActualFileSize - size;
        }
    }
    else if (UArr[fd].ptrfiletable->mode == WRITE)
    {
        if (from == START)
        {
            if (size > MAXFILESIZE)
            {
                return -1;
            }
            UArr[fd].ptrfiletable->WriteOffset = size;
        }
        else if (from == CURRENT)
        {
            if ((UArr[fd].ptrfiletable->WriteOffset + size) > MAXFILESIZE)
            {
                return -1;
            }
            if ((UArr[fd].ptrfiletable->WriteOffset + size) < 0)
            {
                return -1;
            }
            UArr[fd].ptrfiletable->WriteOffset = UArr[fd].ptrfiletable->WriteOffset + size;
        }
        else if (from == END)
        {
            if (size > MAXFILESIZE)
            {
                return -1;
            }
            UArr[fd].ptrfiletable->WriteOffset = MAXFILESIZE - size;
        }
    }
    return 1;
}

void close_file(char *name)
{
    int fd = 0, i = 0;
    for (i = 0; i < 50; i++)
    {
        if (UArr[i].ptrfiletable != NULL)
        {
            if ((strcmp(UArr[i].ptrfiletable->ptrinode->Fname, name) == 0) && (UArr[i].ptrfiletable->ptrinode->ReferenceCount > 0))
            {
                (UArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
                UArr[i].ptrfiletable->ptrinode = NULL;
                free(UArr[i].ptrfiletable);
                UArr[i].ptrfiletable = NULL;
                fd = 1;
            }
        }
    }
    if (fd == 0)
    {
        printf("File %s not exist or not open\n", name);
        return;
    }
    printf("File %s close successfully\n", name);
}

void cat_file(char *name)
{
    if (name == NULL)
    {
        printf("Invalid parameter\n");
        return;
    }
    PINODE temp = head;
    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            if (strcmp(temp->Fname, name) == 0)
            {
                if (temp->Buffer != NULL)
                {
                    printf("%s", temp->Buffer);
                }
                break;
            }
        }
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("File %s not exist\n", name);
        return;
    }
}
