# Custom File System

This project implements a simple file system that combines concepts from both FAT (File Allocation Table) and inode-based systems. It is designed for educational purposes to demonstrate basic file system operations, including block allocation, file metadata management, and persistence.

## Overview

The file system uses a linked-block allocation strategy, where each disk block contains a pointer to the next block (similar to FAT). However, instead of a separate allocation table, the metadata is stored directly within each block structure. File metadata is managed using inodes, a design commonly seen in UNIX-like systems.

## Key Features

- **Superblock Management**:  
  Stores overall file system metadata such as the number of inodes, total disk blocks, and block size.
  
- **Inode-Based File Metadata**:  
  Each file is represented by an inode that stores the file size, starting block, and a short file name.
  
- **Linked-List Block Allocation**:  
  Each disk block contains a pointer to the next block in the file, using special values to indicate free blocks or end-of-file.
  
- **Basic File Operations**:
  - `create_fs()`: Initializes a new file system.
  - `mount_fs()`: Loads an existing file system from disk.
  - `sync_fs()`: Writes the current file system state to disk.
  - `allocate_file()`: Creates a new file and returns its inode number.
  - `set_filesize()`: Resizes a file's allocation.
  - `write_byte()`: Writes data at a specified position within a file.
  - `print_fs()`: Outputs the current state of the file system for debugging.

# Acknowledgement

This project was inspired by fundamental file system design concepts and is intended for learning and experimentation.
Lecture reference :- [File System Implementation](https://www.youtube.com/watch?v=n2AAhiujAqs)
