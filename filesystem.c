#include "filesystem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Global File System Components
struct superblock sb;         // File system metadata
struct inode *inodes;         // Array of inodes
struct disk_block *dbs;       // Array of disk blocks

/*
 * Create a new file system
 * Initializes superblock, inodes, and disk blocks with default values
 */
void create_fs() {
    // Initialize superblock with default values
    sb.num_inodes = 10;       // Fixed number of inodes
    sb.num_blocks = 100;      // Total available blocks
    sb.size_blocks = sizeof(struct disk_block);

    // Allocate and initialize inodes
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for(int i = 0; i < sb.num_inodes; i++) {
        inodes[i].size = -1;           // -1 indicates unused inode
        inodes[i].first_block = -1;    // -1 = no blocks allocated
        strcpy(inodes[i].name, "sarthak"); // Default name (potential placeholder)
    }

    // Allocate and initialize disk blocks
    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    for(int i = 0; i < sb.num_blocks; i++) {
        dbs[i].next_block_num = -1;    // -1 = free block
    }
}

/*
 * Mount existing file system from disk
 * Reads saved state from 'fs_data' file
 */
void mount_fs() {
    FILE *file = fopen("fs_data", "r");
    
    // Read superblock
    fread(&sb, sizeof(struct superblock), 1, file);
    
    // Allocate memory for inodes and blocks
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    
    // Load inodes and blocks from file
    fread(inodes, sizeof(struct inode), sb.num_inodes, file);
    fread(dbs, sizeof(struct disk_block), sb.num_blocks, file);
    
    fclose(file);
}

/*
 * Sync file system to disk
 * Writes current state to 'fs_data' file
 */
void sync_fs() {
    FILE *file = fopen("fs_data", "w+");
    
    // Write superblock first
    fwrite(&sb, sizeof(struct superblock), 1, file);
    
    // Write inodes and blocks
    fwrite(inodes, sizeof(struct inode), sb.num_inodes, file);
    fwrite(dbs, sizeof(struct disk_block), sb.num_blocks, file);
    
    fclose(file);
}

/*
 * Print file system status for debugging
 */
void print_fs() {
    printf("Superblock Info:\n");
    printf("\tInodes: %d\n\tBlocks: %d\n\tBlock Size: %d\n", 
           sb.num_inodes, sb.num_blocks, sb.size_blocks);

    printf("\nInodes:\n");
    for(int i = 0; i < sb.num_inodes; i++) {
        printf("\tInode %d: Size: %d, First Block: %d, Name: %s\n",
               i, inodes[i].size, inodes[i].first_block, inodes[i].name);
    }

    printf("\nBlocks:\n");
    for(int i = 0; i < sb.num_blocks; i++) {
        printf("\tBlock %d: Next Block: %d\n", i, dbs[i].next_block_num);
    }
}


/* Helper Functions */

// Find first available inode (returns -1 if full)
int find_empty_inode() {
    for(int i = 0; i < sb.num_inodes; i++) {
        if(inodes[i].first_block == -1) return i;
    }
    return -1;
}

// Find first available disk block (returns -1 if full)
int find_empty_block() {
    for(int i = 0; i < sb.num_blocks; i++) {
        if(dbs[i].next_block_num == -1) return i;
    }
    return -1;
}

// Get block number at specified offset in file's block chain
int get_block_num(int file, int offset) {
    int togo = offset;
    int bn = inodes[file].first_block;

    while(togo > 0) {
        bn = dbs[bn].next_block_num;
        togo--;
    }

    return bn;
}

// Recursively truncate block chain from specified block onward
void shorten_file(int bn) {
    if(dbs[bn].next_block_num >= 0) {
        shorten_file(dbs[bn].next_block_num);
    }
    dbs[bn].next_block_num = -1; // Mark block as free
}

/* File Operations */

// Create new file with given name
int allocate_file(char name[8]) {
    int inode_num = find_empty_inode();
    if(inode_num == -1) return -1; // No free inodes

    int block_num = find_empty_block();
    if(block_num == -1) return -1; // No free blocks

    // Initialize inode
    inodes[inode_num].first_block = block_num;
    dbs[block_num].next_block_num = -2; // -2 marks allocated but end-of-chain
    strcpy(inodes[inode_num].name, name);
    
    return inode_num;
}

// Set file size by allocating/freeing blocks
void set_filesize(int filenum, int size) {
    // Calculate required blocks (rounding up)
    int num_blocks_needed = (size + BLOCKSIZE - 1) / BLOCKSIZE;
    int current_block = inodes[filenum].first_block;

    // Allocate additional blocks if needed
    for(int i = 1; i < num_blocks_needed; i++) {
        // Check if we need to allocate new block
        if(dbs[current_block].next_block_num == -2) {
            int new_block = find_empty_block();
            if(new_block == -1) return; // No space
            
            dbs[current_block].next_block_num = new_block;
            dbs[new_block].next_block_num = -2;
        }
        current_block = dbs[current_block].next_block_num;
    }

    // Truncate excess blocks if shrinking
    shorten_file(current_block);
    dbs[current_block].next_block_num = -2;

    // Update file size
    inodes[filenum].size = size;
}

// Write single byte to file at specified position
void write_byte(int filenum, int pos, char *data) {
    // Calculate target block and offset
    int relative_block = pos / BLOCKSIZE;
    int block_num = get_block_num(filenum, relative_block);
    int offset = pos % BLOCKSIZE;
    
    // Write data to block
    dbs[block_num].data[offset] = *data;
}