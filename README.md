# CustomisedVirtualFileSystem -
This project emulates file system functionality to create a virtual environment where users can perform file operations without interacting with a real disk

Customised Virtual File System (CVFS) – Purely Implementation in C

## Overview

Developed a Virtual File System (VFS) in C that simulates the core functionalities of a UNIX/Linux file system within a controlled in-memory environment.

The system provides a custom command-line shell to perform file operations without interacting with the actual disk, enabling a deeper understanding of low-level system design.

All file data is stored in RAM, making the system non-persistent and ideal for learning and experimentation with OS concepts.

## The project provides hands-on understanding of:

1. File handling
2. System calls
3. Memory management
4. Operating System internals

### Key Features
        Custom Data Structures
        Implemented all major file system components from scratch, including:  
            Inode & Inode Table
            File Table
            User Area (UAREA)
            User File Descriptor Table (UFDT)
            Super Block
            Disk Inode List Block
            Data Blocks
            Boot Block

### System Call Implementations
        Developed core file system calls using custom logic inspired by UNIX algorithms:
        open, close, read, write, lseek
        create, rm, ls
        stat
          
### Custom Shell Interface

Interactive Command-Line Interface (CLI)
Real-time execution of file system commands
User-friendly command structure for seamless interaction

### User-friendly CLI for executing commands.

### Provides real-time interaction with the CVFS
