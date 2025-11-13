
#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitializeMemoryManager() {
    if (mem_manager != NULL) {
        // Cleanup existing manager
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
    }
    
    mem_manager = (PMEM_MANAGER)malloc(sizeof(MEM_MANAGER));
    if (!mem_manager) return;
    
    mem_manager->total_memory = TOTAL_MEMORY;
    mem_manager->free_memory = TOTAL_MEMORY;
    mem_manager->used_memory = 0;
    mem_manager->current_algorithm = FIRST_FIT;
    mem_manager->page_faults = 0;
    mem_manager->next_pid = 1;
    mem_manager->process_list = NULL;
    
    // Initialize with one big free block
    mem_manager->memory_list = (PMEMBLOCK)malloc(sizeof(MEMBLOCK));
    if (mem_manager->memory_list) {
        mem_manager->memory_list->start_address = 0;
        mem_manager->memory_list->size = TOTAL_MEMORY;
        mem_manager->memory_list->is_free = 1;
        mem_manager->memory_list->pid = -1;
        mem_manager->memory_list->next = NULL;
    }
    
    printf("Memory Manager Initialized: %d KB total memory\n", TOTAL_MEMORY / 1024);
}

int CreateProcess(char *name, int memory_required) {
    if (!mem_manager || !name || memory_required <= 0) return -1;
    
    if (memory_required > mem_manager->free_memory) {
        printf("Error: Not enough memory. Available: %d, Required: %d\n", 
               mem_manager->free_memory, memory_required);
        return -1;
    }
    
    // Create new process
    PPROCESS new_process = (PPROCESS)malloc(sizeof(PROCESS));
    if (!new_process) return -1;
    
    new_process->pid = mem_manager->next_pid++;
    strncpy(new_process->name, name, sizeof(new_process->name) - 1);
    new_process->name[sizeof(new_process->name) - 1] = '\0';
    new_process->memory_required = memory_required;
    new_process->pages_allocated = 0;
    new_process->page_table = NULL;
    new_process->state = 1; // running
    new_process->next = NULL;
    
    // Allocate memory for process
    int result = AllocateMemory(new_process->pid, memory_required, mem_manager->current_algorithm);
    if (result < 0) {
        free(new_process);
        printf("Error: Memory allocation failed for process %s\n", name);
        return -1;
    }
    
    // Add to process list
    if (!mem_manager->process_list) {
        mem_manager->process_list = new_process;
    } else {
        PPROCESS temp = mem_manager->process_list;
        while (temp->next) temp = temp->next;
        temp->next = new_process;
    }
    
    printf("Process created: PID=%d, Name=%s, Memory=%d bytes\n", 
           new_process->pid, new_process->name, memory_required);
    return new_process->pid;
}

int AllocateMemory(int pid, int size, ALLOC_ALGO algo) {
    if (!mem_manager || size <= 0) return -1;
    
    PMEMBLOCK current = mem_manager->memory_list;
    PMEMBLOCK best_block = NULL;
    PMEMBLOCK worst_block = NULL;
    
    switch (algo) {
        case FIRST_FIT:
            while (current) {
                if (current->is_free && current->size >= size) {
                    // Split the block if needed
                    if (current->size > size) {
                        PMEMBLOCK new_block = (PMEMBLOCK)malloc(sizeof(MEMBLOCK));
                        if (!new_block) return -1;
                        
                        new_block->start_address = current->start_address + size;
                        new_block->size = current->size - size;
                        new_block->is_free = 1;
                        new_block->pid = -1;
                        new_block->next = current->next;
                        
                        current->size = size;
                        current->next = new_block;
                    }
                    
                    current->is_free = 0;
                    current->pid = pid;
                    mem_manager->used_memory += size;
                    mem_manager->free_memory -= size;
                    return current->start_address;
                }
                current = current->next;
            }
            break;
            
        case BEST_FIT:
            while (current) {
                if (current->is_free && current->size >= size) {
                    if (!best_block || current->size < best_block->size) {
                        best_block = current;
                    }
                }
                current = current->next;
            }
            
            if (best_block) {
                if (best_block->size > size) {
                    PMEMBLOCK new_block = (PMEMBLOCK)malloc(sizeof(MEMBLOCK));
                    if (!new_block) return -1;
                    
                    new_block->start_address = best_block->start_address + size;
                    new_block->size = best_block->size - size;
                    new_block->is_free = 1;
                    new_block->pid = -1;
                    new_block->next = best_block->next;
                    
                    best_block->size = size;
                    best_block->next = new_block;
                }
                
                best_block->is_free = 0;
                best_block->pid = pid;
                mem_manager->used_memory += size;
                mem_manager->free_memory -= size;
                return best_block->start_address;
            }
            break;
            
        case WORST_FIT:
            while (current) {
                if (current->is_free && current->size >= size) {
                    if (!worst_block || current->size > worst_block->size) {
                        worst_block = current;
                    }
                }
                current = current->next;
            }
            
            if (worst_block) {
                if (worst_block->size > size) {
                    PMEMBLOCK new_block = (PMEMBLOCK)malloc(sizeof(MEMBLOCK));
                    if (!new_block) return -1;
                    
                    new_block->start_address = worst_block->start_address + size;
                    new_block->size = worst_block->size - size;
                    new_block->is_free = 1;
                    new_block->pid = -1;
                    new_block->next = worst_block->next;
                    
                    worst_block->size = size;
                    worst_block->next = new_block;
                }
                
                worst_block->is_free = 0;
                worst_block->pid = pid;
                mem_manager->used_memory += size;
                mem_manager->free_memory -= size;
                return worst_block->start_address;
            }
            break;
    }
    
    return -1; // No suitable block found
}

int DeallocateMemory(int pid) {
    if (!mem_manager) return -1;
    
    PMEMBLOCK current = mem_manager->memory_list;
    int freed_memory = 0;
    
    while (current) {
        if (current->pid == pid && !current->is_free) {
            current->is_free = 1;
            current->pid = -1;
            freed_memory += current->size;
        }
        current = current->next;
    }
    
    // Merge adjacent free blocks
    PMEMBLOCK temp = mem_manager->memory_list;
    while (temp && temp->next) {
        if (temp->is_free && temp->next->is_free) {
            temp->size += temp->next->size;
            PMEMBLOCK to_delete = temp->next;
            temp->next = to_delete->next;
            free(to_delete);
        } else {
            temp = temp->next;
        }
    }
    
    mem_manager->used_memory -= freed_memory;
    mem_manager->free_memory += freed_memory;
    return freed_memory;
}

int TerminateProcess(int pid) {
    if (!mem_manager) return -1;
    
    PPROCESS prev = NULL;
    PPROCESS current = mem_manager->process_list;
    
    while (current) {
        if (current->pid == pid) {
            // Deallocate memory
            int freed = DeallocateMemory(pid);
            
            // Remove from process list
            if (prev) {
                prev->next = current->next;
            } else {
                mem_manager->process_list = current->next;
            }
            
            if (current->page_table) {
                free(current->page_table);
            }
            free(current);
            
            printf("Process terminated: PID=%d, Freed memory: %d bytes\n", pid, freed);
            return freed;
        }
        prev = current;
        current = current->next;
    }
    
    printf("Error: Process with PID %d not found\n", pid);
    return -1; // Process not found
}

void DisplayMemoryMap() {
    if (!mem_manager) {
        printf("Memory manager not initialized. Use 'meminit' first.\n");
        return;
    }
    
    printf("\nMemory Map:\n");
    printf("-----------\n");
    printf("Total Memory: %d bytes\n", mem_manager->total_memory);
    printf("Used Memory:  %d bytes\n", mem_manager->used_memory);
    printf("Free Memory:  %d bytes\n", mem_manager->free_memory);
    printf("Allocation Algorithm: ");
    
    switch (mem_manager->current_algorithm) {
        case FIRST_FIT: printf("First Fit\n"); break;
        case BEST_FIT: printf("Best Fit\n"); break;
        case WORST_FIT: printf("Worst Fit\n"); break;
    }
    
    printf("\nMemory Blocks:\n");
    printf("Start\tSize\tStatus\tPID\n");
    printf("-----\t----\t------\t---\n");
    
    PMEMBLOCK current = mem_manager->memory_list;
    while (current) {
        printf("%d\t%d\t%s\t%d\n", 
               current->start_address,
               current->size,
               current->is_free ? "Free" : "Used",
               current->pid);
        current = current->next;
    }
}

void DisplayProcessList() {
    if (!mem_manager) {
        printf("Memory manager not initialized. Use 'meminit' first.\n");
        return;
    }
    
    if (!mem_manager->process_list) {
        printf("No processes running\n");
        return;
    }
    
    printf("\nProcess List:\n");
    printf("-------------\n");
    printf("PID\tName\tMemory\tState\n");
    printf("---\t----\t------\t-----\n");
    
    PPROCESS current = mem_manager->process_list;
    while (current) {
        printf("%d\t%s\t%d\t%s\n", 
               current->pid,
               current->name,
               current->memory_required,
               current->state == 1 ? "Running" : "Waiting");
        current = current->next;
    }
}

void SetAllocationAlgorithm(ALLOC_ALGO algo) {
    if (mem_manager) {
        mem_manager->current_algorithm = algo;
        printf("Allocation algorithm set to: ");
        switch (algo) {
            case FIRST_FIT: printf("First Fit\n"); break;
            case BEST_FIT: printf("Best Fit\n"); break;
            case WORST_FIT: printf("Worst Fit\n"); break;
        }
    } else {
        printf("Memory manager not initialized. Use 'meminit' first.\n");
    }
}

void MemoryStats() {
    if (!mem_manager) {
        printf("Memory manager not initialized. Use 'meminit' first.\n");
        return;
    }
    
    printf("\nMemory Statistics:\n");
    printf("==================\n");
    printf("Total memory: %d bytes\n", mem_manager->total_memory);
    printf("Used memory:  %d bytes\n", mem_manager->used_memory);
    printf("Free memory:  %d bytes\n", mem_manager->free_memory);
    printf("Utilization:  %.2f%%\n", 
           (float)mem_manager->used_memory / mem_manager->total_memory * 100);
    printf("Page faults:  %d\n", mem_manager->page_faults);
    
    int process_count = 0;
    PPROCESS current = mem_manager->process_list;
    while (current) {
        process_count++;
        current = current->next;
    }
    printf("Active processes: %d\n", process_count);
}

void CompactMemory() {
    if (!mem_manager) {
        printf("Memory manager not initialized. Use 'meminit' first.\n");
        return;
    }
    
    PMEMBLOCK current = mem_manager->memory_list;
    PMEMBLOCK new_list = NULL;
    PMEMBLOCK last_block = NULL;
    
    // Create new compacted list with used blocks first
    int current_address = 0;
    
    // First add all used blocks
    current = mem_manager->memory_list;
    while (current) {
        if (!current->is_free) {
            PMEMBLOCK new_block = (PMEMBLOCK)malloc(sizeof(MEMBLOCK));
            if (!new_block) return;
            
            new_block->start_address = current_address;
            new_block->size = current->size;
            new_block->is_free = 0;
            new_block->pid = current->pid;
            new_block->next = NULL;
            
            if (!new_list) {
                new_list = new_block;
                last_block = new_block;
            } else {
                last_block->next = new_block;
                last_block = new_block;
            }
            
            current_address += current->size;
        }
        current = current->next;
    }
    
    // Add one big free block at the end
    if (current_address < mem_manager->total_memory) {
        PMEMBLOCK free_block = (PMEMBLOCK)malloc(sizeof(MEMBLOCK));
        if (free_block) {
            free_block->start_address = current_address;
            free_block->size = mem_manager->total_memory - current_address;
            free_block->is_free = 1;
            free_block->pid = -1;
            free_block->next = NULL;
            
            if (!new_list) {
                new_list = free_block;
            } else {
                last_block->next = free_block;
            }
        }
    }
    
    // Free old list and update
    current = mem_manager->memory_list;
    while (current) {
        PMEMBLOCK next = current->next;
        free(current);
        current = next;
    }
    
    mem_manager->memory_list = new_list;
    printf("Memory compaction completed. All used blocks moved to the beginning.\n");
}

// GUI Functions
char* DisplayMemoryMapGUI() {
    static char buffer[4096] = "";
    if (!mem_manager) {
        strcpy(buffer, "Memory manager not initialized. Use 'meminit' first.");
        return buffer;
    }
    
    memset(buffer, 0, sizeof(buffer));
    
    snprintf(buffer, sizeof(buffer),
            "Memory Management Status\n"
            "=======================\n"
            "Total Memory: %d bytes\n"
            "Used Memory:  %d bytes\n"
            "Free Memory:  %d bytes\n"
            "Allocation:   %s\n\n"
            "Memory Blocks:\n"
            "Start\tSize\tStatus\tPID\n",
            mem_manager->total_memory,
            mem_manager->used_memory,
            mem_manager->free_memory,
            mem_manager->current_algorithm == FIRST_FIT ? "First Fit" :
            mem_manager->current_algorithm == BEST_FIT ? "Best Fit" : "Worst Fit");
    
    PMEMBLOCK current = mem_manager->memory_list;
    char temp[100];
    while (current && strlen(buffer) < sizeof(buffer) - 100) {
        snprintf(temp, sizeof(temp), "%d\t%d\t%s\t%d\n",
                current->start_address,
                current->size,
                current->is_free ? "Free" : "Used",
                current->pid);
        strncat(buffer, temp, sizeof(buffer) - strlen(buffer) - 1);
        current = current->next;
    }
    
    return buffer;
}

char* DisplayProcessListGUI() {
    static char buffer[2048] = "";
    if (!mem_manager) {
        strcpy(buffer, "Memory manager not initialized.");
        return buffer;
    }
    
    if (!mem_manager->process_list) {
        strcpy(buffer, "No processes running");
        return buffer;
    }
    
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "Running Processes:\nPID\tName\tMemory\tState\n");
    
    PPROCESS current = mem_manager->process_list;
    char temp[100];
    while (current && strlen(buffer) < sizeof(buffer) - 100) {
        snprintf(temp, sizeof(temp), "%d\t%s\t%d\t%s\n",
                current->pid,
                current->name,
                current->memory_required,
                current->state == 1 ? "Running" : "Waiting");
        strncat(buffer, temp, sizeof(buffer) - strlen(buffer) - 1);
        current = current->next;
    }
    
    return buffer;
}

char* MemoryStatsGUI() {
    static char buffer[1024] = "";
    if (!mem_manager) {
        strcpy(buffer, "Memory manager not initialized.");
        return buffer;
    }
    
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer),
            "Memory Statistics\n"
            "=================\n"
            "Total Memory: %d bytes\n"
            "Used Memory:  %d bytes\n"
            "Free Memory:  %d bytes\n"
            "Utilization:  %.2f%%\n"
            "Page Faults:  %d\n"
            "Active Processes: %d\n"
            "Algorithm: %s",
            mem_manager->total_memory,
            mem_manager->used_memory,
            mem_manager->free_memory,
            (float)mem_manager->used_memory / mem_manager->total_memory * 100,
            mem_manager->page_faults,
            mem_manager->process_list ? 1 : 0, // Simple count
            mem_manager->current_algorithm == FIRST_FIT ? "First Fit" :
            mem_manager->current_algorithm == BEST_FIT ? "Best Fit" : "Worst Fit");
    
    return buffer;
}

// CLI wrapper functions
int CreateProcessCLI(char *name, int memory) {
    return CreateProcess(name, memory);
}

int TerminateProcessCLI(int pid) {
    return TerminateProcess(pid);
}

void DisplayMemoryMapCLI() {
    DisplayMemoryMap();
}

void DisplayProcessListCLI() {
    DisplayProcessList();
}

void SetAllocAlgorithmCLI(int algo) {
    if (algo >= 0 && algo <= 2) {
        SetAllocationAlgorithm((ALLOC_ALGO)algo);
    } else {
        printf("Error: Invalid algorithm. Use 0=FirstFit, 1=BestFit, 2=WorstFit\n");
    }
}

void MemoryStatsCLI() {
    MemoryStats();
}

void CompactMemoryCLI() {
    CompactMemory();
}
