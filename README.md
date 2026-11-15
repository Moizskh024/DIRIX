# DIRIX — Smart File Directory Management System

DIRIX is a menu-driven **C11 console application** that simulates a hierarchical file directory using a **General Tree**. It supports directory and file management, navigation by absolute paths, recursive searching, and persistent storage using standard C file handling.

## Problem Statement

Design a program to simulate a file directory system using a tree data structure.

Required operations:
1. Create directories and subdirectories.
2. Insert files into a directory.
3. Search for a file or folder.
4. Display the directory structure in hierarchical form.

DIRIX implements these core requirements and extends them with rename, move, delete, item properties, statistics, and save/load functionality.

## Features

- Create directories and subdirectories.
- Add files with a user-specified size.
- Search the tree by exact item name.
- Display the complete hierarchy.
- Rename files and directories.
- Move items between directories.
- Delete files or complete directory subtrees.
- View item properties and directory statistics.
- Automatically load saved data at startup and save on exit.
- Prevent duplicate names within the same directory.
- Prevent moving a directory into itself or one of its descendants.

## Data Structure

DIRIX uses a **General Tree** with the **First-Child / Next-Sibling (FCNS)** representation.

Each node stores:
- `name`: file or directory name.
- `type`: directory or file.
- `file_size`: file size in bytes (zero for directories).
- `parent`: pointer to the parent directory.
- `first_child`: pointer to the first child.
- `next_sibling`: pointer to the next item in the same directory.

A general tree is suitable because a directory can contain any number of files and subdirectories.

## Persistence and File Handling

The tree is stored in `data/directory_data.txt`. DIRIX serializes each item as a record containing its type, absolute path, and file size.

C file handling functions used:
- `fopen()` — open the data file.
- `fprintf()` — write directory records.
- `fgets()` — read records.
- `fclose()` — close the file.

The program rebuilds pointers and tree relationships when loading. It does not write raw pointer values to disk.

## Project Structure

```text
DIRIX/
├── src/
│   ├── main.c
│   ├── tree.c
│   ├── tree.h
│   ├── storage.c
│   └── storage.h
├── data/
│   └── directory_data.txt
├── docs/
└── README.md
```

## Requirements

- GCC or another C11-compatible compiler.
- Windows, Linux, or macOS.
- No external libraries are required.

## Compile and Run

Run commands from the project root.

### Windows (GCC / MinGW / MSYS2)

```powershell
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/tree.c src/storage.c -o dirix.exe
.\dirix.exe
```

### Linux / macOS

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/tree.c src/storage.c -o dirix
./dirix
```

Make sure the `data` directory exists before running. It is included in this project.

## Console Menu

```text
========== DIRIX ==========
1. Create Directory
2. Add File
3. Search File / Directory
4. Display Directory Tree
5. Rename File / Directory
6. Move File / Directory
7. Delete File / Directory
8. View Item Properties
9. Directory Statistics
10. Save Now
0. Save and Exit
===========================
```

## Path Rules

- `/` represents the root directory.
- `/Documents` represents a directory directly under root.
- `/Documents/College` represents a nested directory.
- Paths and names are case-sensitive.
- Names cannot contain `/`, `\\`, or `|`.
- An item name must be unique within its parent directory; the same name can exist in different directories.

## Algorithms and Complexity

Let `n` be the number of nodes, `s` the number of siblings in a target directory, and `k` the number of nodes in a deleted subtree.

| Operation | Complexity | Notes |
|---|---:|---|
| Search by name | O(n) | Recursive depth-first search. |
| Display tree | O(n) | Visits every node once. |
| Statistics | O(n) | Traverses all nodes in selected subtree. |
| Add child | O(s) | Appends to the sibling list. |
| Delete subtree | O(k) | Frees every node in the subtree. |
| Move item | O(s + t) | Scans old and new sibling lists. |

## C Concepts Demonstrated

- Structures and pointers.
- General Tree and linked representation.
- Dynamic memory allocation using `malloc()`.
- Memory deallocation using `free()`.
- Recursion and depth-first traversal.
- String handling and path parsing.
- File handling and persistent storage.
- Modular programming using header and source files.

## Scope and Limitations

- DIRIX is a virtual directory simulator; it does not create or modify actual operating-system files and folders.
- File sizes are metadata entered by the user; file contents are not stored.
- Search by name returns the first matching item encountered. Absolute paths identify a specific item.
- Storage uses a simple text record format and is intended for educational use, not concurrent or production use.

## Academic Mapping

**Primary topic: Unit IV — Trees and Graphs**
- Tree terminology: root, parent, child, sibling, and leaf.
- General tree representation.
- Recursive traversal and searching.
- Dynamic memory allocation for linked tree nodes.

**Supporting C concepts:** structures, pointers, strings, functions, and file handling.

---

**Project:** DIRIX — Smart File Directory Management System  
**Language:** C11  
**Interface:** Console-based  
**Core Data Structure:** General Tree (First-Child / Next-Sibling)  
