// Define the block size for the file system (512 bytes per block)
#define BLOCKSIZE 512

/*
 * Superblock structure - Metadata about the file system
 * - num_inodes: Total number of inodes in the file system
 * - num_blocks: Total number of disk blocks available
 * - size_blocks: Size of each disk block in bytes
 */
struct superblock {
    int num_inodes;
    int num_blocks;
    int size_blocks;
};

/*
 * Inode structure - Metadata about a file
 * - size: File size in bytes
 * - first_block: Starting block number of the file's data
 * - name: File name (up to 8 characters)
 */
struct inode {
    int size;
    int first_block;
    char name[8];
};

/*
 * Disk Block structure - Data storage unit
 * - next_block_num: Linked list pointer to next block (-1 = free, -2 = end-of-file)
 * - data: Array to store block data (BLOCKSIZE bytes)
 */
struct disk_block {
    int next_block_num;
    char data[BLOCKSIZE];
};

// File System Management Functions
void create_fs();  // Initialize a new file system with default values
void mount_fs();   // Load existing file system from disk
void sync_fs();    // Write file system state to disk (persistence)

// File Operations
int allocate_file(char name[8]);  // Create new file, returns inode number
void set_filesize(int filenum, int size);  // Resize file allocation
void write_byte(int filenum, int pos, char *data);  // Write data to specific position

// Debugging
void print_fs();   // Print current file system state