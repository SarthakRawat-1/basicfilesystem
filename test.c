#include <stdio.h>
#include "filesystem.h"

void main() {
    // Create new file system (uncomment to initialize)
    // create_fs();
    // sync_fs();

    // Mount existing file system
    mount_fs();

    // Create new file
    // allocate_file("another");
    int file = allocate_file("onemore");

    // Set file size to 1000 bytes (allocates blocks)
    set_filesize(file, 1000);
    // set_filesize(0, 1000);

    // Write test data at regular intervals
    char data = 'b';
    // char data = 'a';

    for(int i = 0; i < 49; i++) {
        // Write to every 100th byte (tests multiple blocks)
        write_byte(file, i * 100, &data);
        // write_byte(0, i * 100, &data);
    }

    // Persist changes and print status
    sync_fs();
    print_fs();

    printf("Completed\n");
}