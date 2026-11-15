

# DIRIX
### Smart File Directory Management System

**Structure. Organize. Navigate.**

DIRIX is a C-based console application that simulates a hierarchical file and directory management system using a **General Tree** data structure.

It uses the **First Child-Next Sibling (FCNS)** representation to organize files and directories dynamically, with persistent storage through C file handling.

## 1. Project Overview

DIRIX demonstrates how a hierarchical directory system can be implemented using a General Tree in C.

Each node represents either a file or a directory. Pointers maintain the parent, child, and sibling relationships needed to build and manage the hierarchy.

The project focuses on Data Structures, dynamic memory allocation, recursion, tree traversal, path handling, and persistent storage using file handling.

> DIRIX is a virtual directory simulator. It does not create, modify, or delete actual files and folders on the operating system.

## 2. Objectives

- Implement a General Tree in C.
- Represent multiple children using FCNS pointers.
- Create directories and subdirectories.
- Insert files into directories.
- Search for files and directories.
- Display the directory hierarchy.
- Implement deletion, renaming, and movement operations.
- Practice dynamic memory allocation and recursion.
- Save and restore directory information using C file handling.
- Prevent invalid operations and circular tree structures.

## 3. Technology Stack

| Component | Technology |
|---|---|
| Language | C |
| Standard | C11 |
| Data Structure | General Tree |
| Representation | First Child-Next Sibling (FCNS) |
| Compiler | GCC |
| Interface | Menu-driven console |
| Storage | C file handling |
| Environment | Windows / Linux / macOS |

---

## 4. Core Features

- Create directories and nested subdirectories.
- Insert file entries into directories.
- Display the directory tree recursively.
- Search for files and directories.
- Navigate and identify items using absolute paths.
- Rename existing files and directories.
- Move items between directories.
- Delete files or directories and their descendants.
- Prevent moving a directory into itself or one of its descendants.
- View item properties.
- Display directory statistics.
- Save directory information to a text file.
- Load saved directory information when the application starts.

### Input and hierarchy safeguards

- Duplicate names are not allowed within the same parent directory.
- The same name can be used in different directories.
- A directory cannot be moved into itself or any of its descendants.
- File size is stored as metadata entered by the user.

## 5. Data Structure: FCNS

DIRIX uses a **General Tree** implemented through the **First Child-Next Sibling (FCNS)** representation.

Each node contains fields for:

| Field | Purpose |
|---|---|
| `name` | Stores the file or directory name |
| `type` | Identifies whether the node is a file or directory |
| `file_size` | Stores file-size metadata |
| `parent` | Points to the parent directory |
| `first_child` | Points to the first child node |
| `next_sibling` | Points to the next node at the same level |

This representation supports a variable number of children without requiring a fixed-size child array.

### Example Directory Structure

```text
/
├── Documents/
│   ├── College/
│   │   └── assignment.pdf
│   └── resume.docx
├── Pictures/
│   └── photo.jpg
└── notes.txt
```

## 6. Project Structure

```text
DIRIX_C_Project/
├── src/
│   ├── main.c          # Menu and user interaction
│   ├── tree.c          # General Tree operations
│   ├── tree.h          # Tree structures and declarations
│   ├── storage.c       # Saving and loading directory data
│   └── storage.h       # Storage declarations
├── data/
│   └── directory_data.txt
├── docs/
│   └── VIVA_NOTES.md   # Viva preparation notes
├── .gitignore
├── Makefile
└── README.md
```

## 7. Compilation and Execution

### Prerequisites

- GCC compiler (MinGW-w64, MSYS2, or another GCC distribution on Windows).
- PowerShell, Command Prompt, Linux terminal, or macOS terminal.

### Windows (PowerShell)

Run these commands from the project root directory:

**Compile**

```powershell
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/tree.c src/storage.c -o dirix.exe
```

**Run**

```powershell
.\dirix.exe
```

### Linux / macOS

**Compile**

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/tree.c src/storage.c -o dirix
```

**Run**

```bash
./dirix
```

## 8. Application Menu

The application provides the following menu-driven operations:

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

## 9. Persistent Storage

DIRIX uses C file handling to preserve the virtual directory structure between program runs.

The data is stored in:

```text
data/directory_data.txt
```

The application saves directory and file records using their logical absolute paths and file-size metadata. During loading, it reads the records and rebuilds the tree and its pointer relationships.

File-handling functions used include:

- `fopen()` — opens the data file.
- `fprintf()` — writes directory records.
- `fgets()` — reads saved records.
- `fclose()` — closes the file.

**Storage note:** DIRIX stores directory information and metadata, not raw pointer addresses or actual file contents. The file size entered for a file is only simulated metadata.

## 10. Current Scope and Limitations

The current version is a console-based educational simulation.

- Directory and file entries are virtual nodes in the program's tree.
- Actual operating-system files and folders are not modified.
- Directory information is saved in a text file and restored when the application starts.
- File contents are not stored; only file names and size metadata are maintained.
- The interface is command-line based.
- The storage format is designed for this educational project, not concurrent or production use.

## 11. Future Enhancements

- Graphical user interface.
- Integration with actual operating-system file-system operations.
- More detailed file metadata management.
- Advanced filtering and search options.
- Import and export of directory structures.
- More extensive automated testing.

## 12. Academic Relevance

DIRIX demonstrates practical applications of:

- General Trees.
- First Child-Next Sibling representation.
- Structures and pointers.
- Dynamic memory allocation.
- Recursion and tree traversal.
- Searching and modification operations.
- Path handling and string processing.
- File handling and persistent storage.
- Memory management and input validation.

**Primary data-structure concept:** General Tree — representing a hierarchical directory system in which each directory can contain multiple files and subdirectories.

---


**DIRIX — Structure. Organize. Navigate.**


