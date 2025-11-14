#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME_LENGTH 50
#define MAX_FILE_BLOCKS 100
typedef enum { TYPE_FILE, TYPE_DIRECTORY } NodeType;
typedef struct FreeBlock {
    int blockIndex;
    struct FreeBlock* next;
    struct FreeBlock* prev;
} FreeBlock;

typedef struct FileNode {
    char name[MAX_NAME_LENGTH];
    NodeType type;
    int contentSize;
    int blockPointers[MAX_FILE_BLOCKS];
    int numBlocks;
    struct FileNode* parent;
    struct FileNode* firstChild;
    struct FileNode* next;  
    struct FileNode* prev; 
} FileNode;

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock* freeListHead = NULL; 
FreeBlock* freeListTail = NULL;  
FileNode* root = NULL; 
FileNode* cwd = NULL; 
int totalBlocks = NUM_BLOCKS;
int usedBlocks = 0;

void initializeVFS();
void insertFreeBlock(int blockIndex);
int allocateBlock();
void freeBlock(int blockIndex);
FileNode* createNode(const char* name, NodeType type);
void insertChild(FileNode* parent, FileNode* child);
void removeChild(FileNode* parent, FileNode* child);
FileNode* findChild(FileNode* parent, const char* name);
void cmd_mkdir(const char* dirName);
void cmd_create(const char* fileName);
void cmd_write(const char* fileName, const char* content);
void cmd_read(const char* fileName);
void cmd_delete(const char* fileName);
void cmd_rmdir(const char* dirName);
void cmd_ls();
void cmd_cd(const char* path);
void cmd_pwd();
void cmd_df();
void cmd_exit();
void freeAllMemory(FileNode* node);
char* getCurrentPath();
void initializeVFS() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        insertFreeBlock(i);
        memset(virtualDisk[i], 0, BLOCK_SIZE);
    }
    root = createNode("/", TYPE_DIRECTORY);
    root->parent = NULL;
    cwd = root;
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
}

void insertFreeBlock(int blockIndex) {
    FreeBlock* newBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
    newBlock->blockIndex = blockIndex;
    newBlock->next = NULL;
    newBlock->prev = NULL; 
    if (freeListHead == NULL) {
        freeListHead = freeListTail = newBlock;
    } else {
        freeListTail->next = newBlock;
        newBlock->prev = freeListTail;
        freeListTail = newBlock;
    }
}

int allocateBlock() {
    if (freeListHead == NULL) {
        return -1;
    }
    FreeBlock* block = freeListHead;
    int blockIndex = block->blockIndex;
    freeListHead = freeListHead->next;
    if (freeListHead != NULL) {
        freeListHead->prev = NULL;
    } else {
        freeListTail = NULL;
    }
    free(block);
    usedBlocks++;
    return blockIndex;
}

void freeBlock(int blockIndex) {
    memset(virtualDisk[blockIndex], 0, BLOCK_SIZE);
    insertFreeBlock(blockIndex);
    usedBlocks--;
}

FileNode* createNode(const char* name, NodeType type) {
    FileNode* node = (FileNode*)malloc(sizeof(FileNode));
    strncpy(node->name, name, MAX_NAME_LENGTH - 1);
    node->name[MAX_NAME_LENGTH - 1] = '\0';
    node->type = type;
    node->contentSize = 0;
    node->numBlocks = 0;
    node->parent = NULL;
    node->firstChild = NULL;
    node->next = node;
    node->prev = node;
    for (int i = 0; i < MAX_FILE_BLOCKS; i++) {
        node->blockPointers[i] = -1;
    }
    return node;
}

void insertChild(FileNode* parent, FileNode* child) {
    child->parent = parent;
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
        child->next = child;
        child->prev = child;
    } else {
        FileNode* last = parent->firstChild->prev;
        last->next = child;
        child->prev = last;
        child->next = parent->firstChild;
        parent->firstChild->prev = child;
    }
}

void removeChild(FileNode* parent, FileNode* child) {
    if (child->next == child) {
        parent->firstChild = NULL;
    } else {
        child->prev->next = child->next;
        child->next->prev = child->prev;
        if (parent->firstChild == child) {
            parent->firstChild = child->next;
        }
    }
}

FileNode* findChild(FileNode* parent, const char* name) {
    if (parent->firstChild == NULL) {
        return NULL;
    }
    FileNode* current = parent->firstChild;
    do {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    } while (current != parent->firstChild);
    
    return NULL;
}

void cmd_mkdir(const char* dirName) {
    if (findChild(cwd, dirName) != NULL) {
        printf("Error: '%s' already exists.\n", dirName);
        return;
    }  
    FileNode* newDir = createNode(dirName, TYPE_DIRECTORY);
    insertChild(cwd, newDir);
    printf("Directory '%s' created successfully.\n", dirName);
}

void cmd_create(const char* fileName) {
    if (findChild(cwd, fileName) != NULL) {
        printf("Error: '%s' already exists.\n", fileName);
        return;
    }
    FileNode* newFile = createNode(fileName, TYPE_FILE);
    insertChild(cwd, newFile);
    printf("File '%s' created successfully.\n", fileName);
}

void cmd_write(const char* fileName, const char* content) {
    FileNode* file = findChild(cwd, fileName);
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", fileName);
        return;
    }
    if (file->type != TYPE_FILE) {
        printf("Error: '%s' is not a file.\n", fileName);
        return;
    }
    for (int i = 0; i < file->numBlocks; i++) {
        freeBlock(file->blockPointers[i]);
    }
    file->numBlocks = 0;
    int contentLen = strlen(content);
    int blocksNeeded = (contentLen + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (blocksNeeded > totalBlocks - usedBlocks) {
        printf("Error: Not enough disk space.\n");
        return;
    }
    int bytesWritten = 0;
    for (int i = 0; i < blocksNeeded; i++) {
        int blockIndex = allocateBlock();
        if (blockIndex == -1) {
            printf("Error: Disk full.\n");
            return;
        }
        file->blockPointers[i] = blockIndex;
        file->numBlocks++;
        
        int bytesToWrite = (contentLen - bytesWritten > BLOCK_SIZE) ? 
                           BLOCK_SIZE : (contentLen - bytesWritten);
        
        memcpy(virtualDisk[blockIndex], content + bytesWritten, bytesToWrite);
        bytesWritten += bytesToWrite;
    }
    file->contentSize = contentLen;
    printf("Data written successfully (size=%d bytes).\n", contentLen);
}

void cmd_read(const char* fileName) {
    FileNode* file = findChild(cwd, fileName);
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", fileName);
        return;
    }
    if (file->type != TYPE_FILE) {
        printf("Error: '%s' is not a file.\n", fileName);
        return;
    } 
    if (file->contentSize == 0) {
        printf("(empty file)\n");
        return;
    }
    for (int i = 0; i < file->numBlocks; i++) {
        int blockIndex = file->blockPointers[i];
        int bytesToRead = (i == file->numBlocks - 1) ? 
                          (file->contentSize % BLOCK_SIZE == 0 ? BLOCK_SIZE : file->contentSize % BLOCK_SIZE) : 
                          BLOCK_SIZE;
        
        for (int j = 0; j < bytesToRead; j++) {
            printf("%c", virtualDisk[blockIndex][j]);
        }
    }
    printf("\n");
}

void cmd_delete(const char* fileName) {
    FileNode* file = findChild(cwd, fileName);
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", fileName);
        return;
    } 
    if (file->type != TYPE_FILE) {
        printf("Error: '%s' is not a file.\n", fileName);
        return;
    }
    for (int i = 0; i < file->numBlocks; i++) {
        freeBlock(file->blockPointers[i]);
    }
    removeChild(cwd, file);
    free(file);
    printf("File deleted successfully.\n");
}

void cmd_rmdir(const char* dirName) {
    FileNode* dir = findChild(cwd, dirName);
    if (dir == NULL) {
        printf("Error: Directory '%s' not found.\n", dirName);
        return;
    }
    if (dir->type != TYPE_DIRECTORY) {
        printf("Error: '%s' is not a directory.\n", dirName);
        return;
    }
    if (dir->firstChild != NULL) {
        printf("Error: Directory '%s' is not empty.\n", dirName);
        return;
    }
    removeChild(cwd, dir);
    free(dir);
    printf("Directory removed successfully.\n");
}

void cmd_ls() {
    if (cwd->firstChild == NULL) {
        printf("(empty)\n");
        return;
    }
    FileNode* current = cwd->firstChild;
    do {
        if (current->type == TYPE_DIRECTORY) {
            printf("%s/\n", current->name);
        } else {
            printf("%s\n", current->name);
        }
        current = current->next;
    } while (current != cwd->firstChild);
}

void cmd_cd(const char* path) {
    if (strcmp(path, "..") == 0) {
        if (cwd->parent != NULL) {
            cwd = cwd->parent;
            printf("Moved to %s\n", getCurrentPath());
        } else {
            printf("Already at root directory.\n");
        }
        return;
    } 
    if (strcmp(path, "/") == 0) {
        cwd = root;
        printf("Moved to /\n");
        return;
    }
    FileNode* target = findChild(cwd, path);
    if (target == NULL) {
        printf("Error: Directory '%s' not found.\n", path);
        return;
    } 
    if (target->type != TYPE_DIRECTORY) {
        printf("Error: '%s' is not a directory.\n", path);
        return;
    }
    cwd = target;
    printf("Moved to %s\n", getCurrentPath());
}

void cmd_pwd() {
    printf("%s\n", getCurrentPath());
}

char* getCurrentPath() {
    static char path[1024];
    char temp[1024];
    if (cwd == root) {
        strcpy(path, "/");
        return path;
    }
    path[0] = '\0';
    FileNode* node = cwd;
    while (node != NULL && node != root) {
        strcpy(temp, path);
        sprintf(path, "/%s%s", node->name, temp);
        node = node->parent;
    }
    return path;
}

void cmd_df() {
    int freeBlocks = totalBlocks - usedBlocks;
    float usage = (usedBlocks * 100.0) / totalBlocks;
    printf("Total Blocks: %d\n", totalBlocks);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", usage);
}

void freeAllMemory(FileNode* node) {
    if (node == NULL) return;
    if (node->type == TYPE_DIRECTORY && node->firstChild != NULL) {
        FileNode* current = node->firstChild;
        FileNode* start = current;
        do {
            FileNode* next = current->next;
            freeAllMemory(current);
            current = next;
        } while (current != start);
    }
    if (node->type == TYPE_FILE) {
        for (int i = 0; i < node->numBlocks; i++) {
            freeBlock(node->blockPointers[i]);
        }
    }
    free(node);
}

void cmd_exit() {
    freeAllMemory(root);
    while (freeListHead != NULL) {
        FreeBlock* temp = freeListHead;
        freeListHead = freeListHead->next;
        free(temp);
    } 
    printf("Memory released. Exiting program...\n");
    exit(0);
}

int main() {
    initializeVFS();
    char input[256];
    char command[50], arg1[256], arg2[512];
    while (1) {
        if (cwd == root) {
            printf("/ > ");
        } else {
            printf("%s > ", cwd->name);
        }
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) {
            continue;
        }
        memset(command, 0, sizeof(command));
        memset(arg1, 0, sizeof(arg1));
        memset(arg2, 0, sizeof(arg2));
        if (strncmp(input, "write ", 6) == 0) {
            char* ptr = input + 6;
            sscanf(ptr, "%s", arg1);
            ptr = strchr(ptr, '"');
            if (ptr != NULL) {
                ptr++;
                char* endQuote = strrchr(ptr, '"');
                if (endQuote != NULL) {
                    *endQuote = '\0';
                    strcpy(arg2, ptr);
                }
            }
            strcpy(command, "write");
        } else {
            sscanf(input, "%s %s %s", command, arg1, arg2);
        }
        if (strcmp(command, "mkdir") == 0) {
            cmd_mkdir(arg1);
        } else if (strcmp(command, "create") == 0) {
            cmd_create(arg1);
        } else if (strcmp(command, "write") == 0) {
            cmd_write(arg1, arg2);
        } else if (strcmp(command, "read") == 0) {
            cmd_read(arg1);
        } else if (strcmp(command, "delete") == 0) {
            cmd_delete(arg1);
        } else if (strcmp(command, "rmdir") == 0) {
            cmd_rmdir(arg1);
        } else if (strcmp(command, "ls") == 0) {
            cmd_ls();
        } else if (strcmp(command, "cd") == 0) {
            cmd_cd(arg1);
        } else if (strcmp(command, "pwd") == 0) {
            cmd_pwd();
        } else if (strcmp(command, "df") == 0) {
            cmd_df();
        } else if (strcmp(command, "exit") == 0) {
            cmd_exit();
        } else {
            printf("Unknown command: %s\n", command);
        }
    }  
    return 0;
}